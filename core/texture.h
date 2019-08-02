#pragma once

#include "noise.h"


class Texture
{
public:
	virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
};


class ConstantTexture : public Texture
{
public:
	ConstantTexture(const Vec3& c)
		: m_color(c)
	{}

	Vec3 value(float u, float v, const Vec3& p) const override
	{
		return m_color;
	}

private:
	const Vec3 m_color;
};


class CheckerTexture : public Texture
{
public:
	CheckerTexture(const Texture* evenTex, const Texture* oddTex)
		: m_evenTex(evenTex)
		, m_oddTex(oddTex)
	{}
	
	Vec3 value(float u, float v, const Vec3& p) const override;

private:
	const Texture* m_evenTex{ nullptr };
	const Texture* m_oddTex{ nullptr };
};


class NoiseTexture : public Texture
{
public:
	NoiseTexture() = default;
	NoiseTexture(float s)
		: m_scale(s)
	{}

	Vec3 value(float u, float v, const Vec3& p) const override;

private:
	Perlin m_noise;
	float m_scale{ 1.0f };
};


class ImageTexture : public Texture
{
public:
	ImageTexture() = default;
	ImageTexture(unsigned char* pixels, int A, int B)
		: m_data(pixels)
		, m_nx(A)
		, m_ny(B)
	{}

	Vec3 value(float u, float v, const Vec3& p) const override;

private:
	unsigned char* m_data{ nullptr };
	int m_nx{ 0 };
	int m_ny{ 0 };
};