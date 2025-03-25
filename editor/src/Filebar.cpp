#include "Filebar.hpp"
#include "DrawUtil.hpp"
#include "EditorDef.hpp"

#include <nfd.h>

FilebarCanvas::FilebarCanvas(TopBarCanvas *top_bar, SDL_Surface *surface, SDL_Surface *text_surface, World *world) : 
	Canvas(FILEBAR_X, FILEBAR_Y, FILEBAR_WIDTH, FILEBAR_HEIGHT, surface){

		this->text_surface = text_surface;
		this->world = world;

		options.emplace_back("new");
		options.emplace_back("open");
		options.emplace_back("save");
		options.emplace_back("save as");
		options.emplace_back("quit");
		highlight = 0;

		this->top_bar = top_bar;
		filename = "";
}

void FilebarCanvas::handleInput(Context *context){
	int x, y, w, h;

	context->getMouseXY(&x, &y);

	if(!inBounds(x, y)){
		return;
	}

	setOffset(&x, &y);
	DrawUtil::DropdownMenuSize(options, &w, &h);

	highlight = y / (h / options.size());

	if(!context->wasM1Released())
		return;

	if(x > w || y > h){
		top_bar->restore();
	}

	if(highlight >= (int) options.size())
		return;

	if(options[highlight] == "new"){
		*world = World();
		top_bar->restore();
	}

	if(options[highlight] == "open"){
		/* TODO */
		nfdchar_t *out = NULL;

		if(NFD_OpenDialog(NULL, ".", &out) == NFD_OKAY){
			printf("%s\n", out);
			free(out);
		}

		top_bar->restore();
	}

	if(options[highlight] == "quit"){
		context->quit = true;
	}

}

void FilebarCanvas::render(void){
	int w, h;

	DrawUtil::DropdownMenuSize(options, &w, &h);

	DrawUtil::DrawDropdownMenu(
			this,
			text_surface,
			options,
			0,
			0,
			highlight
			);
}
