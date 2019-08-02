#include "stdafx.h"

#include "box.h"

#include "aabb.h"
#include "hitable_list.h"
#include "rectangle.h"


Box::Box(const Vec3& min, const Vec3& max, const Material* mat)
	: m_min(min)
	, m_max(max)
	, m_material(mat)
{
	Hitable** list = new Hitable*[6];
	list[0] = new XYRect(min.x(), max.x(), min.y(), max.y(), max.z(), mat);
	list[1] = new FlipNormals(new XYRect(min.x(), max.x(), min.y(), max.y(), min.z(), mat));
	list[2] = new XZRect(min.x(), max.x(), min.z(), max.z(), max.y(), mat);
	list[3] = new FlipNormals(new XZRect(min.x(), max.x(), min.z(), max.z(), min.y(), mat));
	list[4] = new YZRect(min.y(), max.y(), min.z(), max.z(), max.x(), mat);
	list[5] = new FlipNormals(new YZRect(min.y(), max.y(), min.z(), max.z(), min.x(), mat));
	m_list = new HitableList(list, 6);
}


bool Box::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	return m_list->hit(r, t_min, t_max, rec);
}


bool Box::bounding_box(float t0, float t1, AABB& box) const
{
	box = AABB(m_min, m_max);
	return true;
}