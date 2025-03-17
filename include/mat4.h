#ifndef MAT4_H
#define MAT4_H

#include "vec3.h"

typedef struct {
	float arr[16];
} Mat4;

void Mat4_Add(Mat4 *result, const Mat4 *a, const Mat4 *b);
void Mat4_Sub(Mat4 *result, const Mat4 *a, const Mat4 *b);
void Mat4_Mul(Mat4 *result, const Mat4 *a, const Mat4 *b);

void Mat4_MulVec3(Vec3 *result, const Mat4 *mat4, const Vec3 *vec3);

void Mat4_Reset(Mat4 *result);
void Mat4_Identity(Mat4 *result);
void Mat4_Transform(Mat4 *result, float x, float y, float z);
void Mat4_Scale(Mat4 *result, float x, float y, float z);
void Mat4_PerspectiveProjection(Mat4 *result, float aspect_ratio, float fov, float far, float near);

#endif
