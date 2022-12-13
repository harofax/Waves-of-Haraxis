#pragma once
#include "ecs.h"

namespace ecs
{
	struct component_data
	{
		// -- component arrays --
		SpriteComponent sprite_components[MAX_ENTITIES];
		TransformComponent transform_components[MAX_ENTITIES];
		VelocityComponent velocity_components[MAX_ENTITIES];
		HealthComponent health_component[MAX_ENTITIES];
		WeaponComponent weapon_component[MAX_ENTITIES];
		DamagingComponent damaging_component[MAX_ENTITIES];
	};
}
