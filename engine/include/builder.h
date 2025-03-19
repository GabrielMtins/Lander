#ifndef BUILDER_H
#define BUILDER_H

#include "g_types.h"
#include "mesh.h"
#include "mems.h"

bool Builder_BuildMesh(Mesh *mesh, Mems *stack, World *world);

#endif
