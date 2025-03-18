#ifndef VEC2_H
#define VEC2_H

#include <stdbool.h>

typedef struct {
	float x, y;
} Vec2;

void Vec2_Add(Vec2 *result, const Vec2 *a, const Vec2 *b);

void Vec2_Sub(Vec2 *result, const Vec2 *a, const Vec2 *b);

void Vec2_Mul(Vec2 *result, const Vec2 *a, float m);

bool Vec2_Div(Vec2 *result, const Vec2 *a, float m);

bool Vec2_Normalize(Vec2 *result, const Vec2 *a);

float Vec2_SizeSqr(const Vec2 *vec);

float Vec2_Size(const Vec2 *vec);

#endif
