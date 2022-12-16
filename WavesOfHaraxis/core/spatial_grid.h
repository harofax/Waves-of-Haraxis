#pragma once
#include <unordered_map>
#include <set>
#include <vector>

#include "entity.h"

namespace ecs
{
	class world;

	constexpr float GRID_PARTITION_SIZE = 50.0f;
	const int GRID_SIZE = 50;
}

struct GridUnit
{
	std::set<ecs::entity> entities;

	std::set<ecs::entity> get_entities()
	{
		return entities;
	}
};



class spatial_grid
{
	friend ecs::world;

public:
	spatial_grid() : grid(ecs::GRID_SIZE, std::vector<GridUnit>(ecs::GRID_SIZE))
	{
	}

	int map_to_grid(float coord)
	{
		if (coord < 0)
		{
			return floor(coord / ecs::GRID_PARTITION_SIZE) * -1;
		}
		else
		{
			return floor(coord / ecs::GRID_PARTITION_SIZE);
		}
	}

	void add_entity(ecs::entity entity, float pos_x, float pos_y)
	{
		// offset to allow for off-screen enemy waves
		//pos_x += ecs::GRID_SIZE / 2;
		//pos_y += ecs::GRID_SIZE / 2;

		const int grid_x = map_to_grid(pos_x);
		const int grid_y = map_to_grid(pos_y);

		printf("adding to grid [%d, %d]\n", grid_x, grid_y);

		grid[grid_x][grid_y].entities.insert(entity);
	}

	void remove_entity(ecs::entity entity, float pos_x, float pos_y)
	{
		//pos_x += ecs::GRID_SIZE / 4;
		//pos_y += ecs::GRID_SIZE / 2;

		const int grid_x = map_to_grid(pos_x);
		const int grid_y = map_to_grid(pos_y);

		grid[grid_x][grid_y].entities.erase(entity);
	}

	std::set<ecs::entity> get_entities_at(float pos_x, float pos_y)
	{
		const int grid_x = map_to_grid(pos_x);
		const int grid_y = map_to_grid(pos_y);

		return grid[grid_x][grid_y].entities;
	}

	std::vector<std::vector<GridUnit>> grid{};
};
