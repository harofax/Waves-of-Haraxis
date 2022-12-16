#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class collision_damage_system : public system
		{
		public:
			collision_damage_system(world& context) : system(context)
			{
				set_signature<Bullet>();
			}

			void run(float dt) override
			{
				for (auto& bullet : get_managed_entities())
				{
					const auto& bullet_pos = world_context.get_component<Position>(bullet);
					const auto& bullet_bounds = world_context.get_component<Bounds>(bullet);
				
					auto nearby = world_context.grid.get_entities_at(bullet_pos.x, bullet_pos.y).get_entities();

					for (const ecs::entity enemy : nearby)
					{
						if (!world_context.has_component<Enemy>(enemy)) continue;
				
						const auto& enemy_pos = world_context.get_component<Position>(enemy);
						const auto& enemy_bounds = world_context.get_component<Bounds>(enemy);
				
				
						if (bullet_pos.x < enemy_pos.x + enemy_bounds.w &&
							bullet_pos.x + bullet_bounds.w > enemy_pos.x &&
							bullet_pos.y < enemy_pos.y + enemy_bounds.h &&
							bullet_bounds.h + bullet_pos.y > enemy_pos.y)
						{
							auto& enemy_health = world_context.get_component<Health>(enemy);
							const auto& dmg = world_context.get_component<Damaging>(bullet).damage;
							enemy_health.health -= dmg;
							world_context.add_component<ToBeRemoved>(bullet);
						}
				
					}
				}
			}
		};
	}
}
