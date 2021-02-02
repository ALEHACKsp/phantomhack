#include "Aimbot.h"
#include "globals.h"

static D3DDEVICE_CREATION_PARAMETERS s_cparams;
static bool s_dcpi = false;

#define M_PI				3.14159265358979323846
#define MYSTERIOUS_DEGREE	57.295779513082f


void CAimbot::Init()
{
	m_lpLockedGuy = nullptr;
	m_uiAimState = STATE_IDLE;
	m_bHaveTarget = false;
	m_dtc = 0.0f;
	m_frequency = 0.001f;
	m_t = 0.0f;
	m_iHitbox = 0;

	if (!s_dcpi)
	{
		g_npDevice->GetCreationParameters(&s_cparams);
		s_dcpi = true;
	}

	UpdateResolution();
}

void CAimbot::Think(CUserCmd* cmd)
{
	static float prev_time = 0.0f;
	float cur_time = g_GlobalVars->curtime;
	if (g_Options.ab_mode == MODE_DISABLED)
		return;

	m_cmd = cmd;

	if(prev_time > cur_time)
		prev_time = 0.0f;

	 m_frequency = 0.001f + (0.03f * g_Options.ab_velocity_reduction);

	 
	if (g_Options.ab_mode != MODE_MANAGED)
	{
		if (CheckAimKey(true))
		{
			switch (m_uiAimState)
			{
			case STATE_IDLE:
				m_uiAimState = STATE_LOOKING_FOR_TARGET;
				m_t = 0.2f;
				break;
			case STATE_LOOKING_FOR_TARGET:
			case STATE_AIMING:
				Reset();
				return;
			default:
				__assume(0);
			}
		}
	}
	else if (g_Options.ab_mode == MODE_MANAGED)
	{
		if (CheckAimKey() && m_uiAimState == STATE_IDLE)
		{
			m_uiAimState = STATE_LOOKING_FOR_TARGET;
			m_t = 0.2f;
		}
		else if(!CheckAimKey())
		{
			Reset();
			return;
		}
	}

	if (m_uiAimState == STATE_IDLE)
	{
		Reset();
		return;
	}

	if (IsFiring() && g_Options.ab_mode == MODE_ONESHOT)
	{
		if (m_uiAimState == STATE_AIMING || m_uiAimState == STATE_LOOKING_FOR_TARGET)
		{
			Reset();
			return;
		}
	}

	if (m_uiAimState == STATE_AIMING)
	{
		if (!CheckConditions(m_lpLockedGuy))
		{
			m_uiAimState = STATE_LOOKING_FOR_TARGET;
			m_t = 0.2f;
			m_prev = m_dest;
			m_lpLockedGuy = nullptr; 
			m_bHaveTarget = false;
		}
	}

	if (m_uiAimState == STATE_LOOKING_FOR_TARGET)
	{
		GetClosestTargetToCrossHair();
		if (m_bHaveTarget)
			m_uiAimState = STATE_AIMING;
	}
	else
	{
		//do aiming
		if (cur_time - prev_time >= m_frequency)
		{
			UpdateDistanceToCrosshair();
			if (m_dtc > g_Options.ab_max_2Ddistance_to_crosshair)
			{
				m_t = 0.2f;
				m_uiAimState = STATE_LOOKING_FOR_TARGET;
				m_prev = m_dest;
				m_lpLockedGuy = nullptr;
				m_bHaveTarget = false;
				return;
			}
			MoveAimTowardsTarget();
			prev_time = cur_time;
			
		}
	}

}

void CAimbot::MoveAimTowardsTarget()
{
	QAngle va;
	if (m_t >= 0.980f)
	{
		if(g_Options.ab_method == METHOD_CURVED)
			GetDestinationAngle(m_dest);
		m_t = 0.2f * g_Options.ab_velocity;
	}
	
	switch (g_Options.ab_method)
	{
		case METHOD_LINEAR:
			GetDestinationAngle(m_dest);
			LineAngle(m_cmd->viewangles, m_dest, m_t);
			break;
		case METHOD_CURVED:
			CurveAngle(m_cmd->viewangles, m_dest, m_t);
			break;
		case METHOD_RAGE:
			GetDestinationAngle(m_dest);
			m_cmd->viewangles = m_dest;
			Math::NormalizeAngles(m_cmd->viewangles);
			break;
		default:
			__assume(0);
	}

	g_EngineClient->SetViewAngles(m_cmd->viewangles);
	m_t += g_Options.ab_acceleration;
}

void CAimbot::LineAngle(QAngle &src, const QAngle &dst, float t) 
{
	QAngle delta = src - dst;
	Math::NormalizeAngles(delta);
	float ot, pfactor = 1.0f, yfactor = 1.0f;

	ot = t;
	if (g_Options.ab_randomize_curve)
	{
		pfactor = Utils::GetRandomFloat(0.4f, 1.0f);
		yfactor = Utils::GetRandomFloat(0.4f, 1.0f);
	}

	if (t != 1.f && t != 0)
	{
		t = t * yfactor;
		ot = ot * pfactor;
	}

	delta.yaw *= t;
	
	delta.pitch *= ot;
	QAngle curved = src - delta;
	Math::NormalizeAngles(curved);
	m_cmd->viewangles = curved;






	////////////////////////////////////

	//float randomSmoothing = 1.0f;
	//if (randomized_smooth > 1.0f)
	//	randomSmoothing = Utils::GetRandomFloat(randomized_smooth / 10.0f, 1.0f);
}

void CAimbot::CurveAngle(QAngle &src, const QAngle &dst, float t) 
{
	QAngle delta = src - dst;
	Math::NormalizeAngles(delta);
	delta.yaw *= t;
	if (t != 1.f && t != 0)
		t = t * Utils::GetRandomFloat(0.7f, 1.0f);
	delta.pitch *= t;
	QAngle curved = src - delta;
	Math::NormalizeAngles(curved);
	m_cmd->viewangles = curved;
}

void CAimbot::GetDestinationAngle(QAngle &angle)
{
	Vector pos = m_lpLockedGuy->GetHitboxPos(m_iHitbox);
	Vector localpos = g_LocalPlayer->GetEyePos();
	Vector abspos = pos - localpos;
	Math::VectorAngles(abspos, angle);
}

void CAimbot::UpdateResolution()
{
	RECT s_rect;
	GetClientRect(s_cparams.hFocusWindow, &s_rect);
	m_iScreenCenterX = s_rect.right * 0.5f;
	m_iScreenCenterY = s_rect.bottom * 0.5f;
}

bool CAimbot::IsTargetOnScreen(LPBasePlayer p)
{
	float x, y, sw, sh;    
	Vector Head, w2sHead;
	RECT s_rect;
	Head = p->GetHitboxPos(HITBOX_HEAD);
	Math::WorldToScreen(Head, w2sHead);
	GetClientRect(s_cparams.hFocusWindow, &s_rect);
	x = w2sHead.x;
	y = w2sHead.y;
	sw = s_rect.right;
	sh = s_rect.bottom;

	if (x < 0 || x > sw)
		return false;
	
	if (y < 0 || y > sh)
		return false;

	return true;
}

std::string CAimbot::GetTargetName()
{
	if (m_bHaveTarget)
	{
		if (m_lpLockedGuy)
			return m_lpLockedGuy->GetPlayerInfo().szName;
		else
			return "none";
	}
	else
		return "none";
}

std::string CAimbot::GetState()
{
	switch (m_uiAimState)
	{
	case STATE_IDLE:
		return "Stopped";
		break;
	case STATE_LOOKING_FOR_TARGET:
		return "Looking for target";
		break;
	case STATE_AIMING:
		return "Aiming";
		break;

	default:
		__assume(0);
	}
}

bool CAimbot::IsFiring()
{
	return (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
}

float CAimbot::DistanceToCrosshair(float X, float Y)
{
	UpdateResolution();
	float distance = sqrt(pow(float(abs(X - m_iScreenCenterX)), 2) + (float(abs(Y - m_iScreenCenterY)), 2));
	return distance;
}

void CAimbot::UpdateDistanceToCrosshair()
{
	float X, Y;
	Vector Head, w2sHead;
	Head = m_lpLockedGuy->GetHitboxPos(HITBOX_HEAD);
	Math::WorldToScreen(Head, w2sHead);
	X = w2sHead.x;
	Y = w2sHead.y;
	UpdateResolution();
	m_dtc = sqrt(pow(float(abs(X - m_iScreenCenterX)), 2) + (float(abs(Y - m_iScreenCenterY)), 2));
}

bool CAimbot::CheckAimKey(bool switchy)
{
	if(switchy)
		return (GetAsyncKeyState(g_Options.ab_hotkey) & 1);
	else
		return (GetAsyncKeyState(g_Options.ab_hotkey) & 0x8000);
}

inline void CAimbot::GetClosestTargetToCrossHair()
{
	size_t EntMax = 65;
	LPBasePlayer player = NULL;
	int last_index = -1;
	float last_distance = g_Options.ab_max_2Ddistance_to_crosshair, current_distance;
	Vector hitbox, w2s;

	if (g_Options.ab_random_hitbox)
		m_iHitbox = Utils::GetRandomInt(0, HITBOX_MAX - 1);
	else
		m_iHitbox = HITBOX_HEAD;
	

	for (size_t i = 1; i < EntMax; i++)
	{
		player = (LPBasePlayer)g_EntityList->GetClientEntity(i);

		if (!CheckConditions(player))
			continue;

		hitbox = player->GetHitboxPos(m_iHitbox);
		Math::WorldToScreen(hitbox, w2s);
		current_distance = DistanceToCrosshair(w2s.x, w2s.y);
		if (current_distance < last_distance)
		{
			last_distance = current_distance;
			last_index = i;
		}
	}

	player = (LPBasePlayer)g_EntityList->GetClientEntity(last_index);

	if (last_index == -1)
	{
		m_bHaveTarget = false;
		m_dtc = 0.0f;
		m_lpLockedGuy = NULL;
		return;
	}

	m_dtc = last_distance;
	m_lpLockedGuy = player; 
	m_bHaveTarget = true;
	GetDestinationAngle(m_dest);
}

void CAimbot::Reset()
{
	m_t = 0.2f;
	m_uiAimState = STATE_IDLE;
	m_lpLockedGuy = nullptr; 
	m_bHaveTarget = false;
}

inline bool CAimbot::CheckConditions(LPBasePlayer p)
{
	if (!g_LocalPlayer)
		return false;

	if (!g_LocalPlayer->IsAlive())
		return false;

	auto weapon = g_LocalPlayer->m_hActiveWeapon();

	if (!weapon)
		return false;

	if (weapon->IsDormant())
		return false;

	if (!weapon->IsWeapon())
		return false;

	if (weapon->IsKnife() || weapon->IsGrenade())
		return false;

	if (g_Options.ab_aim_only_when_scoped && !g_LocalPlayer->m_bIsScoped())
		return false;

	if (!p)
		return false;

	if (p->IsDormant())
		return false;

	if (!p->IsPlayer())
		return false;

	if (p == g_LocalPlayer)
		return false;

	if (!p->IsAlive())
		return false;
	
	if (g_LocalPlayer->m_iTeamNum() == p->m_iTeamNum())
		return false;
	
	if (!g_LocalPlayer->CanSeePlayer(p, m_iHitbox))
		return false;

	if(!IsTargetOnScreen(p))
		return false;

	auto pos = g_LocalPlayer->m_vecOrigin();
	auto epos = p->m_vecOrigin();
	float distance = pos.DistTo(epos);

	if (distance > g_Options.ab_max_reaction_distance)
		return false;

	if (distance < g_Options.ab_min_reaction_distance)
		return false;

	if (g_Options.ab_hunt_player)
	{
		std::string pname = p->GetPlayerInfo().szName;
		if (pname.find(g_Options.ab_filter_name.GetStr()) == std::string::npos)
			return false;
	}

	if(g_Options.ab_disable_mouse)
		g_EngineClient->ExecuteClientCmd("sensitivity 0");
	return true;
}