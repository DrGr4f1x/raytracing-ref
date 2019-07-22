#pragma once

#include "vec3.h"

class Ray
{
public:
	Ray() = default;
	Ray(const Vec3& origin, const Vec3& direction, float time = 0.0f)
		: m_origin(origin)
		, m_direction(direction)
		, m_time(time)
	{}

	const Vec3& origin() const { return m_origin; }
	const Vec3& direction() const { return m_direction; }
	float time() const { return m_time; }

	Vec3 point_at_parameter(float t) const { return m_origin + t * m_direction; }

private:
	Vec3 m_origin{ 0.0f, 0.0f, 0.0f };
	Vec3 m_direction{ 0.0f, 0.0f, 1.0f };
	float m_time;
};