#include "stdafx.h"

#include "medium.h"


#include "material.h"
#include "rng.h"
#include "texture.h"


ConstantMedium::ConstantMedium(const Hitable* hitable, float d, const Texture* tex)
	: m_boundary(hitable)
	, m_density(d)
	, m_phaseFunction(new Isotropic(tex))
{}


bool ConstantMedium::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	bool db = g_RNG.NextFloat() < 0.00001f;
	db = false;
	HitRecord rec1, rec2;
	if (m_boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) 
	{
		if (m_boundary->hit(r, rec1.t + 0.0001f, FLT_MAX, rec2))
		{
			if (rec1.t < t_min)
				rec1.t = t_min;
			if (rec2.t > t_max)
				rec2.t = t_max;
			if (rec1.t >= rec2.t)
				return false;
			if (rec1.t < 0.0f)
				rec1.t = 0.0f;

			float distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
			float hit_distance = -(1.0f / m_density) * logf(g_RNG.NextFloat());
			if (hit_distance < distance_inside_boundary)
			{
				rec.t = rec1.t + hit_distance / r.direction().length();
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = Vec3(1.0f, 0.0f, 0.0f); // arbitrary
				rec.mat = m_phaseFunction;
				return true;
			}
		}
	}
	return false;
}


bool ConstantMedium::bounding_box(float t0, float t1, AABB& box) const
{
	return m_boundary->bounding_box(t0, t1, box);
}