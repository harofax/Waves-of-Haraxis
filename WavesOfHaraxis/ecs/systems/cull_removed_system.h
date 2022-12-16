#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class cull_removed_system : public system
		{
		public:
			cull_removed_system(world& context) : system(context)
			{
				set_signature<ToBeRemoved>();
			}

			void run(float dt) override
			{
				for (auto& removed_entity : get_managed_entities())
				{
					if (world_context.has_component<Position>(removed_entity))
					{
						auto& pos = world_context.get_component<Position>(removed_entity);
						world_context.grid.remove_entity(removed_entity, pos.x, pos.y);
					}
					world_context.remove_entity(removed_entity);
				}
			}
		};
	}
}
