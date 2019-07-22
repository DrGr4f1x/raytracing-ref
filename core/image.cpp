#include "stdafx.h"

#include "image.h"

using namespace std;

Image::Image(int width, int height)
	: m_width(width)
	, m_height(height)
	, m_invWidth(0.0f)
	, m_invHeight(0.0f)
{
	Init();
}


Image::Image(Image && other)
	: m_width(other.m_width)
	, m_height(other.m_height)
	, m_invWidth(other.m_invWidth)
	, m_invHeight(other.m_invHeight)
	, m_imageData(move(other.m_imageData))
{}


void Image::SetPixel(int i, int j, Vec3 color)
{
	m_imageData[i + j * m_width] = color;
}


Vec3* Image::GetData()
{
	return m_imageData.get();
}


void Image::Init()
{
	m_invWidth = 1.0f / static_cast<float>(m_width);
	m_invHeight = 1.0f / static_cast<float>(m_height);

	m_imageData = make_unique<Vec3[]>(m_width * m_height);
}


void Image::SaveAs(const char* filename)
{
	ofstream outfile;
	outfile.open(filename, ios::out | ios::trunc);

	outfile << "P3\n" << m_width << " " << m_height << "\n255\n";

	for (int j = m_height - 1; j >= 0; --j)
	{
		for (int i = 0; i < m_width; ++i)
		{
			const Vec3& color = m_imageData[i + j * m_width];
			int ir = int(255.99f * color.r());
			int ig = int(255.99f * color.g());
			int ib = int(255.99f * color.b());

			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}

	outfile.close();
}