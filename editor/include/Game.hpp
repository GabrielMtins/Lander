#ifndef GAME_HPP
#define GAME_HPP

#include "Context.hpp"
#include "Canvas.hpp"
#include "TopBar.hpp"
#include "World.hpp"

class Game {
	public:
		Game(void);
		void run(void);

	private:
		Context *context;
		TopBarCanvas *top_bar;
		Canvas *active_canvas;
		Canvas *general_canvas;
		Canvas *output_canvas;

		std::unordered_map<std::string, Canvas *> canvases;
		
		SDL_Surface *text_surface;
		SDL_Surface *mouse_surface;

		World world;
};

#endif
