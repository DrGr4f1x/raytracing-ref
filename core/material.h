#pragma once


class Ray;
class Texture;
struct HitRecord;
struct Vec3;


class Material
{
public:
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};


class Lambertian : public Material
{
public:
	Lambertian(const Texture* albedo) : m_albedo(albedo) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

private:
	const Texture* m_albedo;
};


class Metal : public Material
{
public:
	Metal(const Vec3& albedo, float f)
		: m_albedo(albedo)
	{
		m_fuzz = f < 1.0f ? f : 1.0f;
	}

	bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

private:
	Vec3 m_albedo;
	float m_fuzz;
};


class Dielectric : public Material
{
public:
	Dielectric(float refractionIndex)
		: m_refractionIndex(refractionIndex)
	{}

	bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

private:
	float m_refractionIndex;
};