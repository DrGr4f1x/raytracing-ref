#pragma once


class Ray;


class AABB
{
public:
	AABB() = default;
	AABB(const Vec3& min, const Vec3& max)
		: m_min(min)
		, m_max(max)
	{}

	Vec3 min() const { return m_min; }
	Vec3 max() const { return m_max; }

	bool hit(const Ray& r, float t_min, float t_max) const;

private:
	Vec3 m_min;
	Vec3 m_max;
};


AABB surrounding_box(const AABB& box0, const AABB& box1);