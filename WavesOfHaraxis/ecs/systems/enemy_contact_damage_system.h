#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class enemy_contact_damage_system : public system
		{
		public:
			enemy_contact_damage_system(world& context) : system(context)
			{
				set_signature<Enemy, Damaging>();
			}

			void run(float dt) override
			{
				for (auto& enemy : get_managed_entities())
				{
					const auto& enemy_pos = world_context.get_component<Position>(enemy);
					const auto& enemy_bounds = world_context.get_component<Bounds>(enemy);

					auto nearby = world_context.grid.get_entities_at(enemy_pos.x, enemy_pos.y).get_entities();

					for (const ecs::entity player : nearby)
					{
						if (!world_context.has_component<PlayerInput>(player)) continue;

						const auto& player_pos = world_context.get_component<Position>(player);
						const auto& player_bounds = world_context.get_component<Bounds>(player);


						if (enemy_pos.x < player_pos.x + player_bounds.w &&
							enemy_pos.x + enemy_bounds.w > player_pos.x &&
							enemy_pos.y < player_pos.y + player_bounds.h &&
							enemy_bounds.h + enemy_pos.y > player_pos.y)
						{
							auto& player_health = world_context.get_component<Health>(player);
							const auto& dmg = world_context.get_component<Damaging>(enemy).damage;
							player_health.health -= dmg;
							world_context.add_component<ToBeRemoved>(enemy);
						}

					}
				}
			}
		};
	}
}
