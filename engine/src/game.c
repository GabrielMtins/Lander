#include "game.h"
#include "render.h"
#include "scene.h"

#include "mesh.h"
#include "shader.h"
#include "mat4.h"

static Mesh mesh;
static Shader shader;

Game *Game_Create(Context *context){
	Game *game;

	Shader_Load(
			&shader,
			Mems_ReadFileAsString(context->stack, "shaders/brush.vs"), 
			Mems_ReadFileAsString(context->stack, "shaders/brush.fs")
			);

	Mesh_BuildUnitTetrahedron(&mesh);

	Mems_Free(context->stack);

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
	if(index < MAX_TEXTURES){ return &game->resources->textures[index]; }

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
	
	Mat4 tmp, tmp2;

	Mat4_Identity(&tmp);
	Shader_SetUniformMat4(&shader, "model", &tmp);

	Mat4_RotateY(&tmp, -game->main_scene->camera.angle.y);
	Mat4_RotateX(&tmp2, -game->main_scene->camera.angle.x);
	Mat4_Mul(&tmp2, &tmp2, &tmp);

	Mat4_Transform(
			&tmp,
			-game->main_scene->camera.position.x, 
			-game->main_scene->camera.position.y, 
			-game->main_scene->camera.position.z
			);

	//Mat4_RotateY(&tmp2, time / 2.0f);
	Mat4_Mul(&tmp, &tmp2, &tmp);
	Shader_SetUniformMat4(&shader, "view", &tmp);

	Mat4_PerspectiveProjection(
			&tmp,
			(float) INTERNAL_WIDTH / INTERNAL_HEIGHT,
			60.0f / 180.0f * 3.141592f,
			100.0f,
			0.1f
			);

	Shader_SetUniformMat4(&shader, "projection", &tmp);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, game->resources->texture_array.texture_id);

	Mesh_Render(&game->main_scene->world->mesh, &shader);

	Render_Present(game->context);

	return true;
}
