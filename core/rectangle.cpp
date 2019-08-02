#include "stdafx.h"

#include "rectangle.h"

#include "aabb.h"


bool XYRect::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	float t = (m_k - r.origin().z()) / r.direction().z();
	if (t < t_min || t > t_max)
		return false;
	float x = r.origin().x() + t * r.direction().x();
	float y = r.origin().y() + t * r.direction().y();
	if (x < m_x0 || x > m_x1 || y < m_y0 || y > m_y1)
		return false;
	rec.u = (x - m_x0) / (m_x1 - m_x0);
	rec.v = (y - m_y0) / (m_y1 - m_y0);
	rec.t = t;
	rec.mat = m_material;
	rec.p = r.point_at_parameter(t);
	rec.normal = Vec3(0.0f, 0.0f, 1.0f);
	return true;
}


bool XYRect::bounding_box(float t0, float t1, AABB& box) const
{
	box = AABB(Vec3(m_x0, m_y0, m_k - 0.0001f), Vec3(m_x1, m_y1, m_k + 0.0001f));
	return true;
}


bool XZRect::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	float t = (m_k - r.origin().y()) / r.direction().y();
	if (t < t_min || t > t_max)
		return false;
	float x = r.origin().x() + t * r.direction().x();
	float z = r.origin().z() + t * r.direction().z();
	if (x < m_x0 || x > m_x1 || z < m_z0 || z > m_z1)
		return false;
	rec.u = (x - m_x0) / (m_x1 - m_x0);
	rec.v = (z - m_z0) / (m_z1 - m_z0);
	rec.t = t;
	rec.mat = m_material;
	rec.p = r.point_at_parameter(t);
	rec.normal = Vec3(0.0f, 1.0f, 0.0f);
	return true;
}


bool XZRect::bounding_box(float t0, float t1, AABB& box) const
{
	box = AABB(Vec3(m_x0, m_k - 0.0001f, m_z0), Vec3(m_x1, m_k + 0.0001f, m_z1));
	return true;
}


bool YZRect::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	float t = (m_k - r.origin().x()) / r.direction().x();
	if (t < t_min || t > t_max)
		return false;
	float y = r.origin().y() + t * r.direction().y();
	float z = r.origin().z() + t * r.direction().z();
	if (y < m_y0 || y > m_y1 || z < m_z0 || z > m_z1)
		return false;
	rec.u = (y - m_y0) / (m_y1 - m_y0);
	rec.v = (z - m_z0) / (m_z1 - m_z0);
	rec.t = t;
	rec.mat = m_material;
	rec.p = r.point_at_parameter(t);
	rec.normal = Vec3(1.0f, 0.0f, 0.0f);
	return true;
}


bool YZRect::bounding_box(float t0, float t1, AABB& box) const
{
	box = AABB(Vec3(m_k - 0.0001f, m_y0, m_z0), Vec3(m_k + 0.0001f, m_y1, m_z1));
	return true;
}