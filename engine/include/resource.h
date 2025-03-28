#ifndef RESOURCE_H
#define RESOURCE_H

#include "texture.h"
#include "sfx.h"
#include "shader.h"

typedef struct {
	MegaTexture mega_textures[MAX_MEGA_TEXTURES];
	Texture textures[MAX_TEXTURES];
	TextureArray texture_array;
	Sfx sfx[MAX_SFX];
	Music musics[MAX_MUSIC];
	Shader shaders[MAX_SHADERS];

	Shader world_shader;
} Resource;

#endif
