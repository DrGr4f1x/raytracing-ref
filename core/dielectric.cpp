#include "stdafx.h"

#include "dielectric.h"

#include "hitable.h"
#include "rng.h"


float schlick(float cosine, float refractionIndex)
{
	float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * powf((1.0f - cosine), 5.0f);
}


bool Dielectric::scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const
{
	Vec3 outward_normal;
	Vec3 reflected = reflect(r_in.direction(), rec.normal);
	float ni_over_nt;
	attenuation = Vec3(1.0f, 1.0f, 1.0f);
	Vec3 refracted;
	float reflect_prob;
	float cosine;

	if (dot(r_in.direction(), rec.normal) > 0.0f)
	{
		outward_normal = -rec.normal;
		ni_over_nt = m_refractionIndex;
		cosine = m_refractionIndex * dot(r_in.direction(), rec.normal) / r_in.direction().length();
	}
	else
	{
		outward_normal = rec.normal;
		ni_over_nt = 1.0f / m_refractionIndex;
		cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
	}

	if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
	{
		reflect_prob = schlick(cosine, m_refractionIndex);
	}
	else
	{
		reflect_prob = 1.0f;
	}

	if (g_RNG.NextFloat() < reflect_prob)
	{
		scattered = Ray(rec.p, reflected, r_in.time());
	}
	else
	{
		scattered = Ray(rec.p, refracted, r_in.time());
	}

	return true;
}