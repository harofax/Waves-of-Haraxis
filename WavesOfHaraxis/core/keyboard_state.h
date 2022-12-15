#pragma once
#include <cassert>
#include <cstdint>
#include <SDL_keyboard.h>
#include <tuple>

class keyboard_state
{
public:

	bool is_key_down(SDL_Scancode key) const
	{
		return cur_keys[key];
	}
	bool is_key_up(SDL_Scancode key) const
	{
		return !cur_keys[key];
	}

	bool is_key_pressed(SDL_Scancode key) const
	{
		return cur_keys[key] && !prev_keys[key];
	}
	bool is_key_released(SDL_Scancode key) const
	{
		return !cur_keys[key] && prev_keys[key];
	}

	void UpdateKeys()
	{
		SDL_PumpEvents();
		std::ignore = memcpy_s(prev_keys, sizeof(prev_keys), cur_keys, sizeof(cur_keys));
		int keys_size = 0;
		auto* keyboard_state = SDL_GetKeyboardState(&keys_size);

		const auto failed = memcpy_s(cur_keys, sizeof(cur_keys), keyboard_state, keys_size);
		assert(failed == 0 && "failed to allocate input state array");
		//printf("fetched keyboard");

	}

protected:
	uint8_t cur_keys[SDL_NUM_SCANCODES]{};
	uint8_t prev_keys[SDL_NUM_SCANCODES]{};

	
};
