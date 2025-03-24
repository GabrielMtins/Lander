#ifndef EDITOR_2D_CANVAS_HPP
#define EDITOR_2D_CANVAS_HPP

#include "Canvas.hpp"
#include "World.hpp"
#include "Tool.hpp"
#include "OutputCanvas.hpp"

class Editor2dCanvas : public Canvas {
	public:
		Editor2dCanvas(SDL_Surface *surface, World *world, OutputCanvas *output_canvas);

		void render(void);
		void handleInput(Context *context);
		void worldToGrid(const Vec2 &position, int *x, int *y);
		void gridToWorld(int x, int y, Vec2 *position);

		OutputCanvas * getOutputCanvas(void);

	private:
		World *world;
		Vec2 offset;
		Tool *current_tool;
		std::unordered_map<std::string, Tool *> tools;
		OutputCanvas *output_canvas;
		int grid_per_unit;

		std::unordered_map<int, bool> released;

		std::vector<Vec2> positions;
};

class AddSectorTool : public Tool {
	public:
		AddSectorTool(Canvas *parent, World *world);
		void handleInput(Context *context);
		void render(void);

	private:
		void tryAddSector(void);
		bool isSectorClockwise(void);
		bool isConvex(void);

		enum State {
			WAITING_FOR_SECTOR = 0,
			SECTOR_CREATING
		};

		std::vector<Vec2> positions;
		State state;
		World *world;

		int mpos_x, mpos_y;
};

class SelectSectorTool : public Tool {
	public:
		SelectSectorTool(Canvas *parent, World *world);
		void handleInput(Context *context);
		void render(void);

	private:
		int sector_id;

		World *world;
};

class SelectWallTool : public Tool {
	public:
		SelectWallTool(Canvas *parent, World *world);
		void handleInput(Context *context);
		void render(void);

	private:
		void selectWall(Context *context);
		void divideWall(Context *context);

		enum State {
			DIVIDE_WALL,
			SELECT_WALL
		};

		int wall_id;
		State state;

		World *world;
};

#endif
