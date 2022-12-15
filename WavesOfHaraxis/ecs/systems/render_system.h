#pragma once
#include "system.h"
#include "SDL_render.h"
#include "world.h"
#include "components.h"
#include "ecs_config.h"

#define PRINT(x) std::cout << (x) << std::endl

namespace ecs
{
	namespace systems
	{
		class render_system : public system<COMPONENT_CAPACITY, SYSTEM_CAPACITY>
		{
		public:

			render_system(ecs::world<COMPONENT_CAPACITY, SYSTEM_CAPACITY>& context, SDL_Renderer* renderer, SDL_Texture* atlas, SDL_Rect atlas_lookup_table[]) : system(context)
			{
				this->renderer = renderer;
				this->atlas_texture = atlas;
				this->atlas_lookup = atlas_lookup_table;

				set_signature<SpriteComponent, TransformComponent, BoundsComponent>();
			}

			void run(float dt) override
			{
				const auto& drawable_entities = this->get_managed_entities();

				for (const auto& entity : drawable_entities)
				{
					auto [sprite, transform, bounds] =
						world_context.get_components<ecs::SpriteComponent, ecs::TransformComponent, ecs::BoundsComponent>(entity);

					auto atlas_rect = atlas_lookup[sprite.sprite_index];

					SDL_Rect render_rect
					{
						transform.x,
						transform.y,
						bounds.w,
						bounds.h
					};

					

					SDL_RenderCopy(renderer, atlas_texture, &atlas_rect, &render_rect);
				}
			}

		private:
			SDL_Renderer* renderer;
			SDL_Texture* atlas_texture {nullptr};
			SDL_Rect* atlas_lookup;
		};
	}
	
}
