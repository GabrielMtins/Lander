#include "mesh.h"

#include <stddef.h>

bool Vertex_CreateSimple(Vertex *vertex, float x, float y, float z, float u, float v){
	if(vertex == NULL)
		return false;

	vertex->position = (Vec3) {x, y, z};
	vertex->uv = (Vec2) {u, v};
	vertex->normal = (Vec3) {0.0f, 0.0f, 0.0f};
	vertex->color = (Vec3) {0.0f, 0.0f, 0.0f};
	vertex->layer_index = 0.0f;

	return true;
}

bool Mesh_Create(Mesh *mesh, const Vertex *vertices, size_t num_vertices, const unsigned int *indices, size_t num_indices){
	mesh->num_indices = num_indices;

	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ebo);

	glBindVertexArray(mesh->vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, layer_index));
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool Mesh_BuildUnitTetrahedron(Mesh *mesh){
	Vertex vertices[4];

	Vertex_CreateSimple(&vertices[0], +0.5f, +0.0f, -0.35f, 0.0f, 0.0f);
	Vertex_CreateSimple(&vertices[1], -0.5f, +0.0f, -0.35f, 0.0f, 1.0f);
	Vertex_CreateSimple(&vertices[2], +0.0f, +0.5f, +0.35f, 1.0f, 0.0f);
	Vertex_CreateSimple(&vertices[3], +0.0f, -0.5f, +0.35f, 1.0f, 1.0f);

	vertices[0].color = (Vec3) {1.0f, 0.0f, 0.0f};
	vertices[1].color = (Vec3) {0.0f, 1.0f, 0.0f};
	vertices[2].color = (Vec3) {0.0f, 0.0f, 1.0f};
	vertices[3].color = (Vec3) {1.0f, 1.0f, 1.0f};

	const unsigned int indices[] = {
		0, 1, 2,
		0, 1, 3,
		1, 2, 3,
		2, 0, 3
	};

	return Mesh_Create(mesh, vertices, 4, indices, 12);
}

bool Mesh_Render(Mesh *mesh, Shader *shader){
	Shader_Use(shader);

	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return true;
}

bool Mesh_Destroy(Mesh *mesh){
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteBuffers(1, &mesh->ebo);
	glDeleteVertexArrays(1, &mesh->vao);

	return true;
}
