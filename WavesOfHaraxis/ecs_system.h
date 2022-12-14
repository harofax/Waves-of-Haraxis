#pragma once
#include <SDL_render.h>

#include "ecs.h"

namespace ecs
{
    class world;

	namespace system
	{
        class system
        {
        public:
	        virtual ~system() = default;

	        system(ecs::world* context)
            {
                world = context;

            }
            virtual void run(float dt) const;

        protected:
             ecs::world* world;
        };

        class render_system final : system
        {
        public:
	        explicit render_system(ecs::world* context, SDL_Renderer* rendr)
		        : system(context)
	        {
                renderer = rendr;
	        }

            void run(float dt) const override;

        private:
            SDL_Renderer* renderer;

        };

        inline void render_system::run(float dt) const
        {
        }

	}
}
