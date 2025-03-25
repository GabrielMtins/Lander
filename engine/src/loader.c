#include "loader.h"
#include <stdio.h>
#include <string.h>

static bool Loader_LoadCfgs(FILE *file, World *world, size_t num_walls);
static bool Loader_LoadWalls(FILE *file, World *world, size_t num_walls);

bool Loader_LoadWorld(World *world, const char *filename){
	FILE *file;
	char tag[64];
	size_t num_walls;

	world->num_sectors = 0;
	world->num_walls = 0;

	file = fopen(filename, "r");

	if(file == NULL){
		fprintf(stderr, "Failed to load map: %s\n", filename);
		return false;
	}

	while(!feof(file)){
		if(fscanf(file, "%s", tag) != 1)
			break;

		if(strcmp("sector", tag) == 0){
			fscanf(file, "%lu", &num_walls);
			Loader_LoadCfgs(file, world, num_walls);
			Loader_LoadWalls(file, world, num_walls);
		}
	}
	
	fclose(file);

	return true;
}

static bool Loader_LoadCfgs(FILE *file, World *world, size_t num_walls){
	Sector *sector = &world->sectors[world->num_sectors++];
	char tmp[20];

	sector->num_walls = num_walls;

	fscanf(
			file,
			"%s %f %f %f %f %f %i %f %f",
			tmp,
			&sector->bottom.height,
			&sector->bottom.offset.x,
			&sector->bottom.offset.y,
			&sector->bottom.scale.x,
			&sector->bottom.scale.y,
			&sector->bottom.wall_step,
			&sector->bottom.step,
			&sector->bottom.texture
		  );

	fscanf(
			file,
			"%s %f %f %f %f %f %i %f %f",
			tmp,
			&sector->top.height,
			&sector->top.offset.x,
			&sector->top.offset.y,
			&sector->top.scale.x,
			&sector->top.scale.y,
			&sector->top.wall_step,
			&sector->top.step,
			&sector->top.texture
		  );
	
	sector->walls = world->walls + world->num_walls;

	return true;
}

static bool Loader_LoadWalls(FILE *file, World *world, size_t num_walls){
	char tmp[20];

	while(num_walls-- != 0){
		WallCfg *wall = &world->walls[world->num_walls++];

		fscanf(
				file,
				"%s %f %f %f %f %f %f %f %i",
				tmp,
				&wall->position.x,
				&wall->position.y,
				&wall->offset.x,
				&wall->offset.y,
				&wall->scale.x,
				&wall->scale.y,
				&wall->texture,
				&wall->portal
			  );
	}

	return true;
}
