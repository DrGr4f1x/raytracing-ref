#include "stdafx.h"

#include "bvh.h"

#include "rng.h"


int box_x_compare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->bounding_box(0.0f, 0.0f, box_left) || !bh->bounding_box(0.0f, 0.0f, box_right))
		std::cerr << "no bounding box in BVHNode constructor\n";

	if (box_left.min().x() - box_right.min().x() < 0.0f)
		return -1;
	else
		return 1;
}


int box_y_compare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->bounding_box(0.0f, 0.0f, box_left) || !bh->bounding_box(0.0f, 0.0f, box_right))
		std::cerr << "no bounding box in BVHNode constructor\n";

	if (box_left.min().y() - box_right.min().y() < 0.0f)
		return -1;
	else
		return 1;
}


int box_z_compare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->bounding_box(0.0f, 0.0f, box_left) || !bh->bounding_box(0.0f, 0.0f, box_right))
		std::cerr << "no bounding box in BVHNode constructor\n";

	if (box_left.min().z() - box_right.min().z() < 0.0f)
		return -1;
	else
		return 1;
}


BVHNode::BVHNode(Hitable** l, int n, float time0, float time1)
{
	int axis = int(3 * g_RNG.NextFloat());
	if (axis == 0)
		qsort(l, n, sizeof(Hitable*), box_x_compare);
	else if (axis == 1)
		qsort(l, n, sizeof(Hitable*), box_y_compare);
	else
		qsort(l, n, sizeof(Hitable*), box_z_compare);

	if (n == 1)
	{
		m_left = m_right = l[0];
	}
	else
	{
		m_left = new BVHNode(l, n / 2, time0, time1);
		m_right = new BVHNode(l + n / 2, n - n / 2, time0, time1);
	}

	AABB box_left, box_right;
	if (!m_left->bounding_box(time0, time1, box_left) || !m_right->bounding_box(time0, time1, box_right))
		std::cerr << "no bounding box in BVHNode constructor\n";
	m_aabb = surrounding_box(box_left, box_right);
}


bool BVHNode::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	if (m_aabb.hit(r, t_min, t_max))
	{
		HitRecord left_rec, right_rec;
		bool hit_left = m_left->hit(r, t_min, t_max, left_rec);
		bool hit_right = m_right->hit(r, t_min, t_max, right_rec);
		if (hit_left && hit_right)
		{
			if (left_rec.t < right_rec.t)
				rec = left_rec;
			else
				rec = right_rec;
			return true;
		}
		else if (hit_left)
		{
			rec = left_rec;
			return true;
		}
		else if (hit_right)
		{
			rec = right_rec;
			return true;
		}
	}

	return false;
}


bool BVHNode::bounding_box(float t0, float t1, AABB& b) const
{
	b = m_aabb;
	return true;
}