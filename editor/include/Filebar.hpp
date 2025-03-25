#ifndef FILEBAR_HPP
#define FILEBAR_HPP

#include "Canvas.hpp"
#include "World.hpp"

class FilebarCanvas : public Canvas {
	public:
		FilebarCanvas(SDL_Surface *surface, SDL_Surface *text_surface, World *world);
		void handleInput(Context *context);
		void render(void);

	private:
		SDL_Surface *text_surface;
		World *world;
		std::vector<std::string> options;
		int highlight;

		std::string filename;
};

#endif
