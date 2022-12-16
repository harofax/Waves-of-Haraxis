#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class bullet_system : public system
		{
		public:
			bullet_system(world& context) : system(context)
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
