#ifndef COLLISION_H
#define COLLISION_H

#include "vec2.h"

bool Collision_CheckLineCircle(float x, float y, float radius, const Vec2 *line_start, const Vec2 *line_end);

bool Collision_CheckLineLine(const Vec2 *a1, const Vec2 *a2, const Vec2 *b1, const Vec2 *b2);

#endif 
