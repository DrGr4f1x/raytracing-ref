#pragma once

#include "hitable.h"

#include "aabb.h"


class BVHNode : public Hitable
{
public:
	BVHNode() = default;
	BVHNode(Hitable** l, int n, float time0, float time1);

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool bounding_box(float t0, float t1, AABB& box) const override;

private:
	Hitable* m_left{ nullptr };
	Hitable* m_right{ nullptr };
	AABB m_aabb;
};