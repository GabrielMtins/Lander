#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "EditorDef.hpp"
#include <unordered_map>

class Context {
	public:
		Context(void);
		bool pollEvent(void);
		void updateWindow(void);
		void getMouseXY(int *mouse_x, int *mouse_y);

		bool wasM1Pressed(void);
		bool wasM1Released(void);
		void setDrawMousePosition(int draw_mouse_x, int draw_mouse_y);

		void blit(SDL_Surface *surface, SDL_Rect *src, SDL_Rect *dst);
		SDL_Surface * getSurface(void);
		~Context(void);

		std::unordered_map<int, bool> keys;
		float dt;

	private:
		SDL_Window *window;
		SDL_Event event;
		SDL_Surface *surface;
		SDL_Surface *mouse_surface;
		bool quit;
		int w, h, actual_w;
		int mouse_x, mouse_y, draw_mouse_x, draw_mouse_y;
		bool m1_pressed, m1_released;
		uint32_t tick;

		void drawMouse(void);
};

#endif
