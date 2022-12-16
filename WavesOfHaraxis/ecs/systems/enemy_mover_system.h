#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class enemy_mover_system : public system<COMPONENT_CAPACITY, SYSTEM_CAPACITY>
		{
		public:
			enemy_mover_system(world<COMPONENT_CAPACITY, SYSTEM_CAPACITY>& context ) : system(context)
			{
				set_signature<Enemy, Position, Alive>();
			}

			void run(float dt) override
			{
				for ( auto& enemy : get_managed_entities())
				{
					world_context.add_component<Velocity>(enemy, 0, config::enemy_base_speed * dt);
				}
			}
		};
		
	}
}
