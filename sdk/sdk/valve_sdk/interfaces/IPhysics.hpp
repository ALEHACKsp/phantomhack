#pragma once
#include "../../helpers/vfunc_hook.hpp"

struct surfacephysicsparams_t
{
	float friction;
	float elasticity;
	float density;
	float thickness;
	float dampening;
};

struct surfaceaudioparams_t
{
	float audioReflectivity;
	float audioHardnessFactor;
	float audioRoughnessFactor;
	float scrapeRoughThreshold;
	float impactHardThreshold;
	float audioHardMinVelocity;
	float highPitchOcclusion;
	float midPitchOcclusion;
	float lowPitchOcclusion;
};

struct surfacesoundnames_t
{
	short walkLeft;
	short walkRight;
	short runLeft;
	short runRight;
	short impactsoft;
	short impacthard;
	short scrapesmooth;
	short scraperough;
	short bulletimpact;
	short rolling;
	short breakSound; // named "break" in vphysics.dll but since break is also a type rename it to breakSound
	short strain;
};

/*struct surfacegameprops_t
{
	float maxspeedfactor;
	float jumpfactor;
	float flPenetrationModifier;
	float flDamageModifier;
	uint16_t material;
	uint8_t climbable;
};*/

struct surfacegameprops_t
{
public:
	float    maxSpeedFactor;
	float    jumpFactor;
	//char    pad00[0x4];
	float    flPenetrationModifier;
	float    flDamageModifier;
	unsigned short    material;
	byte climbable;
	char    pad01[0x3];
};


struct surfacedata_t
{
	surfacephysicsparams_t	physics;
	surfaceaudioparams_t	audio;
	surfacesoundnames_t		sounds;
	surfacegameprops_t		game;
	char pad[48];
};

class IPhysicsSurfaceProps
{
public:
	virtual ~IPhysicsSurfaceProps(void) {}
	virtual int             ParseSurfaceData(const char *pFilename, const char *pTextfile) = 0;
	virtual int             SurfacePropCount(void) const = 0;
	virtual int             GetSurfaceIndex(const char *pSurfacePropName) const = 0;
	virtual void            GetPhysicsProperties(int surfaceDataIndex, float *density, float *thickness, float *friction, float *elasticity) const = 0;
	virtual surfacedata_t*  GetSurfaceData(int surfaceDataIndex) = 0;
	virtual const char*     GetString(unsigned short stringTableIndex) const = 0;
	virtual const char*     GetPropName(int surfaceDataIndex) const = 0;
	virtual void            SetWorldMaterialIndexTable(int *pMapArray, int mapSize) = 0;
	virtual void            GetPhysicsParameters(int surfaceDataIndex, surfacephysicsparams_t *pParamsOut) const = 0;

	surfacedata_t* GetSurfaceData2(int surfaceDataIndex)
	{
		typedef surfacedata_t*(__thiscall* GetSurfaceData_t)(void*, int);
		vfunc_hook hk;
		hk.setup(this);
		return hk.get_original<GetSurfaceData_t>(5)(this, surfaceDataIndex);
	}
};
