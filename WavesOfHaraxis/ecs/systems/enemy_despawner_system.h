#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class enemy_despawner_system : public system
		{
		public:

			enemy_despawner_system(world& context) : system(context)
			{
				set_signature<Enemy, Alive, Position>();
			}

			void run(float dt) override
			{
				for (auto& entity : get_managed_entities())
				{
					auto& pos = world_context.get_component<Position>(entity);

					if (pos.y > config::desktop_height)
					{
						world_context.add_component<ToBeRemoved>(entity);
					}
				}
			}

		};
	}
}
