#pragma once


class Ray;
struct HitRecord;
struct Vec3;


class Material
{
public:
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};