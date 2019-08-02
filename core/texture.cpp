#include "stdafx.h"

#include "texture.h"


Vec3 CheckerTexture::value(float u, float v, const Vec3& p) const
{
	float sines = sinf(10.0f * p.x()) * sinf(10.0f * p.y()) * sinf(10.0f * p.z());
	
	return sines < 0.0f ? m_oddTex->value(u, v, p) : m_evenTex->value(u, v, p);
}


Vec3 NoiseTexture::value(float u, float v, const Vec3& p) const
{
	//return Vec3(1.0f, 1.0f, 1.0f) * m_noise.noise(m_scale * p);
	return Vec3(1.0f, 1.0f, 1.0f) * 0.5f * (1.0f + sinf(m_scale * p.z() + 10.0f * m_noise.turb(p)));
}


Vec3 ImageTexture::value(float u, float v, const Vec3& p) const
{
	int i = u * m_nx;
	int j = (1.0f - v) * m_ny - 0.001f;
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > m_nx - 1) i = m_nx - 1;
	if (j > m_ny - 1) j = m_ny - 1;
	float r = int(m_data[3 * i + 3 * m_nx * j]) / 255.0f;
	float g = int(m_data[3 * i + 3 * m_nx * j + 1]) / 255.0f;
	float b = int(m_data[3 * i + 3 * m_nx * j + 2]) / 255.0f;
	return Vec3(r, g, b);
}