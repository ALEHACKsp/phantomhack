#pragma once

#include "../valve_sdk/sdk.hpp"

#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

namespace Math
{
	void NormalizeAngles(QAngle& angles);
	void ClampAngles(QAngle& angles);
	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	void AngleVectors(const QAngle &angles, Vector& forward);
	void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up);
	void VectorAngles(const Vector& forward, QAngle& angles);
	bool WorldToScreen(const Vector& in, Vector& out);
	vec_t DotProduct(const Vector& a, const Vector& b);
	float flGetDistance(Vector from, Vector to);
	void QAngleToVector(QAngle &qa, Vector &vec);
	void VectorToQAngle(Vector &vec, QAngle &qa);

	inline bool SanitizeAngles(QAngle &angles)
	{
		QAngle temp = angles;
		Math::NormalizeAngles(temp);
		Math::ClampAngles(temp);

		int iy = isfinite(temp.yaw);
		int ip = isfinite(temp.pitch);
		int ir = isfinite(temp.roll);

		if (!isfinite(temp.yaw) || !isfinite(temp.pitch) || !isfinite(temp.roll))
			return false;

		angles = temp;

		return true;
	}
}