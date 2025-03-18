#ifndef SCENE_H
#define SCENE_H

#include "g_types.h"
#include "mems.h"

#define Scene_GetTileProperty(scene, x, y, member, data) Scene_GetTilePropertyMem(scene, x, y, offsetof(Tile, type), data, sizeof(*data))
#define Scene_SetTileProperty(scene, x, y, member, data) Scene_GetTilePropertyMem(scene, x, y, offsetof(Tile, type), data, sizeof(*data))

Scene *Scene_Setup(void);

bool Scene_BuildWorldMesh(Scene *scene);

bool Scene_Reset(Scene *scene, Game *game);

bool Scene_Update(Scene *scene);

bool Scene_Render(Scene *scene);

bool Scene_SetWorldTexture(Scene *scene, Texture *texture);

bool Scene_SetHudTexture(Scene *scene, Texture *texture);

bool Scene_SetHudTile(Scene *scene, int x, int y, int id);

Tile * Scene_GetTile(Scene *scene, int x, int y);

bool Scene_GetTilePropertyMem(Scene *scene, int x, int y, const void *offset, void *value, size_t size);

bool Scene_SetTilePropertyMem(Scene *scene, int x, int y, const void *offset, const void *value, size_t size);

/*
int Scene_GetTileId(Scene *scene, int x, int y, int layer);

bool Scene_SetTileId(Scene *scene, int x, int y, int layer, int id);
*/

Entity * Scene_AddEntity(Scene *scene);

bool Scene_RemoveEntity(Scene *scene, Entity *entity);

#endif
