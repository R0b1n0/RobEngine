#pragma once
#include "Vector.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "../renderer/Color.h"


struct Triangle
{
	Vec3 p[3]{ 0 };
	Color color{ 255,255,255 };
public:
	Triangle() {}
	Triangle(Vec3 vertices[], Color col = Color{ 255,255,255 })
	{
		p[0] = vertices[0];
		p[1] = vertices[1];
		p[2] = vertices[2];
		color = col;
	}
	Triangle(Vec3 a, Vec3 b, Vec3 c, Color col = Color{ 255,255,255 })
	{
		p[0] = a;
		p[1] = b;
		p[2] = c;
		color = col;
	}
};

struct  Mesh
{
	std::vector<Triangle> triangles;
	bool LoadFromObjectFile(std::string sFilename)
	{
		//See https://cs418.cs.illinois.edu/website/text/obj.html#centering

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

			char junk;

			//Only consider vertices for now
			if (line[0] == 'v' && line[1] == ' ')
			{
				s << line;
				Vec3 v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				bool escape = false;

				//for now just use the indexes, trim the rest
				for (auto& character : line)
				{
					if (character == '/')
					{
						escape = true;
						continue;
					}
					else if (escape && character == ' ')
					{
						escape = false;
					}

					if (!escape)
						s << character;
				}

				//Remove the first token
				s >> junk;

				//int tri[3]{0};
				std::vector<int> indices{};
				int index = 0;

				while (s >> index) {
					indices.push_back(index);
				}

				for (int i = 1; i < indices.size() - 1; i++)
				{
					triangles.push_back(Triangle{ verts[indices[0] - 1], verts[indices[i] - 1], verts[indices[i+1] - 1] });
				}
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
inline bool TryGetLinePlaneIntersection(Vec3 planeP, Vec3 planeN, Vec3 lineStart, Vec3 lineEnd, Vec3& result)
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
	return true;
}

inline int ClipTriangleAgainstSpace(Vec3 planeP, Vec3 planeN, Triangle& in_tri, Triangle& out_tri1, Triangle& out_tri2)
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
		out_tri1.color = in_tri.color;

		out_tri1.p[0] = *inside_points[0];

		TryGetLinePlaneIntersection(planeP, planeN, *inside_points[0], *outside_points[0], out_tri1.p[1]);
		TryGetLinePlaneIntersection(planeP, planeN, *inside_points[0], *outside_points[1], out_tri1.p[2]);

		return 1;
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		out_tri1.color = in_tri.color;
		out_tri2.color = in_tri.color;

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
