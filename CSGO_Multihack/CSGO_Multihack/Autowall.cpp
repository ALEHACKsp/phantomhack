#include "sdk\include\csgosdk.h"
#include "Autowall.h"

//#include "R.h"

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define	HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10


bool HandleBulletPenetration(CCSWeaponInfo *wpn_data, FireBulletData &data);

float GetHitgroupDamageMult(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_GENERIC:
		return 1.f;
	case HITGROUP_HEAD:
		return 4.f;
	case HITGROUP_CHEST:
		return 1.f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTARM:
		return 1.f;
	case HITGROUP_RIGHTARM:
		return 1.f;
	case HITGROUP_LEFTLEG:
		return 0.75f;
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	case HITGROUP_GEAR:
		return 1.f;
	default:
		break;
	}

	return 1.f;
}
bool IsArmored(C_BaseEntity* Entity, int ArmorValue, int Hitgroup)
{
	bool result = false;

	if (ArmorValue > 0)
	{
		switch (Hitgroup)
		{
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			result = true;
			break;
		case HITGROUP_HEAD:
			result = ((C_BasePlayer*)Entity)->m_bHasHelmet(); // DT_CSPlayer -> m_bHasHelmet
			break;
		}
	}

	return result;
}

void ScaleDmg(int Hitgroup, C_BaseEntity* Entity, float WeaponArmorRatio, float &Damage)
{
	auto HeavyArmor = ((C_BasePlayer*)Entity)->m_bHasHeavyArmor(); 
	auto ArmorValue = ((C_BasePlayer*)Entity)->m_ArmorValue(); 
	switch (Hitgroup)
	{
	case HITGROUP_HEAD:
		if (HeavyArmor)
			Damage = (Damage * 4.f) * 0.5f;
		else
			Damage *= 4.f;
		break;
	case HITGROUP_STOMACH:
		Damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		Damage *= 0.75f;
		break;
	}

	if (IsArmored(Entity, ArmorValue, Hitgroup))
	{
		float v47 = 1.f, ArmorBonusRatio = 0.5f, ArmorRatio = WeaponArmorRatio * 0.5f;

		if (HeavyArmor)
		{
			ArmorBonusRatio = 0.33f;
			ArmorRatio = (WeaponArmorRatio * 0.5f) * 0.5f;
			v47 = 0.33f;
		}

		auto NewDamage = Damage * ArmorRatio;

		if (HeavyArmor)
			NewDamage *= 0.85f;

		if (((Damage - (Damage * ArmorRatio)) * (v47 * ArmorBonusRatio)) > ArmorValue)
			NewDamage = Damage - (ArmorValue / ArmorBonusRatio);

		Damage = NewDamage;
	}
}

void UTIL_ClipTraceToPlayers(C_BaseEntity* pEntity, Vector start, Vector end, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	trace_t playerTrace;
	Ray_t ray;
	float smallestFraction = tr->fraction;

	ray.Init(start, end);

	if (!pEntity || !((C_BasePlayer*)pEntity)->IsAlive() || pEntity->IsDormant())
		return;

	if (filter && filter->ShouldHitEntity(pEntity, mask) == false)
		return;

	g_EngineTrace->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, pEntity, &playerTrace);
	if (playerTrace.fraction < smallestFraction)
	{
		// we shortened the ray - save off the trace
		*tr = playerTrace;
		smallestFraction = playerTrace.fraction;
	}
}

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, C_BaseEntity *ignore, int collisionGroup, trace_t *ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter traceFilter;
	traceFilter.pSkip = ignore;
	g_EngineTrace->TraceRay(ray, mask, &traceFilter, ptr);
}


bool SimulateFireBullet(C_BaseEntity* entity, C_BaseEntity *local, C_BaseCombatWeapon *weapon, FireBulletData &data)
{
	//Utils::ToLog("SimulateFireBullet");
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto *wpn_data = weapon->GetCSWeaponData();

	data.current_damage = static_cast<float>(wpn_data->iDamage);

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->flRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		UTIL_TraceLine(data.src, end, 0x4600400B, local, 0, &data.enter_trace);
		UTIL_ClipTraceToPlayers(entity, data.src, end + data.direction * 40.f, 0x4600400B, &data.filter, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if ((data.enter_trace.hitgroup <= 7)
			&& (data.enter_trace.hitgroup > 0)
			&& (local->m_iTeamNum() != ((C_BasePlayer*)data.enter_trace.hit_entity)->m_iTeamNum()))
		{
			data.trace_length += (float)(data.enter_trace.fraction * data.trace_length_remaining);
			data.current_damage *= (float)(pow(wpn_data->flRangeModifier, data.trace_length * 0.002));
			ScaleDmg(data.enter_trace.hitgroup, (C_BaseEntity*)data.enter_trace.hit_entity, wpn_data->flArmorRatio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(wpn_data, data))
			break;
	}

	return false;
}

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static DWORD TraceToExit = (DWORD)Utils::PatternScan(GetModuleHandle(L"client.dll"), "55 8B EC 83 EC 30 F3 0F 10 75");

	if (!TraceToExit)
		return false;

	float start_y = start.y, start_z = start.z, start_x = start.x, dir_y = vEnd.y, dir_x = vEnd.x, dir_z = vEnd.z;

	_asm
	{
		push trace
		push dir_z
		push dir_y
		push dir_x
		push start_z
		push start_y
		push start_x
		mov edx, tr
		mov ecx, end
		call TraceToExit
		add esp, 0x1C
	}
}

inline vec_t VectorLength(const Vector& v)
{
	return (vec_t)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

bool HandleBulletPenetration(CCSWeaponInfo *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = g_PhysSurface->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;


	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= (float)(pow(wpn_data->flRangeModifier, (data.trace_length * 0.002)));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;
	if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = g_PhysSurface->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->flPenetration) * 1.25f);
	float thickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;


	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

/*
*    CanHit() - example of how to use the code
*     @in  point: target hitbox vector
*     @out damage_given: amount of damage the shot would do
*/

bool CanHit(C_BaseEntity* entity, const Vector &point, float *damage_given)
{
	//Utils::ToLog("CANHIT");
	auto data = FireBulletData(g_LocalPlayer->GetEyePos());
	data.filter = CTraceFilter();
	data.filter.pSkip = g_LocalPlayer;

	QAngle angles;
	//	CalcAngle(data.src, point, angles);
	Math::VectorAngles(point - data.src, angles);
	Math::AngleVectors(angles, data.direction);
	data.direction = data.direction.Normalized();

	if (SimulateFireBullet(entity, g_LocalPlayer, g_LocalPlayer->m_hActiveWeapon(), data))
	{
		*damage_given = data.current_damage;
		//Utils::ToLog("CANHIT END");
		return true;
	}

	return false;
}

void CalcAngle(const Vector &lpos, const Vector &pos, QAngle &angle)
{
	Vector delta = lpos - pos;
	double hyp = delta.Length2D();
	angle.yaw = (atan(delta.y / delta.x) * 57.295779513082f);
	angle.pitch = (vec_t)(atan(delta.z / hyp) * 57.295779513082f);
	angle[2] = 0.00;

	if (delta.x >= 0.0)
		angle.yaw += 180.0f;
}


bool CanWallbang(int &dmg)
{

	if (!g_LocalPlayer)
		return false;

	if (!g_LocalPlayer->IsAlive())
		return false;

	auto weapon = g_LocalPlayer->m_hActiveWeapon();

	FireBulletData data = FireBulletData(g_LocalPlayer->GetEyePos());
	data.filter = CTraceFilter();
	data.filter.pSkip = g_LocalPlayer;
	auto flRange = weapon->GetCSWeaponData()->flRange;
	QAngle EyeAng;
	g_EngineClient->GetViewAngles(EyeAng);

	Vector dst, forward;

	Math::AngleVectors(EyeAng, forward);
	dst = data.src + (forward * 8000.f);

	QAngle angles;
	CalcAngle(data.src, dst, angles);
	Math::AngleVectors(angles, data.direction);
	data.direction = data.direction.Normalized();



	if (!weapon)
		return false;

	data.penetrate_count = 1;

	auto weaponData = weapon->GetCSWeaponData();

	if (!weaponData)
		return false;

	data.current_damage = (int)weaponData->iDamage;

	data.trace_length_remaining = weaponData->flRange;

	Vector end = data.src + data.direction * data.trace_length_remaining;

	UTIL_TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, g_LocalPlayer, 0, &data.enter_trace);

	if (data.enter_trace.fraction == 1.f)
		return false;

	if (HandleBulletPenetration(weaponData, data))
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}