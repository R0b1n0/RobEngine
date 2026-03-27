#pragma once
#include "Vector.h"

struct Triangle
{
	Vec3 p[3]{ 0 };
	sf::Color col = sf::Color::White;
};
struct  Mesh
{
	std::vector<Triangle> triangles;
	bool LoadFromObjectFile(std::string sFilename)
	{
		std::ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		std::vector<Vec3> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::stringstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				Vec3 v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				triangles.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}

		return true;
	}
};

/// <summary>
/// Find the intersection point between a plane and a line if it exist.
/// I don't fucking get the math tho, call me if you do.
/// </summary>
/// <param name="planeP :">point within the said plane.</param>
/// <param name="planeN :">plane Normal.</param>
/// <returns>True if there is an intersection.</returns>
bool TryGetLinePlaneIntersection(Vec3 planeP, Vec3 planeN, Vec3 lineStart, Vec3 lineEnd, Vec3& result)
{
	planeN.Normalize();
	Vec3 line = lineEnd - lineStart;

	//if line is colinear to the plane, return 
	if (Vec3::DotProduct(planeN, line) == 0)
		return false;

	float A = Vec3::DotProduct(planeN, planeP);
	float B = Vec3::DotProduct(planeN, lineStart);
	float C = Vec3::DotProduct(planeN, lineEnd);
	float t = (A - B) / (C - B);

	if (t < 0 || t > 1)
		return false;

	result = lineStart + line * t;
}

int ClipTriangleAgainstSpace(Vec3 planeP, Vec3 planeN, Triangle& in_tri, Triangle& out_tri1, Triangle& out_tri2)
{
	planeN.Normalize();

	//Check how many vertices are below the plane 

	float dotNP = Vec3::DotProduct(planeN, planeP);
	auto getSd = [&](Vec3& p)
		{
			return (planeN.x * p.x + planeN.y * p.y + planeN.z * p.z - dotNP);
		};

	Vec3* inside_points[3];  int nInsidePointCount = 0;
	Vec3* outside_points[3]; int nOutsidePointCount = 0;

	for (int i = 0; i < 3; i++)
	{
		if (getSd(in_tri.p[i]) >= 0) {
			inside_points[nInsidePointCount++] = &in_tri.p[i];
		}
		else {
			outside_points[nOutsidePointCount++] = &in_tri.p[i];
		}
	}

	if (nInsidePointCount == 0)
	{
		return 0;
	}

	if (nInsidePointCount == 3)
	{
		out_tri1 = in_tri;
		return 1;
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		out_tri1.col = in_tri.col;

		out_tri1.p[0] = *inside_points[0];

		TryGetLinePlaneIntersection(planeP, planeN, *inside_points[0], *outside_points[0], out_tri1.p[1]);
		TryGetLinePlaneIntersection(planeP, planeN, *inside_points[0], *outside_points[1], out_tri1.p[2]);

		return 1;
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		out_tri1.col = in_tri.col;
		out_tri2.col = in_tri.col;

		out_tri1.p[0] = *inside_points[0];
		out_tri1.p[1] = *inside_points[1];
		TryGetLinePlaneIntersection(planeP, planeN, *inside_points[0], *outside_points[0], out_tri1.p[2]);

		out_tri2.p[0] = *inside_points[1];
		out_tri2.p[1] = out_tri1.p[2];
		TryGetLinePlaneIntersection(planeP, planeN, *inside_points[1], *outside_points[0], out_tri2.p[2]);

		return 2;
	}

	return 0;
}
