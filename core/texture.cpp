#include "stdafx.h"

#include "texture.h"


Vec3 CheckerTexture::value(float u, float v, const Vec3& p) const
{
	float sines = sinf(10.0f * p.x()) * sinf(10.0f * p.y()) * sinf(10.0f * p.z());
	
	return sines < 0.0f ? m_oddTex->value(u, v, p) : m_evenTex->value(u, v, p);
}


Vec3 NoiseTexture::value(float u, float v, const Vec3& p) const
{
	return Vec3(1.0f, 1.0f, 1.0f) * m_noise.noise(p);
}