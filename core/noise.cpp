#include "stdafx.h"

#include "noise.h"

#include "rng.h"


void permute(int* p, int n)
{
	for (int i = n - 1; i > 0; --i)
	{
		int target = int(g_RNG.NextFloat() * (i + 1));
		int temp = p[i];
		p[i] = p[target];
		p[target] = temp;
	}
}


static Vec3* perlinGenerate()
{
	Vec3* p = new Vec3[256];
	for (int i = 0; i < 256; ++i)
	{
		p[i] = unit_vector(Vec3(-1.0f + 2 * g_RNG.NextFloat(), -1.0f + 2.0f * g_RNG.NextFloat(), -1.0f + 2.0f * g_RNG.NextFloat()));
	}

	return p;
}


static int* perlinGeneratePerm()
{
	int* p = new int[256];
	for (int i = 0; i < 256; ++i)
	{
		p[i] = i;
	}
	permute(p, 256);
	return p;
}


Vec3* Perlin::s_ranvec = perlinGenerate();
int* Perlin::s_permX = perlinGeneratePerm();
int* Perlin::s_permY = perlinGeneratePerm();
int* Perlin::s_permZ = perlinGeneratePerm();


inline float trilinear_interp(float c[2][2][2], float u, float v, float w)
{
	float accum = 0.0f;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				accum +=
					(i * u + (1 - i) * (1.0f - u)) *
					(j * v + (1 - j) * (1.0f - v)) *
					(k * w + (1 - k) * (1.0f - w)) * c[i][j][k];
			}
		}
	}
	return accum;
}


inline float hermite(float u)
{
	return u * u * (3.0f - 2.0f * u);
}


inline float perlin_interp(Vec3 c[2][2][2], float u, float v, float w)
{
	float uu = hermite(u);
	float vv = hermite(v);
	float ww = hermite(w);

	float accum = 0.0f;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				Vec3 weight_v{ u - float(i), v - float(j), w - float(k) };
				accum +=
					(i * uu + (1 - i) * (1.0f - uu)) *
					(j * vv + (1 - j) * (1.0f - vv)) *
					(k * ww + (1 - k) * (1.0f - ww)) * dot(c[i][j][k], weight_v);
			}
		}
	}
	return accum;
}


float Perlin::noise(const Vec3& p) const 
{
	float u = p.x() - floorf(p.x());
	float v = p.y() - floorf(p.y());
	float w = p.z() - floorf(p.z());
	int i = int(floorf(p.x()));
	int j = int(floorf(p.y()));
	int k = int(floorf(p.z()));

	Vec3 c[2][2][2];
	for (int di = 0; di < 2; ++di)
	{
		for (int dj = 0; dj < 2; ++dj)
		{
			for (int dk = 0; dk < 2; ++dk)
			{
				c[di][dj][dk] = s_ranvec[s_permX[(i + di) & 255] ^ s_permY[(j + dj) & 255] ^ s_permZ[(k + dk) & 255]];
			}
		}
	}

	return perlin_interp(c, u, v, w);
}


float Perlin::turb(const Vec3& p, int depth) const
{
	float accum = 0.0f;
	Vec3 temp_p = p;
	float weight = 1.0f;
	for (int i = 0; i < depth; ++i)
	{
		accum += weight * noise(temp_p);
		weight *= 0.5f;
		temp_p *= 2.0f;
	}

	return fabsf(accum);
}