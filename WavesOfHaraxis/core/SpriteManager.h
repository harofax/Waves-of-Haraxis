#pragma once
#include <SDL_rect.h>
#include <SDL_render.h>


class SpriteManager
{
public:
	// won't have more than 16 sprites
	static SDL_Rect sprite_table[16];

	// for mapping to sprite_range_table
	// 0 -> planet, 1 -> player, 2 -> enemy
	constexpr static int PLANET_SPRITE_RANGE_INDEX = 0;
	constexpr static int PLAYER_SPRITE_RANGE_INDEX = 1;
	constexpr static int ENEMY_SPRITE_RANGE_INDEX = 2;
	constexpr static int BULLET_SPRITE_INDEX = 3;

	// use preset constexpr statics depending on type
	static int sprite_range_table[4];

	// index of sprite you want to get
	static inline SDL_Rect get_sprite_atlas_rect(const int index)
	{
		return SpriteManager::sprite_table[index];
	};

	static SDL_Texture* atlas_texture;

private:
	SpriteManager() = default;
};

