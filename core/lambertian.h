#pragma once

#include "material.h"


class Lambertian : public Material
{
public:
	Lambertian(const Vec3& albedo) : m_albedo(albedo) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

private:
	Vec3 m_albedo;
};