#pragma once

#include "hitable.h"


class Material;


class Box : public Hitable 
{
public:
	Box() = default;
	Box(const Vec3& min, const Vec3& max, const Material* mat);

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool bounding_box(float t0, float t1, AABB& box) const override;

private:
	Vec3 m_min{ 0.0f, 0.0f, 0.0f };
	Vec3 m_max{ 1.0f, 1.0f, 1.0f };
	const Material* m_material{ nullptr };
	Hitable* m_list{ nullptr };
};