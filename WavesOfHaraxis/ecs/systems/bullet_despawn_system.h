#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class bullet_despawn_system : public system
		{
		public:
			bullet_despawn_system(world& context) : system(context)
			{
				set_signature<Bullet, Position, Bounds>();
			}


			void run(float dt) override
			{
				for (auto& bullet : get_managed_entities())
				{
					auto [pos, bounds] = world_context.get_components<Position, Bounds>(bullet);

					if (pos.y - bounds.h > config::desktop_height || pos.y + bounds.h < 0)
					{
						world_context.add_component<ToBeRemoved>(bullet);
					}
				}
			}
		};
	}
}
