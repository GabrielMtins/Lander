#ifndef MESH_H
#define MESH_H

#include <stdint.h>
#include "context.h"
#include "vec2.h"
#include "vec3.h"
#include "shader.h"

typedef struct {
	Vec3 position;
	Vec2 uv;
	Vec3 normal;
	Vec3 color;
	float layer_index;
} Vertex;

typedef struct {
	unsigned int vao, vbo, ebo;
	unsigned int num_indices;
} Mesh;

bool Vertex_CreateSimple(Vertex *vertex, float x, float y, float z, float u, float v);

bool Mesh_Create(Mesh *mesh, const Vertex *vertices, size_t num_vertices, const unsigned int *indices, size_t num_indices);

bool Mesh_BuildUnitTetrahedron(Mesh *mesh);

bool Mesh_Render(Mesh *mesh, Shader *shader);

bool Mesh_Destroy(Mesh *mesh);

#endif
