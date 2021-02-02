#include <process.h>
#include <chrono>
#include <thread>
#include "MemMgr.h"
#include "offsets.h"
#include "sdk\include\csgosdk.h"
#include "globals.h"
#include "Autowall.h"

void TracingTriggerBot(CUserCmd* cmd)
{
	C_BasePlayer* pl = nullptr;
	static bool is_shooting = false;
	static float stime = 0.0f;
	static float last_trigger = 0.0f;
	float time = g_GlobalVars->curtime;
	float d = 0.0f;

	if (stime > time)
		stime = 0.0f;
	if (last_trigger > time)
		last_trigger = 0.0f;

	if (g_TBMode == 0)
		return;

	if(g_TriggerHoldFire)
		is_shooting = false;

	if (is_shooting)
	{
		d = time - stime;

		if (d > 0.021f)
		{
			stime = 0.0f;
			is_shooting = false;
		}
		else
			return;
	}

	float fdelay = float(g_TPostDelay / 1000.0f);
	d = time - last_trigger;
	if (d <= fdelay)
		return;

	if (!g_LocalPlayer->IsAlive())
		return;

	C_BaseCombatWeapon* weap = g_LocalPlayer->m_hActiveWeapon();

	if (!weap)
		return;

	if (weap->IsDormant())
		return;

	if (!weap->IsWeapon())
		return;

	if (weap->IsKnife() || weap->IsGrenade())
		return;

	Vector vTraceStart = g_LocalPlayer->GetEyePos();
	Vector vTraceForward;
	QAngle qViewAngles = cmd->viewangles + g_LocalPlayer->m_aimPunchAngle() * 2.0f;
	Math::AngleVectors(qViewAngles, vTraceForward);
	vTraceForward = vTraceForward.Normalized();
	Vector vTraceEnd = vTraceStart + (vTraceForward * 8192.0f);
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = g_LocalPlayer;
	ray.Init(vTraceStart, vTraceEnd);
	trace_t tr;
	
	if (g_TBIgnoreWalls)
	{
		for (size_t i = 1; i < 65; i++)
		{
			C_BasePlayer* tmp = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
			if (!tmp)
				continue;

			if (!tmp->IsPlayer())
				continue;

			if (!tmp->IsAlive() || tmp->IsDormant())
				continue;

			if (tmp->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
				continue;

			g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, tmp, &tr);

			if (tr.DidHit())
			{
				if (tr.hit_entity == tmp)
				{
					pl = tmp;
					break;
				}
			}
		}
	}
	else
	{
		g_EngineTrace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &tr);
		pl = (C_BasePlayer*)tr.hit_entity;

		if (!pl)
			return;

		if (!pl->IsPlayer()
		 || pl->m_iTeamNum() == g_LocalPlayer->m_iTeamNum()
		 || !pl->IsAlive()
	     || pl->IsDormant())
		{
			return;
		}
	}


	if (!pl)
		return;

	if (tr.hitbox < 0 || tr.hitbox >= HITBOX_MAX)
		return;

	if (g_TriggerHeadsOnly && tr.hitbox != HITBOX_HEAD)
		return;

	bool fire = false;

	switch (g_TBMode)
	{
		case 1:
			WaitForSingleObject(hTriggerHotkeyMutex, INFINITE);
			fire = (GetAsyncKeyState(g_TriggerHotkey) & 0x8000);
			ReleaseMutex(hTriggerHotkeyMutex);
			break;
		case 2:
			fire = true;
			break;
		default:
			__assume(0);
	}

	if(fire)
	{
		cmd->buttons |= IN_ATTACK;
		stime = time;
		is_shooting = true;
		last_trigger = stime;
	}
}

void ClassicTriggerBot(CUserCmd* cmd)
{
	CMemMgr csgo_mem;
	int id, myTeam, enemyTeam, enemyHealth;
	DWORD LocalBase, EnemyInCrosshair;

	static bool is_shooting = false;
	static float stime = 0.0f;
	static float last_trigger = 0.0f;
	static float d = 0.0f;
	float time = g_GlobalVars->curtime;

	if (stime > time)
		stime = 0.0f;
	if (last_trigger > time)
		last_trigger = 0.0f;

	if (g_TBMode == 0)
		return;

	if (g_TriggerHoldFire)
		is_shooting = false;

	if (is_shooting)
	{
		d = time - stime;

		if (d > 0.021f)
		{
			stime = 0.0f;
			is_shooting = false;
		}
		else
			return;
	}

	float fdelay = float(g_TPostDelay / 1000.0f);
	d = time - last_trigger;
	if (d <= fdelay)
		return;

	if (!g_LocalPlayer)
		return;

	if (!g_LocalPlayer->IsAlive())
		return;

	C_BaseCombatWeapon* weap = g_LocalPlayer->m_hActiveWeapon();

	if (!weap)
		return;

	if (weap->IsDormant())
		return;

	if (!weap->IsWeapon())
		return;

	if (weap->IsKnife() || weap->IsGrenade())
		return;

	LocalBase = csgo_mem.Read<DWORD>(TEXT("client.dll"), off_dwLocalPlayer);
	id = csgo_mem.Read<int>(LocalBase, off_iCrossHairID);
	myTeam = csgo_mem.Read<int>(LocalBase, off_iTeam);
	EnemyInCrosshair = csgo_mem.Read<DWORD>(TEXT("client.dll"), off_EntityList + ((id - 1) * off_EntityLoopDistance));
	enemyTeam = csgo_mem.Read<int>(EnemyInCrosshair, off_iTeam);
	enemyHealth = csgo_mem.Read<int>(EnemyInCrosshair, off_iHealth);

	WaitForSingleObject(hTriggerHotkeyMutex, INFINITE);
	bool fire = (GetAsyncKeyState(g_TriggerHotkey) & 0x8000);
	ReleaseMutex(hTriggerHotkeyMutex);

	if (g_TBMode == 1 && !fire)
		return;

	if (myTeam != enemyTeam && enemyHealth > 0 && id > 0 && id < 65)
	{
		cmd->buttons |= IN_ATTACK;
		stime = time;
		is_shooting = true;
		last_trigger = stime;
	}
			
}