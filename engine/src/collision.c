#include "collision.h"
#include <math.h>
#include <stdio.h>

bool Collision_CheckLineCircle(float x, float y, float radius, const Vec2 *line_start, const Vec2 *line_end){
	Vec2 origin = (Vec2) {x, y};
	Vec2 diff_origin_end, diff_origin_start;
	Vec2 line_difference;
	Vec2 normal;

	Vec2_Sub(&line_difference, line_start, line_end);
	normal = (Vec2) {-line_difference.y, line_difference.x};
	Vec2_Normalize(&normal, &normal);

	Vec2_Sub(&diff_origin_start, &origin, line_start);

	/* Verificar se o círculo não está tocando na linha */
	if(fabsf(Vec2_Dot(&diff_origin_start, &normal)) > radius)
		return false;

	/* Verificar se quaisquer pontos estão dentro do círculo */
	if(Vec2_SizeSqr(&diff_origin_start) <= radius * radius)
		return true;

	Vec2_Sub(&diff_origin_end, &origin, line_end);

	if(Vec2_SizeSqr(&diff_origin_end) <= radius * radius)
		return true;

	/* Checar se os pontos estão no mesmo semiplano, dividido pela
	 * perpendicular da linha passando pelo centro do círculo;
	 * se eles estiverem no mesmo semiplano, não há colisão.*/

	if(Vec2_Dot(&diff_origin_start, &line_difference) * Vec2_Dot(&diff_origin_end, &line_difference) > 0.0f)
		return false;

	return true;
}

bool Collision_CheckLineLine(const Vec2 *a1, const Vec2 *a2, const Vec2 *b1, const Vec2 *b2){
	/* 
	 * line_diff1 = a2 - a1
	 * origin_diff = b1 - a1
	 * point = a1 + line_diff1 * u, 0 < u < 1
	 * dot(normal_line2, point) = dot(normal_line2, b1)
	 * dot(normal_line2, a1 + line_diff1 * u) = dot(normal_line2, b1)
	 * dot(normal_line2, a1) + u * dot(normal_line2, line_diff1) = dot(normal_line2, b1)
	 * u = dot(normal_line2, b1 - a1) / dot(normal_line2, line_diff1)
	 * u = dot(normal_line2, origin_diff) / dot(normal_line2, line_diff1)
	 */
	Vec2 line_diff1, line_diff2, origin_diff;
	Vec2 normal_line2;
	Vec2 found_point;
	float div, u;

	Vec2_Sub(&line_diff1, a2, a1);
	Vec2_Sub(&line_diff2, b2, b1);
	Vec2_Sub(&origin_diff, b1, a1);
	normal_line2 = (Vec2) {-line_diff2.y, line_diff2.x};

	div = Vec2_Dot(&normal_line2, &line_diff1);

	if(div == 0.0f)
		return false;

	u = Vec2_Dot(&normal_line2, &origin_diff) / Vec2_Dot(&normal_line2, &line_diff1);

	if(u < 0.0f || u >= 1.0f)
		return false;

	Vec2_Mul(&found_point, &line_diff1, u);
	Vec2_Add(&found_point, &found_point, a1);
	/*
	 * ------.------- aqui está o ponto dividindo a reta
	 * ----->.<------ os vetores devem ficar em direção oposta
	 */

	Vec2_Sub(&line_diff1, &found_point, b1);
	Vec2_Sub(&line_diff2, &found_point, b2);

	if(Vec2_Dot(&line_diff1, &line_diff2) > 0.0f)
		return false;

	return true;
}
