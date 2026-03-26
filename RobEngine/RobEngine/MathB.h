#pragma once
struct Vec3
{
	// w helps with matrice calculation
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;


	Vec3 operator+(const Vec3& a)
	{
		return Vec3{ x + a.x, y + a.y, z + a.z };
	}
	Vec3 operator-(const Vec3& a) const
	{
		return Vec3{ x - a.x, y - a.y, z - a.z };
	}
	Vec3 operator*(float const& b)
	{
		return {
			x * b,
			y * b,
			z * b,
		};
	}
	Vec3 operator/(float const& b)
	{
		return {
			x / b,
			y / b,
			z / b,
		};
	}
	Vec3 operator *=(const Vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}
	Vec3 operator += (Vec3 const& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	Vec3 operator += (float a)
	{
		x += a;
		y += a;
		z += a;
		return *this;
	}
	Vec3 operator -= (Vec3 const& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}
	Vec3 operator -= (float a)
	{
		x -= a;
		y -= a;
		z -= a;
		return *this;
	}
	Vec3 operator /= (float a)
	{
		x /= a;
		y /= a;
		z /= a;
		return *this;
	}

	static float DotProduct(const Vec3& v1, const Vec3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	static Vec3 CrossProduct(const Vec3& u, const Vec3& v)
	{
		return Vec3{
			u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x,
		};
	}

	float Length()
	{
		return sqrtf(DotProduct(*this, *this));
	}
	Vec3 Normalize()
	{
		float l = Length();
		x /= l;
		y /= l;
		z /= l;
		return *this;
	}
	Vec3 Normalized()
	{
		float l = Length();
		return Vec3{
			this->x / l,
			this->y / l,
			this->z / l,
		};
	}
};

struct Mat4x4
{
public:
	// [Row] [Column]
	float m[4][4] = { 0 };

	/// <summary>
	/// Perform a matrice multiplication such as Vert * Mat
	/// </summary>
	/// <param name="vec">Left operand</param>
	/// <returns></returns>
	Vec3 Multiply(const Vec3& vec)
	{
		Vec3 result{};
		result.x = vec.x * m[0][0] + vec.y * m[1][0] + vec.z * m[2][0] + vec.w * m[3][0];
		result.y = vec.x * m[0][1] + vec.y * m[1][1] + vec.z * m[2][1] + vec.w * m[3][1];
		result.z = vec.x * m[0][2] + vec.y * m[1][2] + vec.z * m[2][2] + vec.w * m[3][2];
		result.w = vec.x * m[0][3] + vec.y * m[1][3] + vec.z * m[2][3] + vec.w * m[3][3];
		return result;
	}
	Mat4x4 operator*(const Mat4x4& o)
	{
		Mat4x4 result{};
		for (int r = 0; r < 4; r++)
			for (int c = 0; c < 4; c++)
				result.m[r][c] = m[r][0] * o.m[0][c] + m[r][1] * o.m[1][c] + m[r][2] * o.m[2][c] + m[r][3] * o.m[3][c];
		return result;
	}

	static Mat4x4 MakeProjectionMatrix(float a, float fovRad, float fNear, float fFar)
	{
		Mat4x4 mat{};
		mat.m[0][0] = a * fovRad;
		mat.m[1][1] = fovRad;
		mat.m[2][2] = fFar / (fFar - fNear);
		mat.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		mat.m[2][3] = 1.0f;
		mat.m[3][3] = 0.0f;
		return mat;
	}
	static Mat4x4 MakeRotationMatrixX(float angle)
	{
		Mat4x4 mat{};
		mat.m[0][0] = 1;
		mat.m[1][1] = cosf(angle);
		mat.m[1][2] = sinf(angle);
		mat.m[2][1] = -sinf(angle);
		mat.m[2][2] = cosf(angle);
		mat.m[3][3] = 1;
		return mat;
	}
	static Mat4x4 MakeRotationMatrixY(float angle)
	{
		Mat4x4 mat{};
		mat.m[0][0] = cos(angle);
		mat.m[0][2] = sin(angle);
		mat.m[1][1] = 1;
		mat.m[2][0] = -sin(angle);
		mat.m[2][2] = cos(angle);
		mat.m[3][3] = 1;
		return mat;
	}
	static Mat4x4 MakeRotationMatriZ(float angle)
	{
		Mat4x4 mat{};
		mat.m[0][0] = cosf(angle);
		mat.m[0][1] = sinf(angle);
		mat.m[1][0] = -sinf(angle);
		mat.m[1][1] = cosf(angle);
		mat.m[2][2] = 1;
		mat.m[3][3] = 1;
		return mat;
	}
	static Mat4x4 MakeTranslationMatrix(float x, float y, float z)
	{
		Mat4x4 mat{};
		mat.m[0][0] = 1.0f;
		mat.m[1][1] = 1.0f;
		mat.m[2][2] = 1.0f;
		mat.m[3][3] = 1.0f;
		mat.m[3][0] = x;
		mat.m[3][1] = y;
		mat.m[3][2] = z;
		return mat;
	}
	static Mat4x4 MakePointAt(const Vec3& pos, const Vec3& target, const Vec3& up)
	{
		Vec3 newForward = (target - pos).Normalize();

		Vec3 upOffset = newForward * Vec3::DotProduct(up, newForward);
		Vec3 newUp = (up - upOffset).Normalize();

		Vec3 newRight = Vec3::CrossProduct(newUp, newForward);

		Mat4x4 mat;
		mat.m[0][0] = newRight.x;	mat.m[0][1] = newRight.y;	mat.m[0][2] = newRight.z;	mat.m[0][3] = 0.0f;
		mat.m[1][0] = newUp.x;		mat.m[1][1] = newUp.y;		mat.m[1][2] = newUp.z;		mat.m[1][3] = 0.0f;
		mat.m[2][0] = newForward.x;	mat.m[2][1] = newForward.y;	mat.m[2][2] = newForward.z;	mat.m[2][3] = 0.0f;
		mat.m[3][0] = pos.x;		mat.m[3][1] = pos.y;		mat.m[3][2] = pos.z;		mat.m[3][3] = 1.0f;
		return mat;
	}
	static Mat4x4 MatrixQuickInverse(Mat4x4& m) // Only for Rotation/Translation Matrices
	{
		Mat4x4 matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}
	static Mat4x4 MakeIdentity()
	{
		Mat4x4 result{};
		//Everything doesn't have to be a for loop >:(
		result.m[0][0] = 1;
		result.m[1][1] = 1;
		result.m[2][2] = 1;
		result.m[3][3] = 1;
		return result;
	}
};

struct Triangle
{
	Vec3 p[3]{ 0 };
	sf::Color col = sf::Color::White;
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

float Max(float a, float b)
{
	return a > b ? a : b;
}
float Min(float a, float b)
{
	return a < b ? a : b;
}
