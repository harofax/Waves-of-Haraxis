#pragma once
#include <SDL.h>
#include <SDL_syswm.h>
#include <iostream>
#include <SDL_image.h>

#include "keyboard_state.h"

class TransparentWindow
{
public:
	TransparentWindow(const char* name);
	virtual ~TransparentWindow();

	void Run();

	void Quit();

	bool is_running { false };

	virtual void Update(float dt) = 0;
	virtual void Draw(float dt, SDL_Renderer* renderer) = 0;

	SDL_Texture* load_texture(const char* path) const;

	// Get resolution of primary monitor
	const int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
	const int desktopHeight = GetSystemMetrics(SM_CYSCREEN);

	SDL_Renderer* renderer = nullptr;

	keyboard_state keyboard{};


private:
	bool init(const char* window_name);
	

	bool make_window_transparent(COLORREF colorKey) const;

	SDL_Window* window = nullptr;
	
};
