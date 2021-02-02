#pragma once

#include "sdk\include\csgosdk.h"


/*
TODO:

1. Outdated. Shows wrong damage. Fix it.
2. Fix reaction on nonexistent entities
*/


#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
//#define CHAR_TEX_UNUSED		'J'
#define CHAR_TEX_SNOW			'K'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
//#define CHAR_TEX_UNUSED		'Q'
#define CHAR_TEX_REFLECTIVE		'R'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'  
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
//#define CHAR_TEX_UNUSED		'X' ///< do not use - "fake" materials use this (ladders, wading, clips, etc)
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.

#define CHAR_TEX_STEAM_PIPE		11

#define TEAM_CT					2
#define TEAM_T					3


using IsBreakableEntity_t			= bool(__thiscall*)(IClientEntity*);
using UTIL_ClipTraceToPlayers_t = void(__thiscall*)(const Vector&, const Vector&, unsigned int, ITraceFilter*, trace_t*);

class HitGroups
{
private:
	int i;
public:

	enum 
	{
		HITGROUP_GENERIC,
		HITGROUP_HEAD,
		HITGROUP_CHEST,
		HITGROUP_STOMACH,
		HITGROUP_LEFTARM,
		HITGROUP_RIGHTARM,
		HITGROUP_LEFTLEG,
		HITGROUP_RIGHTLEG,
		HITGROUP_GEAR = 10,
	};

	HitGroups()
	{
		i = 0;
	}

	HitGroups(int ii)
	{
		i = ii;
	}

	HitGroups(const HitGroups& cpy)
	{
		i = cpy;
	}

	HitGroups& operator=(int ii)
	{
		i = ii;
		return *this;
	}
	
	operator int() const
	{
		return i;
	}
};

struct FireBulletData
{
	FireBulletData(const Vector &eye_pos)
		: src(eye_pos)
	{
	}

	Vector           src;
	trace_t          enter_trace;
	Vector           direction;
	CTraceFilter    filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

bool CanHit(C_BaseEntity* entity, const Vector &point, float *damage_given);
bool CanWallbang(int &dmg);