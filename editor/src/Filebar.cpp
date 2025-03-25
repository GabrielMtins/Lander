#include "Filebar.hpp"
#include "DrawUtil.hpp"
#include "EditorDef.hpp"

FilebarCanvas::FilebarCanvas(SDL_Surface *surface, SDL_Surface *text_surface, World *world) : 
	Canvas(FILEBAR_X, FILEBAR_Y, FILEBAR_WIDTH, FILEBAR_HEIGHT, surface){
		this->text_surface = text_surface;
		this->world = world;

		options.emplace_back("new");
		options.emplace_back("open");
		options.emplace_back("save");
		options.emplace_back("save as");
		options.emplace_back("quit");
		highlight = 0;

		filename = "";
}

void FilebarCanvas::handleInput(Context *context){
	int x, y, w, h;

	context->getMouseXY(&x, &y);

	if(!inBounds(x, y))
		return;

	setOffset(&x, &y);
	DrawUtil::DropdownMenuSize(options, &w, &h);

	highlight = y / (h / options.size());

	if(x > w || y > h)
		return;

	if(!context->wasM1Pressed())
		return;

	if(highlight >= (int) options.size())
		return;

	if(options[highlight] == "new"){
		printf("criar novo\n");
	}

	if(options[highlight] == "quit"){
		context->quit = true;
	}
}

void FilebarCanvas::render(void){
	int w, h;

	DrawUtil::DropdownMenuSize(options, &w, &h);
	/*
	setColor(topbar_shadow_color);
	fillRect(4, 4, w, h);
	*/

	DrawUtil::DrawDropdownMenu(
			this,
			text_surface,
			options,
			0,
			0,
			highlight
			);
}
