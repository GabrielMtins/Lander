#include "Tool.hpp"

Tool::Tool(Canvas *parent){
	this->parent = parent;
}

void Tool::handleInput(Context *context){
	(void) context;
}

void Tool::render(void){
}

void Tool::reset(void){
}
