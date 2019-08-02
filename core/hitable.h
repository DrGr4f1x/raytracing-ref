#pragma once

#include "ray.h"


class AABB;
class Material;


struct HitRecord
{
	float u;
	float v;
	float t;
	Vec3 p;
	Vec3 normal;
	const Material* mat;
};


class Hitable
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, AABB& box) const = 0;
};


class FlipNormals : public Hitable
{
public:
	FlipNormals(const Hitable* hitable)
		: m_hitable(hitable)
	{}

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
	{
		if (m_hitable->hit(r, t_min, t_max, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		return false;
	}

	bool bounding_box(float t0, float t1, AABB& box) const override
	{
		return m_hitable->bounding_box(t0, t1, box);
	}

private:
	const Hitable* m_hitable;
};