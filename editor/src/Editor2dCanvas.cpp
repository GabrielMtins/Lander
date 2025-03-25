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

	for(int i = 0; i < rect.w; i += GRID_SIZE){
		drawLine(i, 0, i, rect.h);
	}

	for(int i = 0; i < rect.h; i += GRID_SIZE){
		drawLine(0, i, rect.w, i);
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

	setColor(topbar_div_color);
	fillRect(0, 0, rect.w, 1);
	fillRect(0, 0, 1, rect.h);
	fillRect(0, rect.h - 1, rect.w, 1);
	fillRect(rect.w - 1, 0, 1, rect.h);
}

void Editor2dCanvas::worldToGrid(const Vec2 &position, int *x, int *y){
	*x = (position.x - offset.x) * GRID_SIZE * grid_per_unit + rect.w / 2;
	*y = (position.y - offset.y) * GRID_SIZE * grid_per_unit + rect.h / 2;

}

void Editor2dCanvas::gridToWorld(int x, int y, Vec2 *position){
	position->x = (float) (x - rect.w / 2) / GRID_SIZE / grid_per_unit + offset.x;
	position->y = (float) (y - rect.h / 2) / GRID_SIZE / grid_per_unit + offset.y;
}

void Editor2dCanvas::snapToGrid(int *x, int *y){
	*x = ((*x + GRID_SIZE / 2) / GRID_SIZE) * GRID_SIZE;
	*y = ((*y + GRID_SIZE / 2) / GRID_SIZE) * GRID_SIZE;
}

void Editor2dCanvas::selectTool(const std::string& tool_name){
	current_tool = tools[tool_name];
	current_tool->reset();
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
				selectTool("add_sector");
				break;

			case SDL_SCANCODE_V:
				selectTool("select_sector");
				break;

			case SDL_SCANCODE_T:
				selectTool("select_wall");
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
	reset();
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

	editor->gridToWorld(x, y, &position);
	index = world->findClosestPoint(position, 0.001f);

	if(index == -1){
		editor->snapToGrid(&x, &y);
		mpos_x = x;
		mpos_y = y;
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

	editor->setColor(0xff, 0xff, 0x00, 0xff);
	editor->fillRect(mpos_x - 1, mpos_y - 1, 3, 3);

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

	{
		int x, y;
		editor->setColor(0x00, 0xff, 0xff, 0xff);
		editor->worldToGrid(positions.back(), &x, &y);
		editor->drawLine(x, y, mpos_x, mpos_y);
	}
}

void AddSectorTool::reset(void){
	state = WAITING_FOR_SECTOR;
	positions.clear();
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
	reset();
}

void SelectSectorTool::handleInput(Context *context){
	switch(state){
		case SELECT_SECTOR:
			selectSector(context);
			break;

		case DIVIDE_SECTOR:
			divideSector(context);
			break;
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

	if(state == DIVIDE_SECTOR){
		if(chose_point_first != -1){
			int x, y;
			editor->worldToGrid(world->positions[chose_point_first], &x, &y);
			editor->setColor(0x00, 0xff, 0xff, 0xff);
			editor->drawLine(mpos_x, mpos_y, x, y);

			editor->setColor(0xff, 0xff, 0x00, 0xff);
			editor->fillRect(x - 1, y - 1, 3, 3);
		}

		editor->setColor(0xff, 0xff, 0x00, 0xff);
		editor->fillRect(mpos_x - 1, mpos_y - 1, 3, 3);
	}
}

void SelectSectorTool::reset(void){
	sector_id = -1;
	state = SELECT_SECTOR;
	chose_point_first = -1;
}

void SelectSectorTool::selectSector(Context *context){
	auto editor = (Editor2dCanvas *) parent;
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	if(sector_id != -1 && (keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_DELETE])){
		world->deleteSector(sector_id);
		sector_id = -1;
	}

	if(sector_id != -1 && keys[SDL_SCANCODE_X]){
		state = DIVIDE_SECTOR;
		chose_point_first = -1;
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

void SelectSectorTool::divideSector(Context *context){
	const uint8_t *keys = SDL_GetKeyboardState(NULL);
	auto editor = (Editor2dCanvas *) parent;

	if(keys[SDL_SCANCODE_ESCAPE])
		state = SELECT_SECTOR;

	int x, y;
	Vec2 position;

	context->getMouseXY(&x, &y);

	if(!parent->inBounds(x, y))
		return;

	parent->setOffset(&x, &y);

	editor->gridToWorld(x, y, &position);
	int closest = world->findClosestPoint(position, 1.0f / DEFAULT_GRID_PER_UNIT);

	if(closest == -1){
		mpos_x = x;
		mpos_y = y;
	}
	else{
		position = world->positions[closest];
		editor->worldToGrid(position, &mpos_x, &mpos_y);
	}

	if(!context->wasM1Pressed())
		return;

	if(chose_point_first == -1){
		chose_point_first = closest;
	}
	else{
		world->divideSector(sector_id, chose_point_first, closest);
		state = SELECT_SECTOR;
	}
}

SelectWallTool::SelectWallTool(Canvas *parent, World *world) : Tool(parent){
	this->world = world;
	reset();
}

void SelectWallTool::handleInput(Context *context){
	const uint8_t *keys = SDL_GetKeyboardState(NULL);

	if(keys[SDL_SCANCODE_X] && wall_id != -1){
		state = DIVIDE_WALL;
	}

	if(keys[SDL_SCANCODE_ESCAPE]){
		wall_id = -1;
		state = SELECT_WALL;
	}

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
	Vec2 diff = b - a;
	Vec2 position;
	float projection;

	editor->gridToWorld(mpos_x, mpos_y, &position);
	projection = (position - a).dot(diff) / diff.dot(diff);
	position = diff * (projection) + a;

	int x1, y1, x2, y2;

	editor->worldToGrid(a, &x1, &y1);
	editor->worldToGrid(b, &x2, &y2);

	editor->setColor(0x00, 0xff, 0xff, 0xff);
	editor->drawLine(x1, y1, x2, y2);

	if(state == DIVIDE_WALL && projection > 0.0f && projection < 1.0f){
		int x, y;
		editor->setColor(0xff, 0xff, 0x00, 0xff);
		editor->worldToGrid(position, &x, &y);
		editor->fillRect(x - 1, y - 1, 3, 3);
	}
}

void SelectWallTool::reset(void){
	wall_id = -1;
	state = SELECT_WALL;
}

void SelectWallTool::selectWall(Context *context){
	if(!context->wasM1Pressed())
		return;

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

	editor->snapToGrid(&x, &y);
	/*
	x = ((x + GRID_SIZE / 2) / GRID_SIZE) * GRID_SIZE;
	y = ((y + GRID_SIZE / 2) / GRID_SIZE) * GRID_SIZE;
	*/

	mpos_x = x;
	mpos_y = y;

	if(!context->wasM1Pressed())
		return;

	editor->gridToWorld(x, y, &position);

	world->divideWallEx(position, wall_id);
	state = SELECT_WALL;
	wall_id = -1;
}
