#pragma once
#include "system.h"
#include "world.h"
#include "components.h"
#include "ecs_config.h"
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
				printf("INPUT: num entities: %llu\n", get_managed_entities().size());
				int x_dir = 0;
				int y_dir = 0;

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
					y_dir = 1;
					moved = true;
				}
				if (input.is_key_down(SDL_SCANCODE_S))
				{
					y_dir = -1;
					moved = true;
				}

				if (moved)
				{
					printf("MOVED!!\n");
					
					for (auto& entity : get_managed_entities())
					{
						printf("adding velocity component to #%d !!!!!!!\n", entity);
						world_context.add_component<Velocity>(entity, x_dir, y_dir);
					}
				}
			}

		private:
			keyboard_state& input;
		};
	}
}
