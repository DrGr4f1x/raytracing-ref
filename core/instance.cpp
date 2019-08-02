#include "stdafx.h"

#include "instance.h"


bool Translate::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	Ray moved_r(r.origin() - m_offset, r.direction(), r.time());
	if (m_hitable->hit(moved_r, t_min, t_max, rec))
	{
		rec.p += m_offset;
		return true;
	}
	return false;
}


bool Translate::bounding_box(float t0, float t1, AABB& box) const
{
	if (m_hitable->bounding_box(t0, t1, box))
	{
		box = AABB(box.min() + m_offset, box.max() + m_offset);
		return true;
	}
	return false;
}


RotateY::RotateY(const Hitable* hitable, float angle)
	: m_hitable(hitable)
{
	float radians = (float(M_PI) / 180.0f) * angle;
	m_sinTheta = sinf(radians);
	m_cosTheta = cosf(radians);
	m_hasBox = m_hitable->bounding_box(0.0f, 1.0f, m_bbox);
	Vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	Vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				float x = i * m_bbox.max().x() + (1 - i) * m_bbox.min().x();
				float y = j * m_bbox.max().y() + (1 - j) * m_bbox.min().y();
				float z = k * m_bbox.max().z() + (1 - k) * m_bbox.min().z();
				float newx = m_cosTheta * x + m_sinTheta * z;
				float newz = -m_sinTheta * x + m_cosTheta * z;

				Vec3 tester(newx, y, newz);
				for (int c = 0; c < 3; ++c)
				{
					if (tester[c] > max[c])
						max[c] = tester[c];
					if (tester[c] < min[c])
						min[c] = tester[c];
				}
			}
		}
	}
	m_bbox = AABB(min, max);
}


bool RotateY::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	Vec3 origin = r.origin();
	Vec3 direction = r.direction();
	origin[0] = m_cosTheta * r.origin()[0] - m_sinTheta * r.origin()[2];
	origin[2] = m_sinTheta * r.origin()[0] + m_cosTheta * r.origin()[2];
	direction[0] = m_cosTheta * r.direction()[0] - m_sinTheta * r.direction()[2];
	direction[2] = m_sinTheta * r.direction()[0] + m_cosTheta * r.direction()[2];
	Ray rotated_r(origin, direction, r.time());
	if (m_hitable->hit(rotated_r, t_min, t_max, rec))
	{
		Vec3 p = rec.p;
		Vec3 normal = rec.normal;
		p[0] = m_cosTheta * rec.p[0] + m_sinTheta * rec.p[2];
		p[2] = -m_sinTheta * rec.p[0] + m_cosTheta * rec.p[2];
		normal[0] = m_cosTheta * rec.normal[0] + m_sinTheta * rec.normal[2];
		normal[2] = -m_sinTheta * rec.normal[0] + m_cosTheta * rec.normal[2];
		rec.p = p;
		rec.normal = normal;
		return true;
	}
	return false;
}


bool RotateY::bounding_box(float t0, float t1, AABB& box) const
{
	box = m_bbox;
	return m_hasBox;
}