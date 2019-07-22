#include "stdafx.h"

#include "sphere.h"

#include "aabb.h"


bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	Vec3 oc = r.origin() - m_center;
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
			rec.normal = (rec.p - m_center) / m_radius;
			rec.mat = m_material;
			return true;
		}
		temp = (-b + sqrtf(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - m_center) / m_radius;
			rec.mat = m_material;
			return true;
		}
	}
	return false;
}


bool Sphere::bounding_box(float t0, float t1, AABB& box) const
{
	const Vec3 vec_rad{ m_radius, m_radius, m_radius };
	box = AABB(m_center - vec_rad, m_center + vec_rad);
	return true;
}