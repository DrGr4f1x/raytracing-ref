#pragma once

#include "material.h"

class Dielectric : public Material
{
public:
	Dielectric(float refractionIndex)
		: m_refractionIndex(refractionIndex)
	{}

	bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

private:
	float m_refractionIndex;
};