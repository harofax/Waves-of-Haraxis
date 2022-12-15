#pragma once
#include "system.h"
#include "world.h"
#include "components.h"
#include "core/game_config.h"
#include "keyboard_state.h"


namespace ecs
{
	namespace systems
	{
		class input_system : public system<COMPONENT_CAPACITY, SYSTEM_CAPACITY>
		{
		public:
			input_system(world<COMPONENT_CAPACITY, SYSTEM_CAPACITY>& context, keyboard_state& kb) : system(context), input(kb)
			{
				set_signature<PlayerInput, Position>();
			}


			void run(float dt) override
			{
				float x_dir = 0;
				float y_dir = 0;

				bool moved = false;

				if (input.is_key_down(SDL_SCANCODE_A))
				{
					x_dir = -1;
					moved = true;
				}
				if (input.is_key_down(SDL_SCANCODE_D))
				{
					x_dir = 1;
					moved = true;
				}
				if (input.is_key_down(SDL_SCANCODE_W))
				{
					y_dir = -1;
					moved = true;
				}
				if (input.is_key_down(SDL_SCANCODE_S))
				{
					y_dir = 1;
					moved = true;
				}

				if (moved)
				{
					for (auto& entity : get_managed_entities())
					{
						auto& player_input = world_context.get_component<PlayerInput>(entity);
						world_context.add_component<Velocity>(entity, x_dir * (player_speed * dt), y_dir * (player_speed * dt));
					}
				}
			}

		private:
			keyboard_state& input;
		};
	}
}
