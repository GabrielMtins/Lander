#include "Context.hpp"
#include "mouse.h"
#include "EditorDef.hpp"

Context::Context(void){
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow(
			"Editor",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_SCALE * WINDOW_WIDTH,
			WINDOW_SCALE * WINDOW_HEIGHT,
			SDL_WINDOW_RESIZABLE
			);

	surface = SDL_CreateRGBSurface(
			0,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			32,
			0,
			0,
			0,
			0
			);

	w = WINDOW_SCALE * WINDOW_WIDTH;
	h = WINDOW_SCALE * WINDOW_HEIGHT;
	actual_w = WINDOW_SCALE * WINDOW_WIDTH;

	quit = false;

	SDL_RWops *ops = SDL_RWFromConstMem(mouse_png, mouse_png_len);
	mouse_surface = IMG_Load_RW(ops, 1);
	tick = 0;

	SDL_SetWindowBordered(window, SDL_FALSE);
	fullscreen = false;
}

bool Context::pollEvent(void){
	uint32_t new_tick;
	uint32_t delta_tick;
	uint32_t fps = 1000 / 60;
	mouse_xrel = 0;
	mouse_yrel = 0;

	m1_pressed = false;
	m1_released = true;

	for(auto &key : keys)
		key.second = false;

	new_tick = SDL_GetTicks();
	delta_tick = new_tick - tick;

	if(delta_tick < fps){
		SDL_Delay(fps - delta_tick);
	}

	new_tick = SDL_GetTicks();
	dt = 0.001f * (new_tick - tick);
	tick = new_tick;

	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_F11){
					fullscreen = !fullscreen;
					SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				}
				break;

			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
					w = event.window.data1;
					h = event.window.data2;
					actual_w = h * WINDOW_WIDTH / WINDOW_HEIGHT;
				}
				break;

			case SDL_MOUSEMOTION:
				mouse_x = (event.motion.x - (w - actual_w) / 2) * WINDOW_WIDTH / actual_w;
				mouse_y = event.motion.y * WINDOW_HEIGHT / h;
				mouse_xrel = event.motion.xrel;
				mouse_yrel = event.motion.yrel;
				break;

			case SDL_MOUSEBUTTONDOWN:
				m1_pressed = event.button.button & 1;

				break;

			case SDL_MOUSEBUTTONUP:
				m1_released = event.button.button & 1;
				break;

		}
	}

	setDrawMousePosition(mouse_x, mouse_y);

	return !quit;
}

void Context::updateWindow(void){
	SDL_Surface *window_surface = SDL_GetWindowSurface(window);

	SDL_FillRect(window_surface, NULL, SDL_MapRGBA(window_surface->format, 0x00, 0x00, 0x00, 0xff));

	SDL_Rect dst = {(w - actual_w) / 2, 0, actual_w, h};
	SDL_BlitScaled(surface, NULL, window_surface, &dst);

	{
		SDL_Rect src = {0, 0, 1, WINDOW_HEIGHT};
		SDL_Rect dst = {0, 0, (w - actual_w) / 2, h};
		SDL_BlitScaled(surface, &src, window_surface, &dst);
	}

	{
		SDL_Rect src = {WINDOW_WIDTH - 1, 0, 1, WINDOW_HEIGHT};
		SDL_Rect dst = {(w + actual_w) / 2, 0, (w - actual_w) / 2, h};
		SDL_BlitScaled(surface, &src, window_surface, &dst);
	}

	drawMouse();

	SDL_UpdateWindowSurface(window);
}

void Context::getMouseXY(int *mouse_x, int *mouse_y){
	*mouse_x = this->mouse_x;
	*mouse_y = this->mouse_y;
}

bool Context::wasM1Pressed(void){
	return m1_pressed;
}

bool Context::wasM1Released(void){
	return m1_released;
}

void Context::setDrawMousePosition(int draw_mouse_x, int draw_mouse_y){
	this->draw_mouse_x = draw_mouse_x;
	this->draw_mouse_y = draw_mouse_y;
}

SDL_Surface * Context::getSurface(void){
	return surface;
}

Context::~Context(void){
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}

void Context::drawMouse(void){
	SDL_Surface *window_surface = SDL_GetWindowSurface(window);

	{
		SDL_Rect src = {0, 8, 8, 8};
		SDL_Rect dst = {
			(draw_mouse_x + 4) * h / WINDOW_HEIGHT + (w - actual_w) / 2,
			(draw_mouse_y + 6) * h / WINDOW_HEIGHT,
			8 * h / WINDOW_HEIGHT,
			8 * h / WINDOW_HEIGHT
		};
		SDL_BlitScaled(mouse_surface, &src, window_surface, &dst);
	}

	{
		SDL_Rect src = {0, 0, 8, 8};
		SDL_Rect dst = {
			draw_mouse_x * h / WINDOW_HEIGHT + (w - actual_w) / 2,
			draw_mouse_y * h / WINDOW_HEIGHT,
			8 * h / WINDOW_HEIGHT,
			8 * h / WINDOW_HEIGHT
		};
		SDL_BlitScaled(mouse_surface, &src, window_surface, &dst);
	}

}
