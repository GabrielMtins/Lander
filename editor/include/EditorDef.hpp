#ifndef EDITOR_DEF_HPP
#define EDITOR_DEF_HPP

#include <SDL2/SDL.h>

#define WINDOW_WIDTH (270 * 16 / 9)
#define WINDOW_HEIGHT 270
#define WINDOW_SCALE 3

#define TOP_BAR_WIDTH (WINDOW_WIDTH)
#define TOP_BAR_HEIGHT (16)

#define TOP_BAR_SHADOW_X (5)
#define TOP_BAR_SHADOW_Y (TOP_BAR_HEIGHT)
#define TOP_BAR_SHADOW_WIDTH (WINDOW_WIDTH - TOP_BAR_SHADOW_X)
#define TOP_BAR_SHADOW_HEIGHT (4)

#define FILEBAR_X (0)
#define FILEBAR_Y (TOP_BAR_HEIGHT)
#define FILEBAR_WIDTH (WINDOW_WIDTH)
#define FILEBAR_HEIGHT (WINDOW_HEIGHT - TOP_BAR_HEIGHT)

#define ERROR_BAR_X (0)
#define ERROR_BAR_Y (WINDOW_HEIGHT - 10)
#define ERROR_BAR_WIDTH (WINDOW_WIDTH)
#define ERROR_BAR_HEIGHT (10)

#define NUM_GRIDS_X (VIEW_CANVAS_WIDTH / GRID_SIZE)
#define NUM_GRIDS_Y (VIEW_CANVAS_HEIGHT / GRID_SIZE)
#define GRID_SIZE 8
#define DEFAULT_GRID_PER_UNIT 4
#define MAX_GRID_PER_UNIT 16

#define FONT_WIDTH 6
#define FONT_HEIGHT 5

#define BUTTON_PADDING_LEFT 2
#define BUTTON_PADDING_RIGHT 2
#define BUTTON_PADDING_UP 3

#define VIEW_CANVAS_X (8)
#define VIEW_CANVAS_Y (TOP_BAR_HEIGHT + 8)
#define VIEW_CANVAS_WIDTH (WINDOW_WIDTH - 15)
#define VIEW_CANVAS_HEIGHT (WINDOW_HEIGHT - TOP_BAR_HEIGHT - 13)

extern const SDL_Color background_color;
extern const SDL_Color topbar_bg_color;
extern const SDL_Color topbar_div_color;
extern const SDL_Color topbar_shadow_color;

extern const SDL_Color button_bg_color;
extern const SDL_Color button_fg_normal_color;
extern const SDL_Color button_fg_dark_color;

extern const SDL_Color button_highlight_color;

#endif
