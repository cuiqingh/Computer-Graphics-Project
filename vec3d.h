#pragma once
#include <math.h>

class Point3D		//三维点类
{
public:
	float x, y, z;	//三维坐标

	Point3D& set(float r, float s, float t)
	{
		x = r;
		y = s;
		z = t;
		return *this;
	}

	Point3D& setx(float r)
	{
		x = r;
		return *this;
	}

	Point3D& sety(float r)
	{
		y = r;
		return *this;
	}

	Point3D& setz(float r)
	{
		z = r;
		return *this;
	}

	Point3D& operator =(Point3D& p) {
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}
};

class Vector3D
{
public:
	float x, y, z;	//三维向量

	Vector3D() {};

	Vector3D(float r, float s, float t) {
		x = r;
		y = s;
		z = t;
	}

	Vector3D& set(float r, float s, float t) {
		x = r;
		y = s;
		z = t;
		return *this;
	}

	Vector3D& normalize() {
		//向量标准化
		float inverseSqrt = 1.0f / sqrt(x * x + y * y + z * z);
		x *= inverseSqrt;
		y *= inverseSqrt;
		z *= inverseSqrt;
		return *this;
	}

	Vector3D& cross(Vector3D& v1, Vector3D& v2) {
		x = v1.y * v2.z - v1.z * v2.y;
		y = v1.z * v2.x - v1.x * v2.z;
		z = v1.x * v2.y - v1.y * v2.x;
		return *this;
	}

	Vector3D& operator =(Vector3D& p) {
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}

	Vector3D operator +(Vector3D& p) {
		return Vector3D(p.x + this->x, p.y + this->y, p.z + this->z);
	}
};