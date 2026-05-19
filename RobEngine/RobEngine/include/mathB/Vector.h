	#pragma once
#include <cmath>
struct Vec3
{

public:
	// w helps with matrice calculation
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;

public:
	Vec3()
	{
	}
	Vec3(float t) : Vec3(t,t,t)
	{
	}
	Vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

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

struct Vec2
{
public : 
	float x = 0;
	float y = 0;

public : 
	Vec2()
	{
	}
	Vec2(float t) : Vec2(t, t)
	{
	}
	Vec2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vec2 operator+(const Vec2& a)
	{
		return Vec2{ x + a.x, y + a.y};
	}
	Vec2 operator-(const Vec2& a) const
	{
		return Vec2{ x - a.x, y - a.y };
	}
	Vec2 operator*(float const& b)
	{
		return {
			x * b,
			y * b
		};
	}
	Vec2 operator/(float const& b)
	{
		return {
			x / b,
			y / b
		};
	}
	Vec2 operator *=(const Vec2& v)
	{
		x *= v.x;
		y *= v.y;

		return *this;
	}
	Vec2 operator += (Vec2 const& a)
	{
		x += a.x;
		y += a.y;
		return *this;
	}
	Vec2 operator += (float a)
	{
		x += a;
		y += a;
		return *this;
	}
	Vec2 operator -= (Vec2 const& a)
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}
	Vec2 operator -= (float a)
	{
		x -= a;
		y -= a;
		return *this;
	}
	Vec2 operator /= (float a)
	{
		x /= a;
		y /= a;
		return *this;
	}

	static float DotProduct(const Vec2& v1, const Vec2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	float Length()
	{
		return sqrtf(DotProduct(*this, *this));
	}
	Vec2 Normalize()
	{
		float l = Length();
		x /= l;
		y /= l;
		return *this;
	}
	Vec2 Normalized()
	{
		float l = Length();
		return Vec2{
			this->x / l,
			this->y / l
		};
	}
};