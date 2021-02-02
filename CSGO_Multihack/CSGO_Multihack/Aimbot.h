#pragma once

#include "sdk\include\csgosdk.h"

/*
TODO:
1. Remove unused vars and methods
3. Fix linear movement
2. Implement curved aimbot movement
*/

typedef C_BasePlayer* LPBasePlayer;

class CAimbot
	: public Singleton<CAimbot>
{
public:
	enum 
	{
		MODE_DISABLED,
		MODE_ONESHOT,
		MODE_TOGGLE,
		MODE_MANAGED
	};

	enum
	{
		METHOD_LINEAR,
		METHOD_CURVED,
		METHOD_RAGE
	};

private:
	LPBasePlayer m_lpLockedGuy;

	UINT m_uiAimState;
	int m_iScreenCenterX;
	int m_iScreenCenterY;
	bool m_bHaveTarget;
	int m_iHitbox;

	float m_dtc;
	float m_frequency;
	float m_t;
	QAngle m_prev;
	QAngle m_dest;
	CUserCmd* m_cmd;

	enum
	{
		STATE_IDLE,
		STATE_LOOKING_FOR_TARGET,
		STATE_AIMING,
	};

public:
	void Init();
	void Think(CUserCmd* cmd);
	std::string GetTargetName();
	std::string GetState();

protected:
	void UpdateResolution();
	bool IsTargetOnScreen(LPBasePlayer p);
	float DistanceToCrosshair(float X, float Y);
	void UpdateDistanceToCrosshair();
	bool CheckAimKey(bool switchy = false);
	inline void GetClosestTargetToCrossHair();
	bool IsFiring();
	void Reset();
	inline bool CheckConditions(LPBasePlayer p);
	void GetDestinationAngle(QAngle &angle);
	void MoveAimTowardsTarget();

	void CurveAngle(QAngle &src, const QAngle &dst, float t);
	void LineAngle(QAngle &src, const QAngle &dst, float t);
};