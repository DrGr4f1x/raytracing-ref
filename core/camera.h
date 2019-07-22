#pragma once

#include "ray.h"


class Camera
{
public:
	Camera(const Vec3& origin, const Vec3& target, const Vec3& up, float fovy, float aspect, float aperture, float focusDist, float t0, float t1);

	Ray get_ray(float u, float v) const;

private:
	float m_fovy;
	float m_aspect;
	float m_aperture;
	float m_focusDist;

	Vec3 m_origin;
	Vec3 m_lowerLeftCorner;
	Vec3 m_horizontal;
	Vec3 m_vertical;

	Vec3 m_u;
	Vec3 m_v;
	Vec3 m_w;

	float m_time0;
	float m_time1;
	float m_lensRadius;
};