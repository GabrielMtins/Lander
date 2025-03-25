#ifndef TOP_BAR_HPP
#define TOP_BAR_HPP

#include "Canvas.hpp"
#include <unordered_map>

class TopBarCanvas : public Canvas {
	public:
		TopBarCanvas(int x, int y, int w, int h, SDL_Surface *surface);
		void addButton(const std::string& text);
		void setTexture(SDL_Surface *surface);
		void render(void);
		void handleInput(Context *context);
		void restore(void);

		bool update;

		std::string selected;

	private:
		std::string old_selected;
		void windowMovement(Context *context);
		std::vector<std::string> keys;

		SDL_Surface *surface;
		int button_width;

};

#endif
