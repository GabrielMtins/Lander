#include "OutputCanvas.hpp"
#include "DrawUtil.hpp"
#include "EditorDef.hpp"

OutputCanvas::OutputCanvas(SDL_Surface *surface, SDL_Surface *text_texture) : Canvas(ERROR_BAR_X, ERROR_BAR_Y, ERROR_BAR_WIDTH, ERROR_BAR_HEIGHT, surface){
	this->text_texture = text_texture;
}

void OutputCanvas::render(void){
	setColor(0x00, 0x00, 0x00, 0xff);
	clear();

	DrawUtil::DrawText(this, text_texture, msg, FONT_HEIGHT / 2, FONT_HEIGHT / 2);
}

void OutputCanvas::setMessage(const std::string &msg){
	this->msg = msg;
}
