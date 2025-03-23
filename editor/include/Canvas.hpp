#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdint>
#include <string>
#include <vector>
#include "Context.hpp"

class Canvas {
	public:
		Canvas(int x, int y, int w, int h, SDL_Surface *surface);

		virtual void handleInput(Context *context);
		virtual void render(void);

		void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void setColor(const SDL_Color& color);
		void putPixel(int x, int y);
		void drawLine(int x1, int y1, int x2, int y2);
		void fillRect(int x, int y, int w, int h);
		void drawRect(int x, int y, int w, int h);
		void clear(void);
		void copySurface(SDL_Surface *surface, SDL_Rect *src, int x, int y);

		bool inBounds(int x, int y);
		void setOffset(int *x, int *y);

	protected:
		SDL_Rect rect;

	private:
		SDL_Color color;

		SDL_Surface *surface;
};

#endif
