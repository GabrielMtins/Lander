#include <stdio.h>
#include <stdio.h>
#include <time.h>
#include "context.h"
#include "game.h"
#include "mems.h"
#include "scene.h"
#include "entity.h"

#include "player.h"

#define MEMORY_USED 1024 * 1024 * 32

int main(int argc, char **argv){
	srand(time(NULL));
	Context *context;
	Game *game;
	Mems memory, stack;

	(void) argc;
	(void) argv;

	memory = Mems_Create(malloc(MEMORY_USED), MEMORY_USED);
	stack = Mems_Create(malloc(STACK_SIZE), STACK_SIZE);

	context = Context_Create("hi", INTERNAL_WIDTH, INTERNAL_HEIGHT, &memory, &stack);
	game = Game_Create(context);

	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(1);

	TextureArray_Create(&game->resources->texture_array, 64, 64);

	MegaTexture_Load(&game->resources->mega_textures[0], context, "floor.png");
	TextureArray_Load(&game->resources->texture_array, context, "floor.png");
	TextureArray_Load(&game->resources->texture_array, context, "wall.png");

	Texture_Create(
			&game->resources->textures[0], 
			&game->resources->mega_textures[0],
			0,
			0,
			64,
			64,
			64,
			64
			);

	Scene_SetHudTexture(game->main_scene, Game_GetTexture(game, 0));
	Scene_SetHudTile(game->main_scene, 4, 4, 1);

	Context_SetFps(context, 60);

	Entity *player = Scene_AddEntity(game->main_scene);
	Player_Create(player);

	Game_Run(game);

	Context_Destroy(context);
	free(memory.block);

	printf("%lu\n", memory.top);

	return 0;
}
