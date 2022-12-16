#pragma once
#include "SDL_render.h"
#include "system.h"

#define PRINT(x) std::cout << (x) << std::endl

namespace ecs
{
	namespace systems
	{
		class render_system : public system
		{
		public:

			render_system(ecs::world& context, SDL_Renderer* renderer) : system(context)
			{
				this->renderer = renderer;
		

				set_signature<Sprite, Position, Bounds>();
			}

			void run(float dt) override
			{
				for (const auto& entity  : get_managed_entities())
				{
					auto [sprite, position, bounds] =
						world_context.get_components<ecs::Sprite, ecs::Position, ecs::Bounds>(entity);

					auto atlas_rect = SpriteManager::sprite_table[sprite.sprite_index];

					SDL_Rect render_rect
					{
						static_cast<int>(position.x),
						static_cast<int>(position.y),
						static_cast<int>(bounds.w),
						static_cast<int>(bounds.h)
					};

					

					SDL_RenderCopy(renderer, SpriteManager::atlas_texture, &atlas_rect, &render_rect);
				}
			}

		private:
			SDL_Renderer* renderer;
		};
	}
	
}
