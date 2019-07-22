#pragma once

#include "hitable.h"


class HitableList : public Hitable
{
public:
	HitableList() = default;
	HitableList(Hitable** l, int n)
		: m_list(l)
		, m_listSize(n)
	{}

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool bounding_box(float t0, float t1, AABB& box) const override;

private:
	Hitable** m_list{ nullptr };
	const int m_listSize{ 0 };
};