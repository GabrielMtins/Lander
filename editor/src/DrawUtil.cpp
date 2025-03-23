#include "DrawUtil.hpp"
#include "EditorDef.hpp"

void DrawUtil::DrawText(Canvas *canvas, SDL_Surface *text_texture, const std::string &text, int x, int y){
	for(char c : text){
		c = tolower(c);

		if(c >= 'a' && c <= 'z'){
			SDL_Rect src = {((int) (tolower(c) - 'a')) * FONT_WIDTH, 0, FONT_WIDTH, FONT_HEIGHT};
			canvas->copySurface(text_texture, &src, x, y);
		}

		x += FONT_WIDTH;
	}
}

void DrawUtil::DrawButton(Canvas *canvas, SDL_Surface *text_surface, const std::string &text, int x, int y, int highlight){
	canvas->setColor(highlight ? button_fg_dark_color : button_bg_color);

	canvas->fillRect(
			x + BUTTON_PADDING_LEFT,
			y + BUTTON_PADDING_UP,
			TextSize(text),
			2 * FONT_HEIGHT
			);

	canvas->setColor(highlight ? button_fg_dark_color : button_fg_normal_color);

	canvas->fillRect(
			x + BUTTON_PADDING_LEFT,
			y + BUTTON_PADDING_UP,
			TextSize(text),
			1
			);

	canvas->fillRect(
			x + BUTTON_PADDING_LEFT,
			y + BUTTON_PADDING_UP,
			1,
			2 * FONT_HEIGHT
			);

	canvas->setColor(highlight ? button_fg_normal_color : button_fg_dark_color);

	canvas->fillRect(
			x + BUTTON_PADDING_LEFT,
			y + BUTTON_PADDING_UP + 2 * FONT_HEIGHT - 1,
			TextSize(text),
			1
			);

	canvas->fillRect(
			x + BUTTON_PADDING_LEFT + TextSize(text) - 1,
			y + BUTTON_PADDING_UP,
			1,
			2 * FONT_HEIGHT
			);

	DrawText(
			canvas,
			text_surface,
			text, x + BUTTON_PADDING_LEFT + FONT_WIDTH / 2,
			y + FONT_HEIGHT / 2 + BUTTON_PADDING_UP
			);
}

int DrawUtil::TextSize(const std::string &text){
	return text.size() * FONT_WIDTH + FONT_WIDTH;
}

int DrawUtil::ButtonSize(const std::string &text){
	return TextSize(text) + BUTTON_PADDING_LEFT + BUTTON_PADDING_RIGHT;
}

SDL_Color DrawUtil::Blend(const SDL_Color& a, const SDL_Color &b, float blend){
	SDL_Color ret = {
		(uint8_t) (a.r * blend + b.r * (1.0f - blend)),
		(uint8_t) (a.g * blend + b.g * (1.0f - blend)),
		(uint8_t) (a.b * blend + b.b * (1.0f - blend)),
		(uint8_t) (a.a * blend + b.a * (1.0f - blend))
	};

	return ret;
}
