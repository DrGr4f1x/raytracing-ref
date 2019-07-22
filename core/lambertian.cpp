#include "stdafx.h"

#include "lambertian.h"

#include "hitable.h"
#include "rng.h"


bool Lambertian::scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const
{
	Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
	scattered = Ray(rec.p, target - rec.p, r_in.time());
	attenuation = m_albedo;

	return true;
}