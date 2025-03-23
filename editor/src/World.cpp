#include "World.hpp"
#include <cstdio>

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

bool Vec2::operator==(const Vec2& other) const{
	return other.x == x && other.y == y;
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
	for(auto& par : positions){
		if(par.second == position){
			par.second.count++;
			return par.first;
		}
	}

	positions[positions_id] = position;

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
	int next_sector = sectors.size();

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

	for(auto& par : walls){
		if(par.second.parent_sector == id){
			to_erase.push_back(par.first);

			/* delete references */
			positions[par.second.start].count--;
			positions[par.second.end].count--;
		}

		if(par.second.portal == id)
			par.second.portal = -1;
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

	return true;
}
