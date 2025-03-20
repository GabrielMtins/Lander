#include "builder.h"

#include <stddef.h>
#include <math.h>

typedef struct {
	Vertex *vertices;
	size_t vertices_count;
	unsigned int *indices;
	size_t indices_count;
} BuilderContext;

static bool Builder_BuildSector(World *world, int sector_id, BuilderContext *context);
static bool Builder_BuildPlane(World *world, int sector_id, BuilderContext *context,  bool bottom);
static bool Builder_BuildWallEx(const WallCfg *cfg, const Vec2 *end, float bottom_height, float top_height, BuilderContext *context);
static bool Builder_BuildWall(World *world, int sector_id, int sector_wall, BuilderContext *context);


bool Builder_BuildMesh(Mesh *mesh, Mems *stack, World *world){
	BuilderContext context;
	size_t state;
	
	world->min_height = 99999.0f;
	world->max_height = -99999.0f;

	context.vertices_count = 0;
	context.indices_count = 0;

	state = Mems_SaveState(stack);

	context.vertices = (Vertex *) Mems_Alloc(stack,	STACK_SIZE / 4);
	context.indices = (unsigned int *) Mems_Alloc(stack, STACK_SIZE / 8);

	for(size_t i = 0; i < world->num_sectors; i++){
		world->min_height = fminf(world->min_height, world->sectors[i].bottom_height);
		world->max_height = fmaxf(world->max_height, world->sectors[i].top_height);
	}

	printf("%f %f\n", world->min_height, world->max_height);

	for(size_t i = 0; i < world->num_sectors; i++){
		Builder_BuildSector(world, i, &context);
	}

	Mesh_Create(mesh, context.vertices, context.vertices_count, context.indices, context.indices_count);

	Mems_RestoreState(stack, state);

	return true;
}

static bool Builder_BuildSector(World *world, int sector_id, BuilderContext *context){
	if(world->sectors[sector_id].is_static){
		return false;
	}

	Builder_BuildPlane(world, sector_id, context, true);
	Builder_BuildPlane(world, sector_id, context, false);

	for(size_t i = 0; i < world->sectors[sector_id].num_walls; i++){
		Builder_BuildWall(world, sector_id, i, context);
	}

	return true;
}

static bool Builder_BuildPlane(World *world, int sector_id, BuilderContext *context, bool bottom){
	Sector *sector;

	sector = &world->sectors[sector_id];

	for(size_t i = 0; i < sector->num_walls; i++){
		const Vec2 *position = &sector->walls[i].position;

		context->vertices[context->vertices_count + i].position = (Vec3){position->x, sector->top_height, position->y};
		context->vertices[context->vertices_count + i].uv = *position;
		context->vertices[context->vertices_count + i].normal = (Vec3){0.0f, 1.0f, 0.0f};
		context->vertices[context->vertices_count + i].layer_index = sector->bottom.texture;
		context->vertices[context->vertices_count + i].color = (Vec3) {0.0f, 1.0f, 0.0f};

		if(bottom)
			context->vertices[context->vertices_count + i].position.y = sector->bottom_height;
	}

	for(size_t i = 1; i < sector->num_walls - 1; i++){
		context->indices[context->indices_count] = context->vertices_count;
		context->indices[context->indices_count + 1] = context->vertices_count + i;
		context->indices[context->indices_count + 2] = context->vertices_count + i + 1;

		context->indices_count += 3;
	}

	context->vertices_count += sector->num_walls;

	return true;
}

static bool Builder_BuildWallEx(const WallCfg *cfg, const Vec2 *end, float bottom_height, float top_height, BuilderContext *context){
	Vec2 diff;
	Vec3 normal;
	float size, diff_height;

	diff_height = top_height - bottom_height;

	if(diff_height <= 0.0f)
		return false;

	Vec2_Sub(&diff, &cfg->position, end);
	size = Vec2_Size(&diff);
	normal = (Vec3){-diff.y, 0.0f, diff.x};
	Vec3_Normalize(&normal, &normal);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count],
			cfg->position.x,
			bottom_height,
			cfg->position.y,
			cfg->offset.x,
			cfg->offset.y
			);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count + 1],
			cfg->position.x,
			top_height,
			cfg->position.y,
			cfg->offset.x,
			cfg->offset.y + cfg->scale.y * diff_height
			);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count + 2],
			end->x,
			bottom_height,
			end->y,
			cfg->offset.x + cfg->scale.x * size,
			cfg->offset.y
			);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count + 3],
			end->x,
			top_height,
			end->y,
			cfg->offset.x + cfg->scale.x * size,
			cfg->offset.y + cfg->scale.y * diff_height
			);

	context->indices[context->indices_count + 0] = context->vertices_count;
	context->indices[context->indices_count + 1] = context->vertices_count + 2;
	context->indices[context->indices_count + 2] = context->vertices_count + 3;

	context->indices[context->indices_count + 3] = context->vertices_count + 0;
	context->indices[context->indices_count + 4] = context->vertices_count + 1;
	context->indices[context->indices_count + 5] = context->vertices_count + 3;

	for(size_t i = 0; i < 4; i++){
		context->vertices[context->vertices_count + i].color = (Vec3) {1.0f, 1.0f, 1.0f};
		context->vertices[context->vertices_count + i].normal = normal;
		context->vertices[context->vertices_count + i].layer_index = cfg->texture;
	}

	context->vertices_count += 4;
	context->indices_count += 6;

	return true;
}

static bool Builder_BuildWall(World *world, int sector_id, int sector_wall, BuilderContext *context){
	Sector *sector;
	int next_sector_wall;

	sector = &world->sectors[sector_id];
	next_sector_wall = (sector_wall + 1) % sector->num_walls;

	if(sector->walls[sector_wall].portal != -1){
		/* Build portal windows */
		Builder_BuildWallEx(
				&world->sectors[sector_id].walls[sector_wall],
				&world->sectors[sector_id].walls[next_sector_wall].position,
				world->min_height,
				sector->bottom_height,
				context
				);

		Builder_BuildWallEx(
				&world->sectors[sector_id].walls[sector_wall],
				&world->sectors[sector_id].walls[next_sector_wall].position,
				world->max_height,
				sector->top_height,
				context
				);
	}
	else{
		Builder_BuildWallEx(
				&world->sectors[sector_id].walls[sector_wall],
				&world->sectors[sector_id].walls[next_sector_wall].position,
				sector->bottom_height,
				sector->top_height,
				context
				);
	}


	return true;

	/*
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
			&context->vertices[context->vertices_count],
			position->x,
			sector->bottom_height,
			position->y,
			offset->x,
			offset->y
			);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count + 1],
			position->x,
			sector->top_height,
			position->y,
			offset->x,
			offset->y + scale->y * diff_height
			);

	position = &sector->walls[next_sector_wall].position;

	printf("%f %f\n\n", position->x, position->y);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count + 2],
			position->x,
			sector->bottom_height,
			position->y,
			offset->x + scale->x * size,
			offset->y
			);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count + 3],
			position->x,
			sector->top_height,
			position->y,
			offset->x + scale->x * size,
			offset->y + scale->y * diff_height
			);

	context->indices[context->indices_count + 0] = context->vertices_count;
	context->indices[context->indices_count + 1] = context->vertices_count + 2;
	context->indices[context->indices_count + 2] = context->vertices_count + 3;

	context->indices[context->indices_count + 3] = context->vertices_count + 0;
	context->indices[context->indices_count + 4] = context->vertices_count + 1;
	context->indices[context->indices_count + 5] = context->vertices_count + 3;

	for(size_t i = 0; i < 4; i++){
		context->vertices[context->vertices_count + i].color = (Vec3) {1.0f, 1.0f, 1.0f};
		context->vertices[context->vertices_count + i].normal = normal;
	}

	context->vertices_count += 4;
	context->indices_count += 6;

	return true;
	*/
}
