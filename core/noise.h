#pragma once

class Perlin
{
public:
	float noise(const Vec3& p) const;
	float turb(const Vec3& p, int depth = 7) const;

private:
	static Vec3* s_ranvec;
	static int* s_permX;
	static int* s_permY;
	static int* s_permZ;
};