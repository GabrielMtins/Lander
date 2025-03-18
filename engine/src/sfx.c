#include "sfx.h"

bool Sfx_Load(Sfx *sfx, const char *file){
	sfx->chunk = Mix_LoadWAV(file);

	return sfx->chunk != NULL;
}

bool Sfx_LoadData(Sfx *sfx, Context *context, int offset){
	SDL_RWseek(context->data, offset, RW_SEEK_SET);

	sfx->chunk = Mix_LoadWAV_RW(context->data, 0);

	return sfx->chunk != NULL;
}

bool Sfx_Play(Sfx *sfx, int channel, int loops){
	return Mix_PlayChannel(channel, sfx->chunk, loops) == 0;
}

bool Sfx_Destroy(Sfx *sfx){
	if(sfx->chunk == NULL)
		return false;

	Mix_FreeChunk(sfx->chunk);

	return true;
}

bool Music_Load(Music *music, const char *file){
	music->music = Mix_LoadMUS(file);

	return music->music != NULL;
}

bool Music_LoadData(Music *music, Context *context, int offset){
	SDL_RWseek(context->data, offset, RW_SEEK_SET);

	music->music = Mix_LoadMUS_RW(context->data, 0);

	return music->music != NULL;
}

bool Music_Play(Music *music, int loops){
	return Mix_PlayMusic(music->music, loops) == 0;
}

bool Music_Destroy(Music *music){
	if(music->music == NULL)
		return false;

	Mix_FreeMusic(music->music);
	
	return true;
}
