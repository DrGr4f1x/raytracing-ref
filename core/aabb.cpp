#include "stdafx.h"

#include "aabb.h"

#include "ray.h"


bool AABB::hit(const Ray& r, float t_min, float t_max) const 
{
	for (int a = 0; a < 3; ++a)
	{
		float invD = 1.0f / r.direction()[a];
		float t0 = (min()[a] - r.origin()[a]) * invD;
		float t1 = (max()[a] - r.origin()[a]) * invD;
		if (invD < 0.0f)
			std::swap(t0, t1);
		t_min = t0 > t_min ? t0 : t_min;
		t_max = t1 < t_max ? t1 : t_max;
		if (t_max <= t_min)
			return false;
	}

	return true;
}


inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }


AABB surrounding_box(const AABB& box0, const AABB& box1)
{
	Vec3 small(
		ffmin(box0.min().x(), box1.min().x()),
		ffmin(box0.min().y(), box1.min().y()),
		ffmin(box0.min().z(), box1.min().z()));

	Vec3 big(
		ffmax(box0.max().x(), box1.max().x()),
		ffmax(box0.max().y(), box1.max().y()),
		ffmax(box0.max().z(), box1.max().z()));

	return AABB(small, big);
}