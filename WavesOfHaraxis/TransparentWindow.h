#pragma once
#include <SDL.h>
#include <SDL_syswm.h>
#include <iostream>
#include <SDL_image.h>

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

	SDL_Texture* load_texture(const char* path);

	// Get resolution of primary monitor
	const int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
	const int desktopHeight = GetSystemMetrics(SM_CYSCREEN);

	void handle_keyboard_input(SDL_Event* e);
	void handle_mouse_input(SDL_Event* e);

private:
	bool init(const char* window_name);
	//void UpdateKeys();

	bool make_window_transparent(COLORREF colorKey) const;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};
