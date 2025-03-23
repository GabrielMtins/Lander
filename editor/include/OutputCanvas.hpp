#ifndef OUTPUT_CANVAS_HPP
#define OUTPUT_CANVAS_HPP

#include "Canvas.hpp"

class OutputCanvas : public Canvas {
	public:
		OutputCanvas(SDL_Surface *surface, SDL_Surface *text_texture);

		void render(void);
		void setMessage(const std::string &msg);

	private:
		std::string msg;
		SDL_Surface *text_texture;
};

#endif
