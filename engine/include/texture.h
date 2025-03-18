#ifndef TEXTURE_H
#define TEXTURE_H

#include "context.h"

typedef struct {
	unsigned int texture_id;
	int w, h;
} MegaTexture;

typedef struct {
	MegaTexture *mega_texture;

	int x, y;
	int cell_w, cell_h;
	int num_cells_x;
} Texture;

bool Texture_LoadDefaults(void);

bool MegaTexture_Load(MegaTexture *mega_texture, Context *context, const char *filename);

bool MegaTexture_LoadData(MegaTexture *mega_texture, Context *context, size_t offset);

bool MegaTexture_Destroy(MegaTexture *mega_texture);

bool Texture_Create(Texture *texture, MegaTexture *mega_texture, int x, int y, int w, int h, int cell_w, int cell_h);

bool Texture_Render(Context *context, Texture *texture, int x, int y, int id, int flip);

#endif
