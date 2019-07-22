#pragma once


#include "hitable.h"


class Material;


class MovingSphere : public Hitable
{
public:
	MovingSphere() = default;
	MovingSphere(const Vec3& center0, const Vec3 center1, float t0, float t1, float radius, const Material* material)
		: m_center0(center0)
		, m_center1(center1)
		, m_time0(t0)
		, m_time1(t1)
		, m_radius(radius)
		, m_material(material)
	{}

	Vec3 center(float time) const;

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool bounding_box(float t0, float t1, AABB& box) const override;

private:
	const Vec3 m_center0;
	const Vec3 m_center1;
	const float m_time0;
	const float m_time1;
	const float m_radius;
	const Material* m_material;
};