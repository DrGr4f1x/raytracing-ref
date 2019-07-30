#include "stdafx.h"

#include "bvh.h"
#include "camera.h"
#include "hitable_list.h"
#include "image.h"
#include "material.h"
#include "moving_sphere.h"
#include "ray.h"
#include "rng.h"
#include "sphere.h"
#include "texture.h"


using namespace std;


// Image parameters
constexpr int IMAGE_WIDTH = 1280;
constexpr int IMAGE_HEIGHT = 720;
constexpr int NUM_SAMPLES = 16;
constexpr float INV_SAMPLES = 1.0f / static_cast<float>(NUM_SAMPLES);
constexpr float ASPECT = static_cast<float>(IMAGE_WIDTH) / static_cast<float>(IMAGE_HEIGHT);

// Stats
atomic_size_t s_totalRays = 0;


Vec3 color(const Ray& r, Hitable* world, int depth)
{
	++s_totalRays;

	HitRecord rec;

	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return Vec3(0.0f, 0.0f, 0.0f);
		}
	}

	Vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}


Hitable* random_scene(float time0, float time1)
{
	int n = 500;
	Hitable** list = new Hitable*[n + 1];
	Texture * checker = new CheckerTexture(new ConstantTexture(Vec3(0.2f, 0.3f, 0.1f)), new ConstantTexture(Vec3(0.9f, 0.9f, 0.9f)));
	list[0] = new Sphere(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(checker));
	int i = 1;
	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			float choose_mat = g_RNG.NextFloat();
			Vec3 center{ float(a) + 0.9f * g_RNG.NextFloat(), 0.2f, float(b) + 0.9f * g_RNG.NextFloat() };
			if ((center - Vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
			{
				if (choose_mat < 0.8f)
				{
					list[i++] = new MovingSphere(center, center + Vec3(0.0f, 0.5f * g_RNG.NextFloat(), 0.0f), time0, time1, 0.2f, new Lambertian(new ConstantTexture(Vec3(g_RNG.NextFloat() * g_RNG.NextFloat(), g_RNG.NextFloat() * g_RNG.NextFloat(), g_RNG.NextFloat() * g_RNG.NextFloat()))));
				}
				else if (choose_mat < 0.95f)
				{
					list[i++] = new Sphere(center, 0.2f, new Metal(Vec3(0.5f * (1.0f + g_RNG.NextFloat()), 0.5f * (1.0f + g_RNG.NextFloat()), 0.5f * (1.0f + g_RNG.NextFloat())), 0.5f * g_RNG.NextFloat()));
				}
				else
				{
					list[i++] = new Sphere(center, 0.2f, new Dielectric(1.5f));
				}
			}
		}
	}

	list[i++] = new Sphere(Vec3(0.0f, 1.0f, 0.0f), 1.0f, new Dielectric(1.5f));
	list[i++] = new Sphere(Vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(new ConstantTexture(Vec3(0.4f, 0.2f, 0.1f))));
	list[i++] = new Sphere(Vec3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(Vec3(0.7f, 0.6f, 0.5f), 0.0f));

	return new BVHNode(list, i, time0, time1);
}


Hitable* two_spheres()
{
	Texture * checker = new CheckerTexture(new ConstantTexture(Vec3(0.2f, 0.3f, 0.1f)), new ConstantTexture(Vec3(0.9f, 0.9f, 0.9f)));
	int n = 50;
	Hitable** list = new Hitable*[n + 1];
	list[0] = new Sphere(Vec3(0.0f, -10.0f, 0.0f), 10.0f, new Lambertian(checker));
	list[1] = new Sphere(Vec3(0.0f, 10.0f, 0.0f), 10.0f, new Lambertian(checker));
	return new HitableList(list, 2);
}


Hitable* two_perlin_spheres()
{
	Texture* pertext = new NoiseTexture();
	int n = 50;
	Hitable** list = new Hitable*[n + 1];
	list[0] = new Sphere(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(pertext));
	list[1] = new Sphere(Vec3(0.0f, 2.0f, 0.0f), 2.0f, new Lambertian(pertext));
	return new HitableList(list, 2);
}


int main()
{
	Timer timer;
	timer.Start();

	Image image(IMAGE_WIDTH, IMAGE_HEIGHT);

	float time0 = 0.0f;
	float time1 = 1.0f;

	//Hitable* world = random_scene(time0, time1);
	Hitable* world = two_perlin_spheres();

	Vec3 origin{ 13.0f, 2.0f, 3.0f };
	Vec3 target{ 0.0f, 0.0f, 0.0f };
	Vec3 up{ 0.0f, 1.0f, 0.0f };
	//float dist_to_focus = (origin - target).length();
	//float aperture = 0.1f;
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;

	Camera cam(
		origin,
		target,
		up,
		20.0f,
		ASPECT,
		aperture,
		dist_to_focus,
		time0,
		time1);

	for (int j = IMAGE_HEIGHT - 1; j >= 0; --j)
	{
		for (int i = 0; i < IMAGE_WIDTH; ++i)
		{
			Vec3 col{ 0.0f, 0.0f, 0.0f };
			for (int s = 0; s < NUM_SAMPLES; ++s)
			{
				float u = (float(i) + g_RNG.NextFloat()) * image.GetInvWidth();
				float v = (float(j) + g_RNG.NextFloat()) * image.GetInvHeight();

				Ray r = cam.get_ray(u, v);

				col += color(r, world, 0);
			}

			col *= INV_SAMPLES;
			col = Vec3(sqrtf(col.x()), sqrtf(col.y()), sqrtf(col.z()));

			image.SetPixel(i, j, col);
		}
	}

	timer.Stop();
	double rayCastSeconds = timer.GetElapsedSeconds();

	image.SaveAs("image.ppm");

	// Calculate stats
	size_t primaryRays = IMAGE_WIDTH * IMAGE_HEIGHT * NUM_SAMPLES;
	double primaryRaysPerSecond = (double)primaryRays / rayCastSeconds;
	double totalRaysPerSecond = (double)s_totalRays / rayCastSeconds;

	// Log stats
	stringstream sstr;
	sstr.precision(12);
	sstr << "Ray cast time: " << rayCastSeconds << endl;
	sstr << "  Image size: " << IMAGE_WIDTH << " x " << IMAGE_HEIGHT << " (" << NUM_SAMPLES << " samples per pixel)" << endl;
	sstr << "  Primary rays per second: " << primaryRaysPerSecond << ", primary rays: " << primaryRays << endl;
	sstr << "  Total rays per second: " << totalRaysPerSecond << ", total rays " << s_totalRays << endl;
	sstr << endl;
	OutputDebugStringA(sstr.str().c_str());

	return 0;
}