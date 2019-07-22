#pragma once

#include "material.h"


class Metal : public Material
{
public:
	Metal(const Vec3& albedo, float f)
		: m_albedo(albedo)
	{
		m_fuzz = f < 1.0f ? f : 1.0f;
	}

	bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

private:
	Vec3 m_albedo;
	float m_fuzz;
};