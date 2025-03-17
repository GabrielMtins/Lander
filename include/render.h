#ifndef RENDER_H
#define RENDER_H

#include "context.h"

void Render_Clear(Context *context, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void Render_Present(Context *context);

#endif
