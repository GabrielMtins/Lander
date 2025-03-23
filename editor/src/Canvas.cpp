#include "Canvas.hpp"
#include "Context.hpp"

#define c_abs(x) (x < 0 ? -x : x)

Canvas::Canvas(int x, int y, int w, int h, SDL_Surface *surface){
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	this->surface = surface;
}

void Canvas::handleInput(Context *context){
	(void) context;
}

void Canvas::render(void){
}

void Canvas::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void Canvas::setColor(const SDL_Color& color){
	this->color = color;
}

void Canvas::putPixel(int x, int y){
	int id;
	uint32_t *pixels;

	if(x < 0 || y < 0 || x >= rect.w || y >= rect.h)
		return;

	id = (y + rect.y) * surface->w + x + rect.x;

	pixels = (uint32_t *) surface->pixels;

	pixels[id] = SDL_MapRGBA(
			surface->format,
			color.r,
			color.g,
			color.b,
			color.a
			);
}

void Canvas::drawLine(int x1, int y1, int x2, int y2){
	int x, y, dx, dy, steps;

	dx = x2 - x1;
	dy = y2 - y1;

	if(c_abs(dx) > c_abs(dy))
		steps = c_abs(dx);
	else
		steps = c_abs(dy);

	for(int i = 0; i < steps; i++){
		x = x1 + dx * i / steps;
		y = y1 + dy * i / steps;
		
		putPixel(x, y);
	}
}

void Canvas::fillRect(int x, int y, int w, int h){
	SDL_Rect dst = {x + rect.x, y + rect.y, w, h};

	SDL_SetClipRect(surface, &rect);
	SDL_FillRect(surface, &dst, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
	SDL_SetClipRect(surface, NULL);
}

void Canvas::clear(void){
	SDL_Rect rct = {rect.x, rect.y, rect.w, rect.h};
	SDL_FillRect(surface, &rct, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
}

void Canvas::copySurface(SDL_Surface *surface, SDL_Rect *src, int x, int y){
	SDL_SetClipRect(surface, &rect);
	SDL_Rect dst = {x + rect.x, y + rect.y, src->w, src->h};
	SDL_BlitScaled(surface, src, this->surface, &dst);
	SDL_SetClipRect(surface, NULL);
}

bool Canvas::inBounds(int x, int y){
	x -= rect.x;
	y -= rect.y;
	
	if(x < 0 || y < 0 || x >= rect.w || y >= rect.h)
		return false;

	return true;
}

void Canvas::setOffset(int *x, int *y){
	*x -= rect.x;
	*y -= rect.y;
}
