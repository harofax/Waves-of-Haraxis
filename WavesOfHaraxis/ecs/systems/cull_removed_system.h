#pragma once
#include "system.h"

namespace ecs
{
	namespace systems
	{
		class cull_removed_system : public system<COMPONENT_CAPACITY, SYSTEM_CAPACITY>
		{
		public:
			cull_removed_system(world<COMPONENT_CAPACITY, SYSTEM_CAPACITY>& context) : system(context)
			{
				set_signature<ToBeRemoved>();
			}

			void run(float dt) override
			{
				for (auto& removed_entity : get_managed_entities())
				{
					world_context.remove_entity(removed_entity);
				}
			}
		};
	}
}
