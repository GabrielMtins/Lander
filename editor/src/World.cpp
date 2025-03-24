#include "World.hpp"
#include <cstdio>
#include <cmath>
#include <algorithm>

Vec2::Vec2(void){
	this->x = 0.0f;
	this->y = 0.0f;
	count = 1;
}

Vec2::Vec2(float x, float y){
	this->x = x;
	this->y = y;
	count = 1;
}

bool Vec2::operator==(const Vec2& other) const {
	return other.x == x && other.y == y;
}

Vec2 Vec2::operator+(const Vec2& other) const {
	return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
	return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float t) const {
	return Vec2(x * t, y * t);
}

Vec2 Vec2::perpendicular(void) const {
	return Vec2(-y, x);
}

Vec2 Vec2::normalize(void) const {
	float size = sqrt(dot(*this));
	 
	return Vec2(x / size, y / size);
}

float Vec2::dot(const Vec2& other) const {
	return x * other.x + y * other.y;
}

Wall::Wall(void){
	start = 0;
	end = 0;
	portal = -1;
}

Wall::Wall(int start, int end){
	this->start = start;
	this->end = end;
	portal = -1;
}

Sector::Sector(void){
	bottom_height = 0.0f;
	top_height = 0.0f;
}

float Sector::signedArea(World *world){
	float signed_area = 0.0f;

	for(int i : wall_indices){
		const Wall& wall = world->walls[i];
		const Vec2& start = world->positions[wall.start];
		const Vec2& end = world->positions[wall.end];

		signed_area += (start.x * end.y) - (end.x * start.y);
	}

	return signed_area < 0.0f;
}

int World::tryAddPosition(const Vec2 &position){
	for(auto& [index, value] : positions){
		if(value == position){
			value.count++;
			return index;
		}
	}

	positions[positions_id] = position;
	positions[positions_id].count = 1;

	return positions_id++;
}

int World::tryAddWall(int start, int end, int sector){
	Wall added_wall(start, end);
	added_wall.parent_sector = sector;

	for(auto& par : walls){
		auto& wall = par.second;

		if(wall.start == start && wall.end == end)
			return -1;

		if(wall.start == end && wall.end == start){
			wall.portal = sector;
			added_wall.portal = wall.parent_sector;
		}
	}

	walls[walls_id] = added_wall;

	return walls_id++;
}

bool World::tryAddSector(const std::vector<Vec2>& vertices){
	Sector sector;

	size_t n = vertices.size();
	int next_sector = sectors_id;

	for(size_t i = 0; i < n; i++){
		int start_wall, end_wall;
		int added_wall;

		start_wall = tryAddPosition(vertices[i]);
		end_wall = tryAddPosition(vertices[(i + 1) % n]);

		added_wall = tryAddWall(start_wall, end_wall, next_sector);

		if(added_wall == -1){
			return false;
		}

		sector.wall_indices.push_back(added_wall);
	}
	
	sectors[sectors_id++] = sector;

	return true;
}

bool World::deleteSector(int id){
	if(sectors.find(id) == sectors.end())
		return false;

	std::vector<int> to_erase;

	for(auto& [index, wall] : walls){
		//printf("Hi: ");
		if(wall.parent_sector == id){
			to_erase.push_back(index);

			/* delete references */
			positions[wall.start].count--;
			positions[wall.end].count--;

			//printf("%i %i", positions[wall.start].count, positions[wall.end].count);
		}
		//printf("\n");

		if(wall.portal == id)
			wall.portal = -1;
	}
	
	for(const auto& i : to_erase){
		if(positions.find(walls[i].start) != positions.end()){
			if(positions[walls[i].start].count <= 0)
				positions.erase(walls[i].start);
		}

		if(positions.find(walls[i].end) != positions.end()){
			if(positions[walls[i].end].count <= 0)
				positions.erase(walls[i].end);
		}

		walls.erase(i);
	}

	sectors.erase(id);

	//printf("%lu\n\n", sectors.size());

	return true;
}

bool World::isPointInsideSector(const Vec2& position, int id){
	if(sectors.find(id) == sectors.end())
		return false;

	auto& sector = sectors[id];

	if(sector.wall_indices.size() < 3)
		return false;

	int sign = 0;

	for(const int& i : sector.wall_indices){
		const Vec2& a = positions[walls[i].start];
		const Vec2& b = positions[walls[i].end];

		float dx1 = b.x - a.x;
		float dy1 = b.y - a.y;
		float dx2 = position.x - a.x;
		float dy2 = position.y - a.y;

		float cross = dx1 * dy2 - dy1 * dx2;

		if (cross != 0) {
			int currentSign = (cross > 0) ? 1 : -1;
			if (sign == 0)
				sign = currentSign;
			else if (sign != currentSign)
				return false; 
		}
	}

	return true;
}

int World::findClosestPoint(const Vec2& position, float radius){
	float dx, dy;

	for(const auto& [index, point] : positions){
		dx = point.x - position.x;
		dy = point.y - position.y;

		if(dx * dx + dy * dy < radius)
			return index;
	}

	return -1;
}

int World::findClosestWall(const Vec2& position){
	float closest_distance = 999999.0f;
	int found_index = -1;

	for(const auto& [index, wall] : walls){
		Vec2& a = positions[wall.start];
		Vec2& b = positions[wall.end];
		Vec2 diff = b - a;

		if((position - a).dot(position - b) > 0){
			continue;
		}

		float dist = fabsf((position - a).dot(diff.perpendicular().normalize()));

		if(dist < closest_distance){
			found_index = index;
			closest_distance = dist;
		}
	}

	return found_index;
}

bool World::divideWall(const Vec2& position, int wall_id){
	if(walls.find(wall_id) == walls.end())
		return false;

	Wall& wall = walls[wall_id];
	int index_new_point;
	int new_wall;
	int old_wall_end;

	old_wall_end = wall.end;

	index_new_point = tryAddPosition(position);

	wall.end = index_new_point;

	new_wall = tryAddWall(index_new_point, old_wall_end, wall.parent_sector);
	walls[new_wall].portal = wall.portal;

	positions[index_new_point].count++;

	auto& wall_indices = sectors[wall.parent_sector].wall_indices;

	for(size_t i = 0; i < wall_indices.size(); i++){
		if(wall_indices[i] == wall_id){
			wall_indices.insert(std::next(wall_indices.begin(), i + 1), new_wall);
			break;
		}
	}

	return true;
}

bool World::divideWallEx(const Vec2& position, int wall_id){
	if(walls.find(wall_id) == walls.end())
		return false;

	Wall& wall = walls[wall_id];
	Vec2& a = positions[wall.start];
	Vec2& b = positions[wall.end];
	Vec2 diff = b - a;
	Vec2 new_point;
	float projection;

	int old_end = wall.end;
	int old_start = wall.start;

	projection = (position - a).dot(diff) / diff.dot(diff);

	if(projection <= 0.0f || projection >= 1.0f)
		return false;

	new_point = diff * projection + a;

	divideWall(new_point, wall_id);

	if(wall.portal != -1){
		for(const auto& [index, value] : walls){
			if(old_start == value.end && old_end == value.start){
				divideWall(new_point, index);
				break;
			}
		}
	}

	return true;
}

bool World::divideSector(int sector_id, int position1_id, int position2_id){
	std::vector<int> position_indices;
	std::vector<Vec2> sector1;
	std::vector<Vec2> sector2;
	size_t index_rotate;
	bool change = false;

	if(sectors.find(sector_id) == sectors.end())
		return false;

	if(positions.find(position1_id) == positions.end())
		return false;

	if(positions.find(position2_id) == positions.end())
		return false;

	{
		const Sector& sector = sectors[sector_id];

		for(const int& i : sector.wall_indices){
			if(walls[i].start == position1_id)
				index_rotate = position_indices.size();

			position_indices.push_back(walls[i].start);
		}
	}

	std::rotate(position_indices.begin(), position_indices.begin() + index_rotate, position_indices.end());

	for(const int& i : position_indices){
		if(i == position2_id){
			change = true;
		}
		
		if(!change){
			sector1.push_back(positions[i]);
		}
		else{
			sector2.push_back(positions[i]);
		}
	}

	sector1.push_back(positions[position2_id]);
	sector2.push_back(positions[position1_id]);

	deleteSector(sector_id);
	
	tryAddSector(sector1);
	tryAddSector(sector2);

	return true;
}
