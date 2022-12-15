#pragma once
#include "components.h"
#include "system.h"
#include "world.h"
#include "core/game_config.h"

namespace ecs
{
	namespace systems
	{
		class apply_velocity_system : public system<COMPONENT_CAPACITY, SYSTEM_CAPACITY>
		{
		public:

			apply_velocity_system(ecs::world<COMPONENT_CAPACITY, SYSTEM_CAPACITY>& context) : system(context)
			{
				set_signature<Position, Velocity, Bounds>();
			}

			void run(float dt) override
			{
				for (const auto& entity : get_managed_entities())
				{
					auto [pos, velocity, bounds] = world_context.get_components<Position, Velocity, Bounds>(entity);


					pos.x += velocity.x_vel;
					pos.y += velocity.y_vel;

					if (pos.x > desktop_width)
						pos.x = 0 - bounds.w;

					// (included 0 for clarity)
					else if (pos.x < 0 - bounds.w)
						pos.x = desktop_width;


					world_context.remove_component<Velocity>(entity);
				}
			}


		};
	}
}
