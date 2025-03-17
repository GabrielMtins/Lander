#include "render.h"

void Render_Clear(Context *context, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	glClearColor((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f, (float) a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render_Present(Context *context){
	SDL_GL_SwapWindow(context->window);
}
