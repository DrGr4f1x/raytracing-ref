#pragma once

#include "hitable.h"


class Material;
class Texture;


class ConstantMedium : public Hitable
{
public:
	ConstantMedium(const Hitable* hitable, float d, const Texture* tex);

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool bounding_box(float t0, float t1, AABB& box) const override;

private:
	const Hitable* m_boundary{ nullptr };
	float m_density;
	const Material* m_phaseFunction;
};