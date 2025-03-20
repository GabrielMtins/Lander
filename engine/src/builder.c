#include "builder.h"

#include <stddef.h>

static bool Builder_BuildSector(World *world, int sector_id, Vertex *vertices, size_t *vertices_count, unsigned int *indices, size_t *indices_count);
static bool Builder_BuildPlane(World *world, int sector_id, Vertex *vertices, size_t *vertices_count, unsigned int *indices, size_t *indices_count, bool bottom);
static bool Builder_BuildWall(World *world, int sector_id, int sector_wall, Vertex *vertices, size_t *vertices_count, unsigned int *indices, size_t *indices_count);


bool Builder_BuildMesh(Mesh *mesh, Mems *stack, World *world){
	Vertex *vertices;
	unsigned int *indices;
	size_t state, vertices_count, indices_count;

	vertices_count = 0;
	indices_count = 0;

	state = Mems_SaveState(stack);

	vertices = (Vertex *) Mems_Alloc(stack,	STACK_SIZE / 4);
	indices = (unsigned int *) Mems_Alloc(stack, STACK_SIZE / 8);

	for(size_t i = 0; i < world->num_sectors; i++){
		Builder_BuildSector(world, i, vertices, &vertices_count, indices, &indices_count);
	}

	Mesh_Create(mesh, vertices, vertices_count, indices, indices_count);

	Mems_RestoreState(stack, state);

	return true;
}

static bool Builder_BuildSector(World *world, int sector_id, Vertex *vertices, size_t *vertices_count, unsigned int *indices, size_t *indices_count){
	if(world->sectors[sector_id].is_static){
		return false;
	}

	Builder_BuildPlane(world, sector_id, vertices, vertices_count, indices, indices_count, true);
	Builder_BuildPlane(world, sector_id, vertices, vertices_count, indices, indices_count, false);

	for(size_t i = 0; i < world->sectors[sector_id].num_walls; i++){
		Builder_BuildWall(world, sector_id, i, vertices, vertices_count, indices, indices_count);
	}

	return true;
}

static bool Builder_BuildPlane(World *world, int sector_id, Vertex *vertices, size_t *vertices_count, unsigned int *indices, size_t *indices_count, bool bottom){
	Sector *sector;

	sector = &world->sectors[sector_id];

	for(size_t i = 0; i < sector->num_walls; i++){
		const Vec2 *position = &sector->walls[i].position;

		vertices[*vertices_count + i].position = (Vec3){position->x, sector->top_height, position->y};
		vertices[*vertices_count + i].uv = *position;
		vertices[*vertices_count + i].normal = (Vec3){0.0f, 1.0f, 0.0f};
		vertices[*vertices_count + i].layer_index = sector->bottom.texture;
		vertices[*vertices_count + i].color = (Vec3) {0.0f, 1.0f, 0.0f};

		if(bottom)
			vertices[*vertices_count + i].position.y = sector->bottom_height;
	}

	for(size_t i = 1; i < sector->num_walls - 1; i++){
		indices[*indices_count] = *vertices_count;
		indices[*indices_count + 1] = *vertices_count + i;
		indices[*indices_count + 2] = *vertices_count + i + 1;

		*indices_count += 3;
	}

	*vertices_count += sector->num_walls;

	return true;
}

static bool Builder_BuildWall(World *world, int sector_id, int sector_wall, Vertex *vertices, size_t *vertices_count, unsigned int *indices, size_t *indices_count){
	Sector *sector;
	int next_sector_wall;
	const Vec2 *position, *offset, *scale;
	Vec3 normal;
	Vec2 diff;
	float diff_height, size;


	sector = &world->sectors[sector_id];
	next_sector_wall = (sector_wall + 1) % sector->num_walls;

	if(sector->walls[sector_wall].portal != -1)
		return false;

	diff_height = sector->top_height - sector->bottom_height;

	position = &sector->walls[sector_wall].position;
	offset = &sector->walls[sector_wall].offset;
	scale = &sector->walls[sector_wall].scale;

	Vec2_Sub(&diff, position, &sector->walls[next_sector_wall].position);
	size = Vec2_Size(&diff);
	normal = (Vec3){-diff.y, 0.0f, diff.x};
	Vec3_Normalize(&normal, &normal);

	printf("%f %f\n", position->x, position->y);

	Vertex_CreateSimple(
			&vertices[*vertices_count],
			position->x,
			sector->bottom_height,
			position->y,
			offset->x,
			offset->y
			);

	Vertex_CreateSimple(
			&vertices[*vertices_count + 1],
			position->x,
			sector->top_height,
			position->y,
			offset->x,
			offset->y + scale->y * diff_height
			);

	position = &sector->walls[next_sector_wall].position;

	printf("%f %f\n\n", position->x, position->y);

	Vertex_CreateSimple(
			&vertices[*vertices_count + 2],
			position->x,
			sector->bottom_height,
			position->y,
			offset->x + scale->x * size,
			offset->y
			);

	Vertex_CreateSimple(
			&vertices[*vertices_count + 3],
			position->x,
			sector->top_height,
			position->y,
			offset->x + scale->x * size,
			offset->y + scale->y * diff_height
			);

	indices[*indices_count + 0] = *vertices_count;
	indices[*indices_count + 1] = *vertices_count + 2;
	indices[*indices_count + 2] = *vertices_count + 3;

	indices[*indices_count + 3] = *vertices_count + 0;
	indices[*indices_count + 4] = *vertices_count + 1;
	indices[*indices_count + 5] = *vertices_count + 3;

	for(size_t i = 0; i < 4; i++){
		vertices[*vertices_count + i].color = (Vec3) {1.0f, 1.0f, 1.0f};
		vertices[*vertices_count + i].normal = normal;
	}

	*vertices_count += 4;
	*indices_count += 6;

	return true;
}
