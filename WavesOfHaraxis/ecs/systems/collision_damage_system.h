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
			}

			void on_managed_entity_removed(entity entity) override
			{
				set_signature<Bullet>();
			}

			void run(float dt) override
			{
				//for (auto& bullet : get_managed_entities())
				//{
				//	auto& pos = world_context.get_component<Position>(bullet);
				//
				//	auto nearby = world_context.grid.get_entities_at(pos.x, pos.y);
				//}
				//
				//return;
				for (auto& grid_row : world_context.grid.grid)
				{
					for (auto& grid_unit : grid_row)
					{
						auto entities = grid_unit.get_entities();

						for (ecs::entity entity1 : entities)
						{
							//auto [e1_bounds, e1_pos] = world_context.get_components<Bounds, Position>(entity1);

							for (ecs::entity entity2 : entities)
							{
								const auto& e1_bounds = world_context.get_component<Bounds>(entity1);
								const auto& e1_pos = world_context.get_component<Position>(entity1);
								const bool e1_is_enemy = world_context.has_component<Enemy>(entity1);
								const bool e1_is_player = world_context.has_component<PlayerInput>(entity1);
								const bool e1_is_bullet = world_context.has_component<Bullet>(entity1);

								if (entity1 != entity2)
								{
									const auto& e2_bounds = world_context.get_component<Bounds>(entity2);
									const auto& e2_pos = world_context.get_component<Position>(entity2);

									if (e1_pos.x < e2_pos.x + e2_bounds.w &&
										e1_pos.x + e1_bounds.w > e2_pos.x &&
										e1_pos.y < e2_pos.y + e2_bounds.h &&
										e1_bounds.h + e1_pos.y > e2_pos.y)
									{
										world_context.add_component<ToBeRemoved>(entity1);
										world_context.add_component<ToBeRemoved>(entity2);
									}

								}
							}
						}

					}
				}

			}
		};
	}
}
