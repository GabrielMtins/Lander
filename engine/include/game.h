#ifndef GAME_H
#define GAME_H

#include "context.h"
#include "g_types.h"

Game *Game_Create(Context *context);

Texture * Game_GetTexture(Game *game, size_t index);

Sfx * Game_GetSfx(Game *game, size_t index);

Music * Game_GetMusic(Game *game, size_t index);

bool Game_Run(Game *game);

bool Game_Loop(Game *game);

#endif
