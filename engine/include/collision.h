#ifndef COLLISION_H
#define COLLISION_H

#include "vec2.h"

bool Collision_CheckLineCircle(float x, float y, float radius, const Vec2 *line_start, const Vec2 *line_end);

#endif 
