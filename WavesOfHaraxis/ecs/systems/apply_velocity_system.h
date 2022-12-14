#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class apply_velocity_system : public system
		{
		public:

			apply_velocity_system(ecs::world& context) : system(context)
			{
				set_signature<Position, Velocity, Bounds>();
			}

			void run(float dt) override
			{
				for (const auto& entity : get_managed_entities())
				{
					auto [pos, velocity, bounds] = world_context.get_components<Position, Velocity, Bounds>(entity);

					const Position old_pos = pos;

					world_context.grid.remove_entity(entity, old_pos.x, old_pos.y);

					pos.x += velocity.x_vel;
					pos.y += velocity.y_vel;

					if (pos.x > config::desktop_width)
					{
						pos.x = 0 - bounds.w;
					}

					// (included 0 for clarity)
					else if (pos.x < 0 - bounds.w)
					{
						pos.x = config::desktop_width;
					}
						

					world_context.grid.add_entity(entity, pos.x, pos.y);

					world_context.remove_component<Velocity>(entity);
				}
			}


		};
	}
}
