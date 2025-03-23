#include "TopBar.hpp"
#include "DrawUtil.hpp"
#include "EditorDef.hpp"
#include "Context.hpp"
#include <ctype.h>
#include <iostream>

#include <cstdlib>

TopBarCanvas::TopBarCanvas(int x, int y, int w, int h, SDL_Surface *surface) : Canvas(x, y, w, h, surface){
	selected = "editor";
	update = true;
}

void TopBarCanvas::addButton(const std::string& text){
	keys.push_back(text);
}

void TopBarCanvas::setTexture(SDL_Surface *surface){
	this->surface = surface;
}

void TopBarCanvas::render(void){
	if(!update)
		return;
	
	int x = 0;

	setColor(topbar_bg_color);
	clear();

	for(int j = 0; j < TOP_BAR_HEIGHT; j++){
		float perc = (float) abs(j - TOP_BAR_HEIGHT / 2) / (TOP_BAR_HEIGHT / 2);

		SDL_Color tmp = DrawUtil::Blend(topbar_div_color, topbar_bg_color, perc);
		setColor(tmp);
		drawLine(0, j, TOP_BAR_WIDTH, j);
	}

	for(const auto& key : keys){
		DrawUtil::DrawButton(
				this,
				surface,
				key,
				x,
				0,
				selected == key
				);

		x += DrawUtil::ButtonSize(key);
	}
}

void TopBarCanvas::handleInput(Context *context){
	int x, y, button;
	if(!context->wasM1Pressed())
		return;

	context->getMouseXY(&x, &y);

	x -= rect.x;
	y -= rect.y;

	if(x < 0 || y < 0 || x > TOP_BAR_WIDTH || y > TOP_BAR_HEIGHT)
		return;

	button = 0;

	for(const auto& key : keys){
		button += DrawUtil::ButtonSize(key);

		if(x < button){
			if(selected != key){
				update = true;
				selected = key;
			}
			break;
		}
	}
}
