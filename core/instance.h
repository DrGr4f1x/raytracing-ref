#pragma once

#include "hitable.h"

#include "aabb.h"


class Translate : public Hitable
{
public:
	Translate(const Hitable* hitable, const Vec3& offset)
		: m_hitable(hitable)
		, m_offset(offset)
	{}

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool bounding_box(float t0, float t1, AABB& box) const override;

private:
	const Hitable* m_hitable{ nullptr };
	const Vec3 m_offset;
};


class RotateY : public Hitable
{
public:
	RotateY(const Hitable* hitable, float angle);

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool bounding_box(float t0, float t1, AABB& box) const override;

private:
	const Hitable* m_hitable{ nullptr };
	float m_sinTheta;
	float m_cosTheta;
	bool m_hasBox;
	AABB m_bbox;
};