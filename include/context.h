#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <glad/glad.h>

#include "mems.h"

enum keys_e {
	INPUT_LEFT = 0,
	INPUT_DOWN,
	INPUT_UP,
	INPUT_RIGHT,
	INPUT_SELECT,
	INPUT_START,
	INPUT_A,
	INPUT_B,
	INPUT_X,
	INPUT_Y,
	INPUT_L,
	INPUT_R,
	NUM_KEYS
};

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_RWops *data;
	SDL_GLContext gl_context;

	int width, height;

	size_t tick;
	size_t delta_tick;
	float dt;

	size_t min_time_frame;

	bool quit;

	int *key_mapping;
	int *button_mapping;

	Mems *memory;

	const uint8_t *keyboard_state;
	SDL_GameController *controller;
} Context;

Context *Context_Create(const char *title, int w, int h, Mems *memory);

bool Context_OpenGameController(Context *context, int joystick_index);

bool Context_GetKey(Context *context, int key);

bool Context_SetFps(Context *context, size_t fps);

bool Context_SetDataFromMem(Context *context, const char *data, int size);

bool Context_SetDataFromFile(Context *context, const char *filename);

void Context_PollEvent(Context *context);

bool Context_Destroy(Context *context);

#endif
