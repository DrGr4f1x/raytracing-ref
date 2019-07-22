#include "stdafx.h"

#include "camera.h"

#include "rng.h"


Camera::Camera(const Vec3& origin, const Vec3& target, const Vec3& up, float fovy, float aspect, float aperture, float focusDist, float t0, float t1)
	: m_fovy(fovy)
	, m_aspect(aspect)
	, m_aperture(aperture)
	, m_focusDist(focusDist)
	, m_time0(t0)
	, m_time1(t1)
{
	m_lensRadius = 0.5f * m_aperture;
	float theta = m_fovy * float(M_PI) / 180.0f;
	float half_height = tanf(theta * 0.5f);
	float half_width = aspect * half_height;
	m_origin = origin;
	m_w = unit_vector(origin - target);
	m_u = unit_vector(cross(up, m_w));
	m_v = cross(m_w, m_u);
	m_lowerLeftCorner = origin - half_width * m_focusDist * m_u - half_height * m_focusDist * m_v - m_focusDist * m_w;
	m_horizontal = 2.0f * half_width * m_focusDist *m_u;
	m_vertical = 2.0f * half_height * m_focusDist * m_v;
}


Ray Camera::get_ray(float u, float v) const
{
	Vec3 rd = m_lensRadius * random_in_unit_disc();
	Vec3 offset = m_u * rd.x() + m_v * rd.y();
	float time = m_time0 + g_RNG.NextFloat() * (m_time1 - m_time0);
	return Ray(m_origin + offset, m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin - offset, time);
}