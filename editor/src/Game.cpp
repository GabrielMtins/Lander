#include "Game.hpp"
#include "text.h"
#include "TopBar.hpp"
#include "font2.h"
#include "Editor2dCanvas.hpp"
#include "OutputCanvas.hpp"

Game::Game(void){
	SDL_RWops *ops = SDL_RWFromConstMem(font2_png, font2_png_len);
	text_surface = IMG_Load_RW(ops, 1);

	context = new Context();
	active_canvas = nullptr;
	top_bar = new TopBarCanvas(0, 0, TOP_BAR_WIDTH, TOP_BAR_HEIGHT, context->getSurface());
	top_bar->setTexture(text_surface);

	output_canvas = new OutputCanvas(context->getSurface(), text_surface);

	general_canvas = new Canvas(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, context->getSurface());

	top_bar->addButton("file");
	top_bar->addButton("editor");
	top_bar->addButton("view");
	
	canvases["editor"] = new Editor2dCanvas(context->getSurface(), &world, (OutputCanvas *) output_canvas);

	active_canvas = canvases["editor"];

	general_canvas->setColor(0x00, 0x00, 0x00, 0xff);
	general_canvas->clear();

	SDL_ShowCursor(0);
}

void Game::run(void){
	general_canvas->setColor(background_color);
	general_canvas->clear();

	while(context->pollEvent()){
		top_bar->handleInput(context);
		top_bar->render();
		//output_canvas->render();

		/* draw shadow */
		{
			general_canvas->setColor(topbar_shadow_color);
			general_canvas->fillRect(
					TOP_BAR_SHADOW_X,
					TOP_BAR_SHADOW_Y,
					TOP_BAR_SHADOW_WIDTH,
					TOP_BAR_SHADOW_HEIGHT
					);
		}

		//active_canvas = canvases[top_bar->selected];

		if(active_canvas != nullptr){
			active_canvas->handleInput(context);
			active_canvas->render();
		}


		context->updateWindow();
	}
}
