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


static float* perlinGenerate()
{
	float* p = new float[256];
	for (int i = 0; i < 256; ++i)
	{
		p[i] = g_RNG.NextFloat();
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


float* Perlin::s_ranfloat = perlinGenerate();
int* Perlin::s_permX = perlinGeneratePerm();
int* Perlin::s_permY = perlinGeneratePerm();
int* Perlin::s_permZ = perlinGeneratePerm();


float Perlin::noise(const Vec3& p) const 
{
	float u = p.x() - floorf(p.x());
	float v = p.y() - floorf(p.y());
	float w = p.z() - floorf(p.z());
	int i = int(4 * p.x()) & 255;
	int j = int(4 * p.y()) & 255;
	int k = int(4 * p.z()) & 255;

	return s_ranfloat[s_permX[i] ^ s_permY[j] ^ s_permZ[k]];
}