#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class bullet_system : public system<COMPONENT_CAPACITY, SYSTEM_CAPACITY>
		{
		public:
			bullet_system(world<COMPONENT_CAPACITY, SYSTEM_CAPACITY>& context) : system(context)
			{
				set_signature<Bullet, Position>();
			}


			void run(float dt) override
			{
				for (auto& bullet : get_managed_entities())
				{
					const auto& dir = world_context.get_component<Bullet>(bullet).direction;
					world_context.add_component<Velocity>(bullet, 0, (config::bullet_speed * dir) * dt);
				}
			}
		};
	}
}
