#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class kill_dead_system : public system
		{
		public:
			kill_dead_system(world& context) : system(context)
			{
				set_signature<Health>();
			}

			void run(float dt) override
			{
				for (auto& entity : get_managed_entities())
				{
					const auto& hp = world_context.get_component<Health>(entity);
					if (hp.health < 0)
					{
						world_context.add_component<ToBeRemoved>(entity);
					}
				}
			}
		};
	}
}

