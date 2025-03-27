#include "Editor3dCanvas.hpp"
#include "EditorDef.hpp"
#include <algorithm>

Editor3dCanvas::Editor3dCanvas(SDL_Surface *surface, World *world) : Canvas(VIEW_CANVAS_X, VIEW_CANVAS_Y, VIEW_CANVAS_WIDTH, VIEW_CANVAS_HEIGHT, surface){
	this->world = world;
	camera = Vec3(0.0f, 0.5f, 0.0f);
	angle_x = 0.0f;
	angle_y = 0.0f;
}

void Editor3dCanvas::handleInput(Context *context){
	(void) context;
}

void Editor3dCanvas::render(void){
	setColor(0x00, 0x00, 0x00, 0xff);
	clear();

	setColor(0xff, 0xff, 0xff, 0xff);

	for(const auto& [_, wall] : world->walls){
		Vec3 start_point(world->positions[wall.start]);
		Vec3 end_point(world->positions[wall.end]);
		start_point.y = end_point.y = world->sectors[wall.parent_sector].bottom.height;
		renderLine(start_point, end_point);
		start_point.y = end_point.y = world->sectors[wall.parent_sector].top.height;
		renderLine(start_point, end_point);

		start_point = end_point;

		start_point.y = world->sectors[wall.parent_sector].bottom.height;
		end_point.y = world->sectors[wall.parent_sector].top.height;
		renderLine(start_point, end_point);
	}
}

void Editor3dCanvas::project(const Vec3 &point, int *x, int *y){
	*x = (point.x / point.z) * rect.h + rect.w / 2;
	*y = (-point.y / point.z) * rect.h + rect.h / 2;
}

Vec3 Editor3dCanvas::applyTransform(const Vec3 &point){
	return (point - camera).rotateY(-angle_y).rotateX(-angle_x);
}

void Editor3dCanvas::renderLine(const Vec3& start, const Vec3& end){
	int x1, y1, x2, y2;
	Vec3 new_start = applyTransform(start);
	Vec3 new_end = applyTransform(end);
	Vec3 inter;

	if(new_end.z < 0.0f && new_start.z < 0.0f)
		return;

	if(new_end.z < new_start.z){
		std::swap(new_end, new_start);
	}

	if(planeIntersection(new_start, new_end - new_start, Vec3(0.0f, 0.0f, 1.0f), 0.1f, &inter)){
		new_start = inter;
	}

	project(new_start, &x1, &y1);
	project(new_end, &x2, &y2);

	drawLine(x1, y1, x2, y2);
}

bool Editor3dCanvas::planeIntersection(const Vec3& start, const Vec3& direction, const Vec3& normal, float distance, Vec3 *intersection){
	float u;

	if(Vec3::dot(direction, normal) == 0)
		return false;

	u = (distance - Vec3::dot(start, normal)) / Vec3::dot(direction, normal);

	if(u < 0)
		return false;

	if(intersection != NULL)
		*intersection = start + direction * u;

	return true;
}
