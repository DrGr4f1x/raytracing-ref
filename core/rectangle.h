#pragma once

#include "hitable.h"


class Material;


class XYRect : public Hitable
{
public:
	XYRect() = default;
	XYRect(float x0, float x1, float y0, float y1, float k, const Material* mat)
		: m_x0(x0)
		, m_x1(x1)
		, m_y0(y0)
		, m_y1(y1)
		, m_k(k)
		, m_material(mat)
	{}

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool bounding_box(float t0, float t1, AABB& box) const override;

private:
	float m_x0{ 0.0f };
	float m_x1{ 1.0f };
	float m_y0{ 0.0f };
	float m_y1{ 1.0f };
	float m_k{ 0.0f };
	const Material* m_material{ nullptr };
};


class XZRect : public Hitable
{
public:
	XZRect() = default;
	XZRect(float x0, float x1, float z0, float z1, float k, const Material* mat)
		: m_x0(x0)
		, m_x1(x1)
		, m_z0(z0)
		, m_z1(z1)
		, m_k(k)
		, m_material(mat)
	{}

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool bounding_box(float t0, float t1, AABB& box) const override;

private:
	float m_x0{ 0.0f };
	float m_x1{ 1.0f };
	float m_z0{ 0.0f };
	float m_z1{ 1.0f };
	float m_k{ 0.0f };
	const Material* m_material{ nullptr };
};


class YZRect : public Hitable
{
public:
	YZRect() = default;
	YZRect(float y0, float y1, float z0, float z1, float k, const Material* mat)
		: m_y0(y0)
		, m_y1(y1)
		, m_z0(z0)
		, m_z1(z1)
		, m_k(k)
		, m_material(mat)
	{}

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool bounding_box(float t0, float t1, AABB& box) const override;

private:
	float m_y0{ 0.0f };
	float m_y1{ 1.0f };
	float m_z0{ 0.0f };
	float m_z1{ 1.0f };
	float m_k{ 0.0f };
	const Material* m_material{ nullptr };
};