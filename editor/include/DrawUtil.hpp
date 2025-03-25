#ifndef DRAW_UTIL_HPP
#define DRAW_UTIL_HPP

#include "Canvas.hpp"
#include <vector>

class DrawUtil {
	public:
		static void DrawText(Canvas *canvas, SDL_Surface *text_texture, const std::string &text, int x, int y);
		static void DrawButton(Canvas *canvas, SDL_Surface *text_texture, const std::string &text, int x, int y, int highlight);
		static void DrawDropdownMenu(Canvas *canvas, SDL_Surface *text_texture, const std::vector<std::string>& text, int x, int y, int highlight);
		static void DropdownMenuSize(const std::vector<std::string>& text, int *w, int *h);
		static int TextSize(const std::string &text);
		static int ButtonSize(const std::string &text);
		static SDL_Color Blend(const SDL_Color& a, const SDL_Color &b, float blend);
};

#endif
