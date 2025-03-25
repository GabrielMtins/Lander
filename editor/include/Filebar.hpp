#ifndef FILEBAR_HPP
#define FILEBAR_HPP

#include "Canvas.hpp"
#include "World.hpp"
#include "TopBar.hpp"

class FilebarCanvas : public Canvas {
	public:
		FilebarCanvas(TopBarCanvas *top_bar, SDL_Surface *surface, SDL_Surface *text_surface, World *world);
		void handleInput(Context *context);
		void render(void);

	private:
		SDL_Surface *text_surface;
		World *world;
		std::vector<std::string> options;
		int highlight;
		TopBarCanvas *top_bar;

		std::string filename;
};

#endif
