#pragma once

class Perlin
{
public:
	float noise(const Vec3& p) const;

	static float* s_ranfloat;
	static int* s_permX;
	static int* s_permY;
	static int* s_permZ;
};