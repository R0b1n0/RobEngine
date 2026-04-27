#pragma once
#include "mathB/Vector.h"
#include <cmath>

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
