#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class enemy_spawner_system : public system
		{
			unsigned int enemies_per_wave;
			float wave_timer;
			float time_remaining;
			std::mt19937 rng;

		public:
			enemy_spawner_system(ecs::world& context,
				const unsigned int num_enemies_per_wave,
				float time_between_waves,
				const std::mt19937& generator) :
			system(context)
			{
				enemies_per_wave = num_enemies_per_wave;
				wave_timer = time_between_waves;
				time_remaining = wave_timer;
				rng = generator;
			}

			void spawn_enemies()
			{
				printf("spawning enemy wave \n");

				// we want them to spawn "above" the screen, hence the negative y distribution
				std::uniform_int_distribution<> x_pos_dist(0, config::desktop_width);
				std::uniform_int_distribution<> y_pos_dist(-200, -50);


				for (unsigned int i = 0; i < enemies_per_wave; i++)
				{
					int pos_x = x_pos_dist(rng);
					int pos_y = y_pos_dist(rng);

					int sprite_ind = SpriteManager::sprite_range_table[SpriteManager::ENEMY_SPRITE_RANGE_INDEX] - 1;

					SDL_Rect enemy_rect = SpriteManager::sprite_table[sprite_ind];

					const int bounds_w = enemy_rect.w * config::render_scale;
					const int bounds_h = enemy_rect.h * config::render_scale;

					const int x_width = pos_x + bounds_w;
					const int y_height = pos_y + bounds_h;

					if (x_width > config::desktop_width)
					{
						pos_x -= enemy_rect.w * config::render_scale;
					}
					if (y_height > config::desktop_height)
					{
						pos_y -= enemy_rect.h * config::render_scale;
					}

					const auto enemy = world_context.create_entity();

					world_context.add_component<Sprite>(enemy, sprite_ind);
					world_context.add_component<Position>(enemy, pos_x, pos_y);
					world_context.add_component<Bounds>(enemy, enemy_rect.w, enemy_rect.h);
					world_context.add_component<Weapon>(enemy, config::bullet_damage, 1);
					world_context.add_component<Enemy>(enemy);
					world_context.add_component<Alive>(enemy);
				}
			}

			void run(float dt) override
			{
				time_remaining -= dt;
				if (time_remaining < 0)
				{
					spawn_enemies();
					time_remaining = wave_timer;
				}
			}
		};
	}
}
