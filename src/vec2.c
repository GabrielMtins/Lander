#include "vec2.h"

#include <math.h>

void Vec2_Add(Vec2 *result, const Vec2 *a, const Vec2 *b){
	result->x = a->x + b->x;
	result->y = a->y + b->y;
}

void Vec2_Sub(Vec2 *result, const Vec2 *a, const Vec2 *b){
	result->x = a->x - b->x;
	result->y = a->y - b->y;
}

void Vec2_Mul(Vec2 *result, const Vec2 *a, float m){
	result->x = a->x * m;
	result->y = a->y * m;
}

bool Vec2_Div(Vec2 *result, const Vec2 *a, float m){
	if(m == 0.0f)
		return false;

	result->x = a->x / m;
	result->y = a->y / m;

	return true;
}

bool Vec2_Normalize(Vec2 *result, const Vec2 *a){
	return Vec2_Div(result, a, Vec2_Size(a));
}

float Vec2_SizeSqr(const Vec2 *vec){
	return vec->x * vec->x + vec->y * vec->y;
}

float Vec2_Size(const Vec2 *vec){
	return sqrt(Vec2_SizeSqr(vec));
}
