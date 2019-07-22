#pragma once

#include "hitable.h"


class Material;


class Sphere : public Hitable
{
public:
	Sphere() = default;
	Sphere(const Vec3& center, float radius, const Material* material)
		: m_center(center)
		, m_radius(radius)
		, m_material(material)
	{}

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

private:
	const Vec3 m_center;
	const float m_radius;
	const Material* m_material;
};