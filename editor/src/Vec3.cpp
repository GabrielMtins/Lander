#include "Vec3.hpp"
#include <cmath>
#include <cstdio>

Vec3::Vec3(void){
	x = y = z = 0.0f;
}

Vec3::Vec3(Vec2 tmp){
	x = tmp.x;
	y = 0.0f;
	z = tmp.y;
}

Vec3::Vec3(float x, float y){
	this->x = x;
	this->y = y;
	this->z = 0;
}

Vec3::Vec3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3 Vec3::operator+(const Vec3& obj) const{
	return Vec3(x + obj.x, y + obj.y, z + obj.z);
}

Vec3 Vec3::operator-(const Vec3& obj) const{
	return Vec3(x - obj.x, y - obj.y, z - obj.z);
}

Vec3 Vec3::operator*(const Vec3& obj) const{
	return Vec3(x * obj.x, y * obj.y, z * obj.z);
}

Vec3 Vec3::operator*(const float& r) const{
	return Vec3(x * r, y * r, z * r);
}

Vec3 Vec3::operator/(const float& r) const{
	return Vec3(x / r, y / r, z / r);
}

Vec3& Vec3::operator+=(const Vec3& obj){
	this->x += obj.x;
	this->y += obj.y;
	this->z += obj.z;

	return *this;
}

Vec3& Vec3::operator-=(const Vec3& obj){
	this->x -= obj.x;
	this->y -= obj.y;
	this->z -= obj.z;

	return *this;
}

Vec3& Vec3::operator*=(const Vec3& obj){
	this->x *= obj.x;
	this->y *= obj.y;
	this->z *= obj.z;

	return *this;
}

Vec3& Vec3::operator*=(const float& r){
	this->x *= r;
	this->y *= r;
	this->z *= r;

	return *this;
}

Vec3& Vec3::operator/=(const float& r){
	this->x /= r;
	this->y /= r;
	this->z /= r;

	return *this;
}

int Vec3::compareLength(float length_compared){
	float current_length = lengthSqr();

	if(current_length < length_compared)
		return -1;
	if(current_length > length_compared)
		return 1;

	return 0;
}

Vec3 Vec3::rotateX(float cos_angle, float sin_angle){
	return Vec3(
			x,
			cos_angle * y - sin_angle * z,
			sin_angle * y + cos_angle * z
			);
}

Vec3 Vec3::rotateY(float cos_angle, float sin_angle){
	return Vec3(
			cos_angle * x + sin_angle * z,
			y,
			-sin_angle * x + cos_angle * z
			);
}

Vec3 Vec3::rotateZ(float cos_angle, float sin_angle){
	return Vec3(
			cos_angle * x - sin_angle * y,
			sin_angle * x + cos_angle * y,
			z
			);
}

Vec3 Vec3::rotateX(float angle){
	return rotateX(cosf(angle), sinf(angle));
}

Vec3 Vec3::rotateY(float angle){
	return rotateY(cosf(angle), sinf(angle));
}

Vec3 Vec3::rotateZ(float angle){
	return rotateZ(cosf(angle), sinf(angle));
}

Vec3 Vec3::normalize(void){
	float l = length();

	if(l == 0)
		return Vec3();

	return Vec3(x / l, y / l, z / l);
}

Vec3 Vec3::sign(void){
	Vec3 other = *this;

	if(other.x < 0) other.x = -1;
	if(other.y < 0) other.y = -1;
	if(other.z < 0) other.z = -1;

	if(other.x > 0) other.x = 1;
	if(other.y > 0) other.y = 1;
	if(other.z > 0) other.z = 1;
	
	return other;
}

Vec3 Vec3::clamp(float new_size){
	return normalize() * new_size;
}

float Vec3::lengthSqr(void){
	return x * x + y * y + z * z;
}

float Vec3::length(void){
	return sqrt(lengthSqr());
}

void Vec3::print(void){
	printf("%f %f %f\n", x, y, z);
}

float Vec3::dot(Vec3 a, Vec3 b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3::cross(Vec3 a, Vec3 b){
	return Vec3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
		);
}

bool Vec3::rectPlaneIntersects(Vec3 start, Vec3 direction, Vec3 plane_normal, float plane_product){
	if(Vec3::dot(plane_normal, direction) == 0) return false;

	float u = (plane_product - Vec3::dot(plane_normal, start)) / Vec3::dot(plane_normal, direction);

	if(u < 0) return false;

	return true;
}

Vec3 Vec3::rectPlaneIntersection(Vec3 start, Vec3 direction, Vec3 plane_normal, float plane_product){
	float u = (plane_product - Vec3::dot(plane_normal, start)) / Vec3::dot(plane_normal, direction);

	return start + direction * u;
}
