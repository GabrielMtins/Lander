#include "vec3.h"

#include <math.h>

void Vec3_Add(Vec3 *result, const Vec3 *a, const Vec3 *b){
	result->x = a->x + b->x;
	result->y = a->y + b->y;
	result->z = a->z + b->z;
}

void Vec3_Sub(Vec3 *result, const Vec3 *a, const Vec3 *b){
	result->x = a->x - b->x;
	result->y = a->y - b->y;
	result->z = a->z - b->z;
}

void Vec3_Mul(Vec3 *result, const Vec3 *a, float m){
	result->x = a->x * m;
	result->y = a->y * m;
	result->z = a->z * m;
}

bool Vec3_Div(Vec3 *result, const Vec3 *a, float m){
	if(m == 0.0f)
		return false;

	result->x = a->x / m;
	result->y = a->y / m;
	result->z = a->z / m;

	return true;
}

bool Vec3_Normalize(Vec3 *result, const Vec3 *a){
	return Vec3_Div(result, a, Vec3_Size(a));
}

float Vec3_SizeSqr(const Vec3 *vec){
	return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
}

float Vec3_Size(const Vec3 *vec){
	return sqrt(Vec3_SizeSqr(vec));
}

float Vec3_Dot(const Vec3 *a, const Vec3 *b){
	return a->x * b->x + a->y * b->y + a->z * b->z;
}
