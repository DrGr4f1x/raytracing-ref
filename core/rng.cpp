#include "stdafx.h"

#include "rng.h"


RandomNumberGenerator g_RNG;


uint32_t RandomNumberGenerator::SetSeedPIDTime()
{
	uint32_t seed = 0;
	seed += GetCurrentProcessId();
	seed += static_cast<uint32_t>(std::time(nullptr));

	SetSeed(seed);

	return seed;
}


Vec3 random_in_unit_sphere()
{
	Vec3 p;
	do
	{
		p = 2.0f * Vec3(g_RNG.NextFloat(), g_RNG.NextFloat(), g_RNG.NextFloat()) - Vec3(1.0f, 1.0f, 1.0f);
	} while (p.squared_length() >= 1.0f);
	return p;
}


Vec3 random_in_unit_disc()
{
	Vec3 p;
	do 
	{
		p = 2.0f * Vec3(g_RNG.NextFloat(), g_RNG.NextFloat(), 0.0f) - Vec3(1.0f, 1.0f, 0.0f);
	} while (p.squared_length() >= 1.0f);
	return p;
}