#include "game.h"
#include "render.h"
#include "scene.h"

#include "mesh.h"
#include "shader.h"
#include "mat4.h"

Game *Game_Create(Context *context){
	Game *game;

	game = (Game *) Mems_Alloc(context->memory, sizeof(Game));

	game->context = context;
	game->resources = (Resource *) Mems_Alloc(context->memory, sizeof(Resource));

	game->main_scene = (Scene *) Mems_Alloc(context->memory, sizeof(Scene));
	game->main_scene->entities = (Entity *) Mems_Alloc(context->memory, sizeof(Entity) * MAX_ENTITIES);
	game->main_scene->world = Mems_Alloc(context->memory, sizeof(World));
	game->main_scene->hud = Mems_Alloc(context->memory, sizeof(Hud));
	game->main_scene->world->mesh.vao = 0;

	Scene_Reset(game->main_scene, game);

	return game;
}

Texture * Game_GetTexture(Game *game, size_t index){
	if(index < MAX_TEXTURES){
		return &game->resources->textures[index];
	}

	return NULL;
}

Sfx * Game_GetChunk(Game *game, size_t index){
	if(index < MAX_TEXTURES){
		return &game->resources->sfx[index];
	}

	return NULL;
}

Music * Game_GetMusic(Game *game, size_t index){
	if(index < MAX_TEXTURES){
		return &game->resources->musics[index];
	}

	return NULL;
}

bool Game_Run(Game *game){
	bool success = true;

	while(!game->context->quit){
		Game_Loop(game);
	}

	return success;
}

static float time = 0.0f;

bool Game_Loop(Game *game){
	Context_PollEvent(game->context);
	Scene_Update(game->main_scene);

	time += game->context->dt;

	Render_Clear(game->context, 0, 0, 0, 255);
	
	Scene_Render(game->main_scene);

	Render_Present(game->context);

	return true;
}
