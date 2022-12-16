#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class shooting_system : public system
		{
		public:
			shooting_system(world& context) : system(context)
			{
				set_signature<Weapon, Shooting, Position, Bounds>();
			}

			void run(float dt) override
			{
				for ( auto& shooter : get_managed_entities())
				{
					auto [pos, weapon, bounds] = world_context.get_components<Position, Weapon, Bounds>(shooter);

					const static auto sprite_index = SpriteManager::sprite_range_table[SpriteManager::BULLET_SPRITE_INDEX];

					const auto& bullet_rect = SpriteManager::sprite_table[sprite_index];

					const int bounds_w = bullet_rect.w * config::render_scale;
					const int bounds_h = bullet_rect.h * config::render_scale;

					const float shoot_offset_x = pos.x + (bounds_w / 2) - (bullet_rect.w / 2);
					const float shoot_offset_y = pos.y - (bounds.h / 2) - (bullet_rect.h / 2);

					const auto bullet = world_context.create_entity();

					world_context.add_component<Sprite>		(bullet, sprite_index);
					world_context.add_component<Bullet>		(bullet, weapon.shoot_direction);
					world_context.add_component<Position>	(bullet, shoot_offset_x, shoot_offset_y);
					world_context.add_component<Bounds>		(bullet, bounds_w, bounds_h);
					world_context.add_component<Damaging>	(bullet, config::bullet_damage);

					world_context.remove_component<Shooting>(shooter);
				}
			}
		};
	}
}
