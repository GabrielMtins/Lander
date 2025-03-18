#include "mat4.h"
#include <math.h>

void Mat4_Add(Mat4 *result, const Mat4 *a, const Mat4 *b){
	for(int i = 0; i < 16; i++){
		result->arr[i] = a->arr[i] + b->arr[i];
	}
}

void Mat4_Sub(Mat4 *result, const Mat4 *a, const Mat4 *b){
	for(int i = 0; i < 16; i++){
		result->arr[i] = a->arr[i] - b->arr[i];
	}
}

void Mat4_Mul(Mat4 *result, const Mat4 *a, const Mat4 *b){
	for(int j = 0; j < 4; j++){
		for(int i = 0; i < 4; i++){
			float res = 0.0f;

			for(int k = 0; k < 4; k++){
				res += a->arr[k + j * 4] * b->arr[i + 4 * k];
			}

			result->arr[i + j * 4] = res;
		}
	}
}

void Mat4_MulVec3(Vec3 *result, const Mat4 *mat4, const Vec3 *vec3){
	result->x = mat4->arr[0] * vec3->x + mat4->arr[1] * vec3->y + mat4->arr[2] * vec3->z + mat4->arr[3];
	result->y = mat4->arr[4] * vec3->x + mat4->arr[5] * vec3->y + mat4->arr[6] * vec3->z + mat4->arr[7];
	result->z = mat4->arr[8] * vec3->x + mat4->arr[9] * vec3->y + mat4->arr[10] * vec3->z + mat4->arr[11];
}

void Mat4_Reset(Mat4 *result){
	for(int i = 0; i < 16; i++)
		result->arr[i] = 0.0f;
}

void Mat4_Identity(Mat4 *result){
	Mat4_Reset(result);

	for(int i = 0; i < 4; i++){
		result->arr[i * 4 + i] = 1;
	}
}

void Mat4_Transform(Mat4 *result, float x, float y, float z){
	Mat4_Identity(result);

	result->arr[3] = x;
	result->arr[7] = y;
	result->arr[11] = z;
}

void Mat4_Scale(Mat4 *result, float x, float y, float z){
	Mat4_Reset(result);

	result->arr[0] = x;
	result->arr[5] = y;
	result->arr[10] = z;
	result->arr[15] = 1.0f;
}

void Mat4_RotateX(Mat4 *result, float angle){
	Mat4_Reset(result);

	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);

	result->arr[0] = 1.0f;
	result->arr[15] = 1.0f;

	result->arr[5] = cos_angle;
	result->arr[6] = -sin_angle;
	result->arr[9] = sin_angle;
	result->arr[10] = cos_angle;
}

void Mat4_RotateY(Mat4 *result, float angle){
	Mat4_Reset(result);

	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);

	result->arr[0] = cos_angle;
	result->arr[2] = sin_angle;
	result->arr[5] = 1.0f;
	result->arr[8] = -sin_angle;
	result->arr[10] = cos_angle;

	result->arr[15] = 1.0f;
}

void Mat4_RotateZ(Mat4 *result, float angle){
	Mat4_Reset(result);

	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);

	result->arr[0] = cos_angle;
	result->arr[1] = -sin_angle;
	result->arr[4] = sin_angle;
	result->arr[5] = cos_angle;
	result->arr[10] = 1.0f;
	result->arr[15] = 1.0f;
}

void Mat4_PerspectiveProjection(Mat4 *result, float aspect_ratio, float fov, float far, float near){
	Mat4_Reset(result);

	float s = 1.0f / tanf(0.5f * fov);
	float a1 = -(far + near)/ (far - near);
	float a2 = - 2.0f * far * near / (far - near);

	result->arr[0] = s / aspect_ratio;
	result->arr[5] = s;
	result->arr[10] = a1;
	result->arr[11] = a2;
	result->arr[14] = -1.0f;
}
