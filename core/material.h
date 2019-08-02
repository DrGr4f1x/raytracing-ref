#pragma once


class Ray;
class Texture;
struct HitRecord;
struct Vec3;


class Material
{
public:
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
	virtual Vec3 emitted(float u, float v, const Vec3& p) const { return Vec3(0.0f, 0.0f, 0.0f); }
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


class DiffuseLight : public Material
{
public:
	DiffuseLight(const Texture* tex)
		: m_emitter(tex)
	{}

	bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override
	{
		return false;
	}
	Vec3 emitted(float u, float v, const Vec3& p) const override;

private:
	const Texture* m_emitter{ nullptr };
};


class Isotropic : public Material
{
public:
	Isotropic(const Texture* tex)
		: m_albedo(tex)
	{}

	bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

private:
	const Texture* m_albedo{ nullptr };
};