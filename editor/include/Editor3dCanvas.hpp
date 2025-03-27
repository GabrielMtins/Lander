#ifndef EDITOR_3D_CANVAS_HPP
#define EDITOR_3D_CANVAS_HPP

#include "Canvas.hpp"
#include "Vec3.hpp"
#include "World.hpp"

class Editor3dCanvas : public Canvas {
	public:
		Editor3dCanvas(SDL_Surface *surface, World *world);
		void handleInput(Context *context);
		void render(void);
	
	private:
		void project(const Vec3 &point, int *x, int *y);
		Vec3 applyTransform(const Vec3 &point);
		void renderLine(const Vec3& start, const Vec3& end);
		bool planeIntersection(const Vec3& start, const Vec3& direction, const Vec3& normal, float distance, Vec3 *intersection);
		bool drawSector(int sector_id);

		Vec3 camera;
		float angle_x, angle_y;
		World *world;
};

#endif
