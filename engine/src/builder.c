#include "builder.h"

#include <stddef.h>
#include <math.h>

typedef struct {
	Vertex *vertices;
	size_t vertices_count;
	unsigned int *indices;
	size_t indices_count;
} BuilderContext;

typedef struct {
	float bottom, top;
} Height;

static bool Builder_BuildSector(World *world, int sector_id, BuilderContext *context);
static bool Builder_BuildPlane(World *world, int sector_id, BuilderContext *context,  bool bottom);
static bool Builder_BuildWallEx(const WallCfg *cfg, const Vec2 *end, const Height *start_height, const Height *end_height, BuilderContext *context);
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
		Sector *sector = &world->sectors[i];

		for(size_t j = 0; j < sector->num_walls; j++){
			const Vec2 *position = &sector->walls[j].position;

			world->min_height = fminf(world->min_height, Builder_GetHeight(sector, position, true));
			world->max_height = fmaxf(world->max_height, Builder_GetHeight(sector, position, false));
		}
	}

	for(size_t i = 0; i < world->num_sectors; i++){
		Builder_BuildSector(world, i, &context);
	}

	Mesh_Create(mesh, context.vertices, context.vertices_count, context.indices, context.indices_count);

	Mems_RestoreState(stack, state);

	return true;
}

float Builder_GetHeight(const Sector *sector, const Vec2 *position, bool bottom){
	const Vec2 *origin, *direction;
	Vec2 diff;
	float step, projection, height;

	if(bottom){
		origin = &sector->bottom.origin;
		direction = &sector->bottom.direction;
		step = sector->bottom.step;
		height = sector->bottom.height;
	}
	else{
		origin = &sector->top.origin;
		direction = &sector->top.direction;
		step = sector->top.step;
		height = sector->top.height;
	}

	if(step == 0.0f)
		return height;

	Vec2_Sub(&diff, position, origin);
	projection = Vec2_Dot(&diff, direction);

	return projection * step + height;
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
		Vertex *vertex = &context->vertices[context->vertices_count + i];

		vertex->position = (Vec3){position->x, sector->top.height, position->y};
		vertex->uv = *position;
		vertex->normal = (Vec3){0.0f, 1.0f, 0.0f};
		vertex->layer_index = sector->bottom.texture;
		vertex->color = (Vec3) {0.0f, 1.0f, 0.0f};

		if(bottom){
			vertex->position.y = Builder_GetHeight(sector, position, true);
			vertex->uv.x = vertex->uv.x * sector->bottom.scale.x + sector->bottom.offset.x;
			vertex->uv.y = vertex->uv.y * sector->bottom.scale.y + sector->bottom.offset.y;
		}
		else{
			vertex->position.y = Builder_GetHeight(sector, position, false);
			vertex->uv.x = vertex->uv.x * sector->top.scale.x + sector->top.offset.x;
			vertex->uv.y = vertex->uv.y * sector->top.scale.y + sector->top.offset.y;
		}
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

static bool Builder_BuildWallEx(const WallCfg *cfg, const Vec2 *end, const Height *start_height, const Height *end_height, BuilderContext *context){
	Vec2 diff;
	Vec3 normal;
	float size;

	if(start_height->top - start_height->bottom <= 0.0f)
		return false;

	if(end_height->top - end_height->bottom <= 0.0f)
		return false;

	Vec2_Sub(&diff, &cfg->position, end);
	size = Vec2_Size(&diff);
	normal = (Vec3){-diff.y, 0.0f, diff.x};
	Vec3_Normalize(&normal, &normal);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count],
			cfg->position.x,
			start_height->bottom, 
			cfg->position.y,
			cfg->offset.x,
			cfg->offset.y + cfg->scale.y * start_height->bottom
			);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count + 1],
			cfg->position.x,
			start_height->top,
			cfg->position.y,
			cfg->offset.x,
			cfg->offset.y + cfg->scale.y * (start_height->top)
			);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count + 2],
			end->x,
			end_height->bottom,
			end->y,
			cfg->offset.x + cfg->scale.x * size,
			cfg->offset.y + cfg->scale.y * end_height->bottom
			);

	Vertex_CreateSimple(
			&context->vertices[context->vertices_count + 3],
			end->x,
			end_height->top,
			end->y,
			cfg->offset.x + cfg->scale.x * size,
			cfg->offset.y + cfg->scale.y * (end_height->top)
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
	Height start_height, end_height;
	WallCfg *start_wall, *end_wall;
	int next_sector_wall;

	sector = &world->sectors[sector_id];
	next_sector_wall = (sector_wall + 1) % sector->num_walls;

	start_wall = &sector->walls[sector_wall];
	end_wall = &sector->walls[next_sector_wall];

	if(sector->walls[sector_wall].portal != -1){
		/* Build portal windows */
		start_height = (Height) {
			world->min_height,
			Builder_GetHeight(sector, &start_wall->position, true)
		};

		end_height = (Height) {
			world->min_height,
			Builder_GetHeight(sector, &end_wall->position, true)
		};

		Builder_BuildWallEx(
				start_wall,
				&end_wall->position,
				&start_height,
				&end_height,
				context
				);

		start_height = (Height) {
			Builder_GetHeight(sector, &start_wall->position, false),
			world->max_height
		};

		end_height = (Height) {
			Builder_GetHeight(sector, &end_wall->position, false),
			world->max_height
		};

		Builder_BuildWallEx(
				start_wall,
				&end_wall->position,
				&start_height,
				&end_height,
				context
				);
	}
	else{
		start_height = (Height) {
			Builder_GetHeight(sector, &start_wall->position, true),
			Builder_GetHeight(sector, &start_wall->position, false)
		};	

		end_height = (Height) {
			Builder_GetHeight(sector, &end_wall->position, true),
			Builder_GetHeight(sector, &end_wall->position, false)
		};	

		Builder_BuildWallEx(
				&sector->walls[sector_wall],
				&sector->walls[next_sector_wall].position,
				&start_height,
				&end_height,
				context
				);
	}


	return true;
}
