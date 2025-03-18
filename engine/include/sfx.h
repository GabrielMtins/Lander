#ifndef SFX_H
#define SFX_H

#include "context.h"

typedef struct {
	Mix_Chunk *chunk;
} Sfx;

typedef struct {
	Mix_Music *music;
} Music;

bool Sfx_Load(Sfx *sfx, const char *file);
bool Sfx_LoadData(Sfx *sfx, Context *context, int offset);
bool Sfx_Play(Sfx *sfx, int channel, int loops);
bool Sfx_Destroy(Sfx *sfx);

bool Music_Load(Music *music, const char *file);
bool Music_LoadData(Music *music, Context *context, int offset);
bool Music_Play(Music *music, int loops);
bool Music_Destroy(Music *music);

#endif
