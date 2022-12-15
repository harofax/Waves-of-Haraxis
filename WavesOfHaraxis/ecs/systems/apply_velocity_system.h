#pragma once
#include "system.h"
#include "ecs_config.h"
#include "world.h"
#include "components.h"

namespace ecs
{
	namespace systems
	{
		class apply_velocity_system : public system<COMPONENT_CAPACITY, SYSTEM_CAPACITY>
		{
		public:

			apply_velocity_system(ecs::world<COMPONENT_CAPACITY, SYSTEM_CAPACITY>& context) : system(context)
			{
				set_signature<Position, Velocity>();
			}

			void run(float dt) override
			{
				printf("VELOCITY: num entities: %llu\n", get_managed_entities().size());
				for (const auto& entity : get_managed_entities())
				{
					auto [transform, velocity] = world_context.get_components<Position, Velocity>(entity);

					transform.x += velocity.x_vel * dt;
					transform.y += velocity.y_vel * dt;

					world_context.get_component<Position>(entity) = transform;
				}
			}


		};
	}
}
