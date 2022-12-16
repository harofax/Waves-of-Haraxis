#pragma once
#include "system.h"
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

				bool shoot = false;

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

				if (input.is_key_pressed(SDL_SCANCODE_SPACE))
				{
					shoot = true;
				}

				if (moved || shoot)
				{
					for (auto& player_ship : get_managed_entities())
					{
						if (moved)
							world_context.add_component<Velocity>(player_ship, x_dir * (config::player_speed * dt), y_dir * (config::player_speed * dt));
						if (shoot)
							world_context.add_component<Shooting>(player_ship);
					}
				}
			}

		private:
			keyboard_state& input;
		};
	}
}
