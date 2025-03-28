#ifndef G_DEF_H
#define G_DEF_H

#define MAX_ENTITIES 16

#define INTERNAL_WIDTH 1280
#define INTERNAL_HEIGHT 720

#define MAX_COLLISIONS 64
#define MAX_SECTORS (4096)
#define MAX_WALLS (8192)
#define MIN_STEP_HEIGHT (3.0f / 16)

/* Tamanho do hud em tiles */
#define HUD_TILE_WIDTH 8
#define HUD_TILE_HEIGHT 8
#define HUD_WIDTH (INTERNAL_WIDTH / HUD_TILE_WIDTH)
#define HUD_HEIGHT (INTERNAL_HEIGHT / HUD_TILE_HEIGHT)
#define HUD_NUM_TILES (HUD_WIDTH * HUD_HEIGHT)

/* Configurações do resource.h */
#define MAX_MEGA_TEXTURES 4
#define MAX_TEXTURES 64
#define MAX_MUSIC 4
#define MAX_SFX 16
#define MAX_SHADERS 64
#define MAX_MESHES 256

/* Tamanho da memória stack, padrão: 16MB */
#define STACK_SIZE 16 * 1024 * 1024

enum WORLD_LAYERS_ENUM {
	WORLD_LAYER_BACKGROUND = 0,
	WORLD_LAYER_FOREGROUND = 1,
	WORLD_LAYER_DETAIL = 2
};

#endif
