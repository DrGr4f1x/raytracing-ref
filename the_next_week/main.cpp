#include "stdafx.h"

#include "box.h"
#include "bvh.h"
#include "camera.h"
#include "hitable_list.h"
#include "image.h"
#include "instance.h"
#include "material.h"
#include "medium.h"
#include "moving_sphere.h"
#include "ray.h"
#include "rectangle.h"
#include "rng.h"
#include "sphere.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace std;


// Image parameters
constexpr int IMAGE_WIDTH = 1280;
constexpr int IMAGE_HEIGHT = 720;
constexpr int NUM_SAMPLES = 64;
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
		Vec3 emitted = rec.mat->emitted(rec.u, rec.v, rec.p);
		if (depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered))
		{
			return emitted + attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return emitted;
		}
	}

	/*Vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);*/
	return Vec3(0.0f, 0.0f, 0.0f);
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
	Texture* pertext = new NoiseTexture(0.1f);
	int n = 50;
	Hitable** list = new Hitable*[n + 1];
	list[0] = new Sphere(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(pertext));
	list[1] = new Sphere(Vec3(0.0f, 2.0f, 0.0f), 2.0f, new Lambertian(pertext));
	return new HitableList(list, 2);
}


Hitable* simple_light()
{
	Texture* pertext = new NoiseTexture(4.0f);
	Hitable** list = new Hitable*[4];
	list[0] = new Sphere(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(pertext));
	list[1] = new Sphere(Vec3(0.0f, 2.0f, 0.0f), 2.0f, new Lambertian(pertext));
	list[2] = new Sphere(Vec3(0.0f, 7.0f, 0.0f), 2.0f, new DiffuseLight(new ConstantTexture(Vec3(4.0f, 4.0f, 4.0f))));
	list[3] = new XYRect(3.0f, 5.0f, 1.0f, 3.0f, -2.0f, new DiffuseLight(new ConstantTexture(Vec3(4.0f, 4.0f, 4.0f))));
	return new HitableList(list, 4);
}


Hitable* cornell_box_basic()
{
	Hitable** list = new Hitable*[6];
	int i = 0;
	Material* red = new Lambertian(new ConstantTexture(Vec3(0.65f, 0.05f, 0.05f)));
	Material* white = new Lambertian(new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
	Material* green = new Lambertian(new ConstantTexture(Vec3(0.12f, 0.45f, 0.15f)));
	Material* light =new DiffuseLight(new ConstantTexture(Vec3(15.0f, 15.0f, 15.0f)));
	list[i++] = new FlipNormals(new YZRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green));
	list[i++] = new YZRect(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red);
	list[i++] = new XZRect(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light);
	list[i++] = new FlipNormals(new XZRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	list[i++] = new XZRect(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white);
	list[i++] = new FlipNormals(new XYRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	return new HitableList(list, i);
}


Hitable* cornell_box_blocks()
{
	Hitable** list = new Hitable*[8];
	int i = 0;
	Material* red = new Lambertian(new ConstantTexture(Vec3(0.65f, 0.05f, 0.05f)));
	Material* white = new Lambertian(new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
	Material* green = new Lambertian(new ConstantTexture(Vec3(0.12f, 0.45f, 0.15f)));
	Material* light = new DiffuseLight(new ConstantTexture(Vec3(15.0f, 15.0f, 15.0f)));
	list[i++] = new FlipNormals(new YZRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green));
	list[i++] = new YZRect(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red);
	list[i++] = new XZRect(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light);
	list[i++] = new FlipNormals(new XZRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	list[i++] = new XZRect(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white);
	list[i++] = new FlipNormals(new XYRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	list[i++] = new Translate(new RotateY(new Box(Vec3(0.0f, 0.0f, 0.0f), Vec3(165.0f, 165.0f, 165.0f), white), -18.0f), Vec3(130.0f, 0.0f, 65.0f));
	list[i++] = new Translate(new RotateY(new Box(Vec3(0.0f, 0.0f, 0.0f), Vec3(165.0f, 330.0f, 165.0f), white), 15.0f), Vec3(265.0f, 0.0f, 295.0f));
	return new HitableList(list, i);
}


Hitable* cornell_box_smoke()
{
	Hitable** list = new Hitable*[8];
	int i = 0;
	Material* red = new Lambertian(new ConstantTexture(Vec3(0.65f, 0.05f, 0.05f)));
	Material* white = new Lambertian(new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
	Material* green = new Lambertian(new ConstantTexture(Vec3(0.12f, 0.45f, 0.15f)));
	Material* light = new DiffuseLight(new ConstantTexture(Vec3(7.0f, 7.0f, 7.0f)));
	list[i++] = new FlipNormals(new YZRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green));
	list[i++] = new YZRect(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red);
	list[i++] = new XZRect(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light);
	list[i++] = new FlipNormals(new XZRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	list[i++] = new XZRect(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white);
	list[i++] = new FlipNormals(new XYRect(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
	Hitable* b1 = new Translate(new RotateY(new Box(Vec3(0.0f, 0.0f, 0.0f), Vec3(165.0f, 165.0f, 165.0f), white), -18.0f), Vec3(130.0f, 0.0f, 65.0f));
	Hitable* b2 = new Translate(new RotateY(new Box(Vec3(0.0f, 0.0f, 0.0f), Vec3(165.0f, 330.0f, 165.0f), white), 15.0f), Vec3(265.0f, 0.0f, 295.0f));
	list[i++] = new ConstantMedium(b1, 0.01f, new ConstantTexture(Vec3(1.0f, 1.0f, 1.0f)));
	list[i++] = new ConstantMedium(b2, 0.01f, new ConstantTexture(Vec3(0.0f, 0.0f, 0.0f)));
	return new HitableList(list, i);
}


Hitable* final_scene()
{
	int nb = 20;
	Hitable** list = new Hitable*[30];
	Hitable** boxlist = new Hitable*[10000];
	Hitable** boxlist2 = new Hitable*[10000];
	Material* white = new Lambertian(new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
	Material* ground = new Lambertian(new ConstantTexture(Vec3(0.48f, 0.83f, 0.53f)));
	int b = 0;
	for (int i = 0; i < nb; ++i)
	{
		for (int j = 0; j < nb; ++j)
		{
			float w = 100.0f;
			float x0 = -1000.0f + i * w;
			float z0 = -1000.0f + j * w;
			float y0 = 0.0f;
			float x1 = x0 + w;
			float y1 = 100.0f * (g_RNG.NextFloat() + 0.01f);
			float z1 = z0 + w;
			boxlist[b++] = new Box(Vec3(x0, y0, z0), Vec3(x1, y1, z1), ground);
		}
	}

	int l = 0;
	list[l++] = new BVHNode(boxlist, b, 0.0f, 1.0f);
	Material* light = new DiffuseLight(new ConstantTexture(Vec3(7.0f, 7.0f, 7.0f)));
	list[l++] = new XZRect(123.0f, 423.0f, 147.0f, 412.0f, 554.0f, light);
	Vec3 center(400.0f, 400.0f, 200.0f);
	list[l++] = new MovingSphere(center, center + Vec3(30.0f, 0.0f, 0.0f), 0.0f, 1.0f, 50.0f, new Lambertian(new ConstantTexture(Vec3(0.7f, 0.3f, 0.1f))));
	list[l++] = new Sphere(Vec3(260.0f, 150.0f, 45.0f), 50.0f, new Dielectric(1.5f));
	list[l++] = new Sphere(Vec3(0.0f, 150.0f, 145.0f), 50.0f, new Metal(Vec3(0.8f, 0.8f, 0.9f), 10.0f));
	Hitable* boundary = new Sphere(Vec3(360.0f, 150.0f, 145.0f), 70.0f, new Dielectric(1.5f));
	list[l++] = boundary;
	list[l++] = new ConstantMedium(boundary, 0.2f, new ConstantTexture(Vec3(0.2f, 0.4f, 0.9f)));
	boundary = new Sphere(Vec3(0.0f, 0.0f, 0.0f), 5000.0f, new Dielectric(1.5f));
	list[l++] = new ConstantMedium(boundary, 0.0001f, new ConstantTexture(Vec3(1.0f, 1.0f, 1.0f)));

	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	Material *emat = new Lambertian(new ImageTexture(tex_data, nx, ny));
	list[l++] = new Sphere(Vec3(400, 200, 400), 100, emat);

	Texture* pertext = new NoiseTexture(0.1f);
	list[l++] = new Sphere(Vec3(220.0f, 280.0f, 300.0f), 80.0f, new Lambertian(pertext));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxlist2[j] = new Sphere(Vec3(165.0f * g_RNG.NextFloat(), 165.0f * g_RNG.NextFloat(), 165.0f * g_RNG.NextFloat()), 10.0f, white);
	}
	list[l++] = new Translate(new RotateY(new BVHNode(boxlist2, ns, 0.0f, 1.0f), 15.0f), Vec3(-100.0f, 270.0f, 395.0f));
	return new HitableList(list, l);
}

inline float clamp(float val, float min, float max)
{
	val = val < min ? min : val;
	val = val > max ? max : val;
	return val;
}


int main()
{
	Timer timer;
	timer.Start();

	Image image(IMAGE_WIDTH, IMAGE_HEIGHT);

	float time0 = 0.0f;
	float time1 = 1.0f;

	//Hitable* world = random_scene(time0, time1);
	//Hitable* world = two_perlin_spheres();
	//Hitable* world = simple_light();
	//Hitable* world = cornell_box_basic();
	//Hitable* world = cornell_box_blocks();
	//Hitable* world = cornell_box_smoke();
	Hitable* world = final_scene();

	//Vec3 origin{ 13.0f, 2.0f, 3.0f };
	//Vec3 origin{ 3.0f, 6.0f, 3.0f };
	Vec3 origin{ 278.0f, 278.0f, -800.0f };
	Vec3 target{ 278.0f, 278.0f, 0.0f };
	//Vec3 target{ 0.0f, 4.0f, 0.0f };
	Vec3 up{ 0.0f, 1.0f, 0.0f };
	//float dist_to_focus = (origin - target).length();
	//float aperture = 0.1f;
	float dist_to_focus = 10.0f;
	float aperture = 0.0f;

	Camera cam(
		origin,
		target,
		up,
		40.0f,
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
			col = Vec3(clamp(sqrtf(col.x()), 0.0f, 1.0f), clamp(sqrtf(col.y()), 0.0f, 1.0f), clamp(sqrtf(col.z()), 0.0f, 1.0f));

			image.SetPixel(i, j, col);
		}
	}

	timer.Stop();
	double rayCastSeconds = timer.GetElapsedSeconds();

	image.SaveAs("image.ppm");

	// Calculate stats
	size_t primaryRays = size_t(IMAGE_WIDTH) * size_t(IMAGE_HEIGHT) * size_t(NUM_SAMPLES);
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