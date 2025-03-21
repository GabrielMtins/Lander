#ifndef VEC3_H
#define VEC3_H

#include <stdbool.h>

typedef struct {
	float x, y, z;
} Vec3;

void Vec3_Add(Vec3 *result, const Vec3 *a, const Vec3 *b);

void Vec3_Sub(Vec3 *result, const Vec3 *a, const Vec3 *b);

void Vec3_Mul(Vec3 *result, const Vec3 *a, float m);

bool Vec3_Div(Vec3 *result, const Vec3 *a, float m);

bool Vec3_Normalize(Vec3 *result, const Vec3 *a);

void Vec3_Clip(Vec3 *result, const Vec3 *vector, const Vec3 *clip);

float Vec3_SizeSqr(const Vec3 *vec);

float Vec3_Size(const Vec3 *vec);

float Vec3_Dot(const Vec3 *a, const Vec3 *b);

void Vec3_Cross(Vec3 *result, const Vec3 *a, const Vec3 *b);

void Vec3_Print(const Vec3 *a);

#endif
