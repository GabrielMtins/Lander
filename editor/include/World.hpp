#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <unordered_map>
#include <string>

struct World;

struct Vec2 {
	float x, y;
	int count;

	Vec2(void);
	Vec2(float x, float y);
	bool operator==(const Vec2& other) const;
	Vec2 operator+(const Vec2& other) const;
	Vec2 operator-(const Vec2& other) const;
	Vec2 operator*(float t) const;
	Vec2 perpendicular(void) const;
	Vec2 normalize(void) const;

	float dot(const Vec2& other) const;
};

struct Wall {
	int start;
	int end;
	int portal;
	int parent_sector;

	Vec2 offset;
	Vec2 scale;
	int texture;

	Wall(void);
	Wall(int start, int end);
};

struct Sector {
	std::vector<int> wall_indices;

	struct {
		float height;
		Vec2 offset;
		Vec2 scale;
		int wall_step;
		float step;
		int texture;
	} bottom, top;

	Sector(void);
	float signedArea(World* world);
};

struct World {
	std::unordered_map<int, Sector> sectors;
	std::unordered_map<int, Wall> walls;
	std::unordered_map<int, Vec2> positions;
	std::unordered_map<int, int> id_to_order;

	int positions_id = 0;
	int walls_id = 0;
	int sectors_id = 0;

	World(void);

	int tryAddPosition(const Vec2 &position);
	int tryAddWall(int start, int end, int sector);
	int getNextSector(void);
	bool tryAddSector(const std::vector<Vec2>& vertices);
	bool deleteSector(int id);
	bool isPointInsideSector(const Vec2& position, int id);

	int findClosestPoint(const Vec2& position, float radius);
	int findClosestWall(const Vec2& position);
	bool divideWall(const Vec2& position, int wall_id);
	bool divideWallEx(const Vec2& position, int wall_id);
	bool divideSector(int sector_id, int position1_id, int position2_id);

	std::string exportMap(const Wall& wall);
	std::string exportMap(const Sector& sector);
	bool exportMap(const std::string& filename);

	/* TODO */
	bool loadMap(const std::string& filename);
};

#endif
