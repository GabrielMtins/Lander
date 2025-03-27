#ifndef VEC3_HPP
#define VEC3_HPP

#include "World.hpp"

struct Vec3 {
	float x, y, z;

	Vec3(void);
	Vec3(Vec2 tmp);
	Vec3(float x, float y);
	Vec3(float x, float y, float z);

	Vec3 operator+(const Vec3& obj) const;
	Vec3 operator-(const Vec3& obj) const;
	Vec3 operator*(const Vec3& obj) const;
	Vec3 operator*(const float& r) const;
	Vec3 operator/(const float& r) const;

	Vec3& operator+=(const Vec3& obj);
	Vec3& operator-=(const Vec3& obj);
	Vec3& operator*=(const Vec3& obj);
	Vec3& operator*=(const float& r);
	Vec3& operator/=(const float& r);

	int compareLength(float length_compared);

	Vec3 rotateX(float cos_angle, float sin_angle);
	Vec3 rotateY(float cos_angle, float sin_angle);
	Vec3 rotateZ(float cos_angle, float sin_angle);

	Vec3 rotateX(float angle);
	Vec3 rotateY(float angle);
	Vec3 rotateZ(float angle);

	Vec3 normalize(void);
	Vec3 sign(void);
	Vec3 clamp(float new_size);

	float lengthSqr(void);
	float length(void);

	void print(void);

	static float dot(Vec3 a, Vec3 b);
	static Vec3 cross(Vec3 a, Vec3 b);

	static bool rectPlaneIntersects(Vec3 start, Vec3 direction, Vec3 plane_normal, float plane_product);
	static Vec3 rectPlaneIntersection(Vec3 start, Vec3 direction, Vec3 plane_normal, float plane_product);
};

#endif
