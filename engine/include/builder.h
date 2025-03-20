#ifndef BUILDER_H
#define BUILDER_H

#include "g_types.h"
#include "mesh.h"
#include "mems.h"

float Builder_GetHeight(const Sector *sector, const Vec2 *position, bool bottom);

bool Builder_BuildMesh(Mesh *mesh, Mems *stack, World *world);

#endif
