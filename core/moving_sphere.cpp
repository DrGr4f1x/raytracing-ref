#include "stdafx.h"

#include "moving_sphere.h"

#include "aabb.h"


Vec3 MovingSphere::center(float time) const
{
	return m_center0 + ((time - m_time0) / (m_time1 - m_time0)) * (m_center1 - m_center0);
}


bool MovingSphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	Vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - m_radius * m_radius;
	float discriminant = b * b - a * c;

	if (discriminant > 0.0f)
	{
		float temp = (-b - sqrtf(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / m_radius;
			rec.mat = m_material;
			return true;
		}
		temp = (-b + sqrtf(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / m_radius;
			rec.mat = m_material;
			return true;
		}
	}
	return false;
}


bool MovingSphere::bounding_box(float t0, float t1, AABB& box) const
{
	const Vec3 vec_rad{ m_radius, m_radius, m_radius };

	AABB box0 = AABB(center(t0) - vec_rad, center(t0) + vec_rad);
	AABB box1 = AABB(center(t1) - vec_rad, center(t1) + vec_rad);

	box = surrounding_box(box0, box1);

	return true;
}