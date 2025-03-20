#include "collision.h"

bool Collision_CheckLineCircle(float x, float y, float radius, const Vec2 *line_start, const Vec2 *line_end){
	Vec2 origin = (Vec2) {x, y};
	Vec2 diff_origin_end, diff_origin_start;
	Vec2 line_difference;
	Vec2 normal;

	Vec2_Sub(&diff, line_start, line_end);
	normal = (Vec2) {-diff.x, diff.y};
	Vec2_Normalize(&normal, &normal);

	Vec2_Sub(&diff_origin_start, &origin, line_start);

	/* Verificar se o círculo não está tocando na linha */
	if(Vec2_Dot(diff, normal) > radius)
		return false;

	/* Verificar se quaisquer pontos estão dentro do círculo */
	if(Vec2_SizeSqr(diff) <= radius * radius)
		return true;

	Vec2_Sub(&diff_origin_end, &origin, line_end);

	if(Vec2_SizeSqr(diff) <= radius * radius)
		return true;

	/* Checar se os pontos estão no mesmo semiplano, dividido pela
	 * perpendicular da linha passando pelo centro do círculo;
	 * se eles estiverem no mesmo semiplano, não há colisão.*/

	Vec2_Sub(&line_difference, line_start, line_end);

	if(Vec2_Dot(&diff_origin_start, &line_difference) * Vec2_Dot(&diff_origin_end, &line_difference) > 0.0f)
		return false;

	return true;
}
