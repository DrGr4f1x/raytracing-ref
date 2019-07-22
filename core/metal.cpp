#include "stdafx.h"

#include "metal.h"

#include "hitable.h"
#include "rng.h"


bool Metal::scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const
{
	Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
	scattered = Ray(rec.p, reflected + m_fuzz * random_in_unit_sphere(), r_in.time());
	attenuation = m_albedo;
	return dot(scattered.direction(), rec.normal) > 0.0f;
}