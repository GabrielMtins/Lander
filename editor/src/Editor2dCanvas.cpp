#include "Editor2dCanvas.hpp"
#include <algorithm>
#include <iostream>
#include "EditorDef.hpp"

Editor2dCanvas::Editor2dCanvas(SDL_Surface *surface, World *world, OutputCanvas *output_canvas) : Canvas(VIEW_CANVAS_X, VIEW_CANVAS_Y, VIEW_CANVAS_WIDTH, VIEW_CANVAS_HEIGHT, surface) {
	this->world = world;
	this->output_canvas = output_canvas;

	tools["add_sector"] = (Tool *) new AddSectorTool(this, world);
	tools["select_sector"] = (Tool *) new SelectSectorTool(this, world);
	tools["select_wall"] = (Tool *) new SelectWallTool(this, world);
	current_tool = tools["select_sector"];

	grid_per_unit = 4;
}

void Editor2dCanvas::render(void){
	setColor(0x00, 0x00, 0x00, 0xff);
	clear();

	setColor(0x20, 0x20, 0x20, 0xff);

	for(int i = 0; i < VIEW_CANVAS_WIDTH; i += GRID_SIZE){
		drawLine(i, 0, i, VIEW_CANVAS_HEIGHT);
	}

	for(int i = 0; i < VIEW_CANVAS_HEIGHT; i += GRID_SIZE){
		drawLine(0, i, VIEW_CANVAS_WIDTH, i);
	}

	setColor(0x00, 0x00, 0xff, 0xff);

	for(const auto& i : world->walls){
		if(i.second.portal == -1)
			setColor(0x00, 0x00, 0xff, 0xff);
		else
			setColor(0x00, 0xff, 0x00, 0xff);

		int x1, y1, x2, y2;

		worldToGrid(world->positions[i.second.start], &x1, &y1);
		worldToGrid(world->positions[i.second.end], &x2, &y2);
		drawLine(x1, y1, x2, y2);
	}

	setColor(0xff, 0x00, 0x00, 0xff);
	for(const auto &i : world->positions){
		int x, y;

		worldToGrid(i.second, &x, &y);
		fillRect(x - 1, y - 1, 3, 3);
		putPixel(x, y);
	}

	current_tool->render();
}

void Editor2dCanvas::worldToGrid(const Vec2 &position, int *x, int *y){
	*x = (position.x - offset.x) * GRID_SIZE * grid_per_unit + VIEW_CANVAS_WIDTH / 2;
	*y = (position.y - offset.y) * GRID_SIZE * grid_per_unit + VIEW_CANVAS_HEIGHT / 2;

}

void Editor2dCanvas::gridToWorld(int x, int y, Vec2 *position){
	position->x = (float) (x - VIEW_CANVAS_WIDTH / 2) / GRID_SIZE / grid_per_unit + offset.x;
	position->y = (float) (y - VIEW_CANVAS_HEIGHT / 2) / GRID_SIZE / grid_per_unit + offset.y;
}

void Editor2dCanvas::handleInput(Context *context){
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < SDL_NUM_SCANCODES; i++){
		if(!keys[i]){
			released[i] = true;
			continue;
		}

		if(!released[i])
			continue;

		released[i] = false;

		switch(i){
			case SDL_SCANCODE_B:
				current_tool = tools["add_sector"];
				break;

			case SDL_SCANCODE_V:
				current_tool = tools["select_sector"];
				break;

			case SDL_SCANCODE_T:
				current_tool = tools["select_wall"];
				break;

			case SDL_SCANCODE_0:
				grid_per_unit = DEFAULT_GRID_PER_UNIT;
				break;

			case SDL_SCANCODE_MINUS:
				grid_per_unit /= 2;
				break;

			case SDL_SCANCODE_EQUALS:
				grid_per_unit *= 2;
				break;

			case SDL_SCANCODE_W:
				offset.y -= 1.0f;
				break;

			case SDL_SCANCODE_S:
				offset.y += 1.0f;
				break;

			case SDL_SCANCODE_A:
				offset.x -= 1.0f;
				break;

			case SDL_SCANCODE_D:
				offset.x += 1.0f;
				break;
		}
	}

	if(grid_per_unit > MAX_GRID_PER_UNIT)
		grid_per_unit = MAX_GRID_PER_UNIT;
	if(grid_per_unit < DEFAULT_GRID_PER_UNIT)
		grid_per_unit = DEFAULT_GRID_PER_UNIT;

	current_tool->handleInput(context);
}

OutputCanvas * Editor2dCanvas::getOutputCanvas(void){
	return output_canvas;
}

AddSectorTool::AddSectorTool(Canvas *parent, World *world) : Tool(parent){
	this->world = world;
	state = WAITING_FOR_SECTOR;
}

void AddSectorTool::handleInput(Context *context){
	auto editor = (Editor2dCanvas *) parent;
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	if(keys[SDL_SCANCODE_ESCAPE]){
		positions.clear();
		state = WAITING_FOR_SECTOR;
	}
	
	int x, y, index;
	Vec2 position;

	context->getMouseXY(&x, &y);

	if(!parent->inBounds(x, y))
		return;

	parent->setOffset(&x, &y);

	mpos_x = ((x + GRID_SIZE / 2) / GRID_SIZE) * GRID_SIZE;
	mpos_y = ((y + GRID_SIZE / 2) / GRID_SIZE) * GRID_SIZE;

	editor->gridToWorld(x, y, &position);
	index = world->findClosestPoint(position, 0.001f);

	if(index == -1){
		editor->gridToWorld(
				mpos_x,
				mpos_y,
				&position
				);
	}
	else{
		position = world->positions[index];
		editor->worldToGrid(position, &mpos_x, &mpos_y);
	}

	if(!context->wasM1Pressed())
		return;

	if(positions.size() > 0 && positions[0] == position){
		tryAddSector();
		state = WAITING_FOR_SECTOR;
	}
	else{
		positions.push_back(position);
		state = SECTOR_CREATING;
	}
}

void AddSectorTool::render(void){
	auto editor = (Editor2dCanvas *) parent;

	editor->setColor(0xff, 0x00, 0x00, 0xff);

	for(const auto& v : positions){
		int x, y;
		editor->worldToGrid(v, &x, &y);
		editor->fillRect(x - 1, y - 1, 3, 3);
	}

	//if(state == SECTOR_CREATING){
		editor->setColor(0xff, 0xff, 0x00, 0xff);
		editor->fillRect(mpos_x - 1, mpos_y - 1, 3, 3);
	//}

	if(positions.size() == 0)
		return;

	for(size_t i = 0; i < positions.size() - 1; i++){
		int x1, y1, x2, y2;

		editor->setColor(0x00, 0xff, 0xff, 0xff);
		editor->worldToGrid(positions[i], &x1, &y1);
		editor->worldToGrid(positions[(i + 1) % positions.size()], &x2, &y2);
		editor->drawLine(x1, y1, x2, y2);

		editor->setColor(0xff, 0x00, 0x00, 0xff);
		editor->putPixel(x1, y1);
	}
}

void AddSectorTool::tryAddSector(void){
	if(isSectorClockwise()){
		std::reverse(positions.begin(), positions.end());
	}

	if(!isConvex()){
		positions.clear();
		return;
	}

	World old_state = *world;

	if(!world->tryAddSector(positions)){
		*world = old_state;
	}

	positions.clear();
}

bool AddSectorTool::isSectorClockwise(void){
	float area = 0.0f;
	int n = positions.size();

	for (int i = 0; i < n; ++i) {
		const Vec2& a = positions[i];
		const Vec2& b = positions[(i + 1) % n];
		area += (a.x * b.y) - (b.x * a.y);
	}

	return area < 0.0f;
}

bool AddSectorTool::isConvex(void) {
	int n = positions.size();
	if (n < 3) return false;

	int sign = 0;

	for (int i = 0; i < n; ++i) {
		const Vec2& a = positions[i];
		const Vec2& b = positions[(i + 1) % n];
		const Vec2& c = positions[(i + 2) % n];

		float dx1 = b.x - a.x;
		float dy1 = b.y - a.y;
		float dx2 = c.x - b.x;
		float dy2 = c.y - b.y;

		float cross = dx1 * dy2 - dy1 * dx2;

		if (cross != 0) {
			if (sign == 0)
				sign = (cross > 0) ? 1 : -1;
			else if ((cross > 0 && sign < 0) || (cross < 0 && sign > 0))
				return false; 
		}
	}

	return true;
}

SelectSectorTool::SelectSectorTool(Canvas *parent, World *world) : Tool(parent){
	this->world = world;
	sector_id = -1;
}

void SelectSectorTool::handleInput(Context *context){
	auto editor = (Editor2dCanvas *) parent;
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	if(sector_id != -1 && keys[SDL_SCANCODE_X]){
		world->deleteSector(sector_id);
	}
	
	int x, y;
	Vec2 position;

	context->getMouseXY(&x, &y);

	if(!parent->inBounds(x, y))
		return;

	parent->setOffset(&x, &y);

	if(!context->wasM1Pressed())
		return;

	editor->gridToWorld(
			x,
			y,
			&position
			);

	sector_id = -1;

	for(const auto& [key, _] : world->sectors){
		if(world->isPointInsideSector(position, key)){
			sector_id = key;
			break;
		}
	}
}

void SelectSectorTool::render(void){
	if(sector_id == -1)
		return;

	auto editor = (Editor2dCanvas *) parent;

	Sector& sector = world->sectors[sector_id];

	for(const int& i : sector.wall_indices){
		Wall& wall = world->walls[i];
		int x1, y1, x2, y2;

		editor->setColor(0x00, 0xff, 0xff, 0xff);
		editor->worldToGrid(world->positions[wall.start], &x1, &y1);
		editor->worldToGrid(world->positions[wall.end], &x2, &y2);
		editor->drawLine(x1, y1, x2, y2);

		editor->setColor(0xff, 0x00, 0x00, 0xff);
		editor->putPixel(x1, y1);
		editor->putPixel(x2, y2);
	}
}

SelectWallTool::SelectWallTool(Canvas *parent, World *world) : Tool(parent){
	this->world = world;
	wall_id = -1;
	state = SELECT_WALL;
}

void SelectWallTool::handleInput(Context *context){
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	if(keys[SDL_SCANCODE_G] && wall_id != -1){
		state = DIVIDE_WALL;
	}

	if(keys[SDL_SCANCODE_ESCAPE]){
		wall_id = -1;
		state = SELECT_WALL;
	}

	if(!context->wasM1Pressed())
		return;

	switch(state){
		case DIVIDE_WALL:
			divideWall(context);
			break;

		case SELECT_WALL:
			selectWall(context);
			break;
	}
}

void SelectWallTool::render(void){
	if(wall_id == -1)
		return;

	auto editor = (Editor2dCanvas *) parent;

	const Vec2& a = world->positions[world->walls[wall_id].start];
	const Vec2& b = world->positions[world->walls[wall_id].end];

	int x1, y1, x2, y2;

	editor->worldToGrid(a, &x1, &y1);
	editor->worldToGrid(b, &x2, &y2);

	editor->setColor(0x00, 0xff, 0xff, 0xff);
	editor->drawLine(x1, y1, x2, y2);
}

void SelectWallTool::selectWall(Context *context){
	auto editor = (Editor2dCanvas *) parent;
	int x, y;
	Vec2 position;

	context->getMouseXY(&x, &y);

	if(!parent->inBounds(x, y))
		return;

	parent->setOffset(&x, &y);

	editor->gridToWorld(x, y, &position);

	wall_id = world->findClosestWall(position);
}

void SelectWallTool::divideWall(Context *context){
	auto editor = (Editor2dCanvas *) parent;
	int x, y;
	Vec2 position;

	context->getMouseXY(&x, &y);

	if(!parent->inBounds(x, y))
		return;

	parent->setOffset(&x, &y);

	x = ((x + GRID_SIZE / 2) / GRID_SIZE) * GRID_SIZE;
	y = ((y + GRID_SIZE / 2) / GRID_SIZE) * GRID_SIZE;

	editor->gridToWorld(x, y, &position);

	world->divideWallEx(position, wall_id);
}
