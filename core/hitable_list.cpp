#include "stdafx.h"

#include "hitable_list.h"

#include "aabb.h"


bool HitableList::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	HitRecord temp_rec;
	bool hit_anything = false;

	float closest_so_far = t_max;

	for (int i = 0; i < m_listSize; ++i)
	{
		if (m_list[i]->hit(r, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}


bool HitableList::bounding_box(float t0, float t1, AABB& box) const
{
	if (m_listSize < 1)
		return false;

	AABB temp_box;
	bool first_true = m_list[0]->bounding_box(t0, t1, temp_box);

	if (!first_true)
		return false;
	else
		box = temp_box;

	for (int i = 1; i < m_listSize; ++i)
	{
		if (m_list[i]->bounding_box(t0, t1, temp_box))
		{
			box = surrounding_box(box, temp_box);
		}
		else
			return false;
	}
	return true;
}