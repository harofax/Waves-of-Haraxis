#include "TransparentWindow.h"

#include <cassert>


static int quit_filter(void* data, SDL_Event* event)
{
	if (event->type == SDL_QUIT)
	{
		const auto win = static_cast<TransparentWindow*>(data);
		win->Quit();
	}
	return 0;
};

TransparentWindow::TransparentWindow(const char* name)
{
	if (!init(name))
	{
		printf("Failed to init!\n");
	}

	SDL_AddEventWatch(quit_filter, this);
}

void TransparentWindow::Run()
{
	auto tick = SDL_GetTicks64();
	auto dt = 0.0f;

	is_running = true;

	// event handler
	SDL_Event event;

	while (is_running)
	{
		//SDL_PumpEvents();
		//UpdateKeys();
		while(SDL_PollEvent(&event) != 0)
		{
			handle_keyboard_input(&event);
			handle_mouse_input(&event);
		}


		// tick stuff
		{
			const auto new_tick = SDL_GetTicks64();
			const auto diff = new_tick - tick;
			dt = static_cast<float>(diff) / 1000.0f;
			tick = new_tick;
		}

		Update(dt);

		// clear to magenta (our color key -> transparent)
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderClear(renderer);

		Draw(dt, renderer);

		SDL_RenderPresent(renderer);
	}
}

void TransparentWindow::Quit()
{
	is_running = false;
}

SDL_Texture* TransparentWindow::load_texture(const char* path)
{
	// output texture
	SDL_Texture* output_texture = nullptr;

	SDL_Surface* loaded_surface = IMG_Load(path);
	if (loaded_surface == nullptr)
	{
		printf("Unable to load image %s. IMG_Error: %s\n", path, IMG_GetError());
	}
	else
	{
		output_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);

		if (output_texture == nullptr)
		{
			printf("Unable to create texture from %s. IMG_Error: %s", path, IMG_GetError());
		}

		SDL_FreeSurface(loaded_surface);
	}

	return output_texture;
}


void TransparentWindow::handle_keyboard_input(SDL_Event* e)
{
	if (e->type == SDL_QUIT)
	{
		is_running = false;
	}
	if (e->type == SDL_KEYDOWN)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			is_running = false;
			break;
		}
	}
}

void TransparentWindow::handle_mouse_input(SDL_Event* e)
{
	// mouse input
}

bool TransparentWindow::init(const char* window_name)
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		window = SDL_CreateWindow(window_name,
			0, 0,
			desktopWidth, desktopHeight, SDL_WINDOW_BORDERLESS);

		if (window == nullptr)
		{
			printf("Window couldn't get created! SDL_Error: %s", SDL_GetError());

			success = false;
		}
		else
		{
			// our colorkey is magenta, everything magenta => transparent
			constexpr auto magenta = RGB(255, 0, 255);
			if (!make_window_transparent(magenta))
			{
				printf("Could not make window transparent! Error: %lu", GetLastError());
				success = false;
			}
			else
			{
				renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

				if (renderer == nullptr)
				{
					printf("SDL_Renderer could not get created. SDL_Error: %s", SDL_GetError());
					success = false;
				}
				else
				{
					// set background colour to magenta to make it transparent
					SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

					if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
					{
						printf("SDL_Image could not initialize! SDL_Img error: %s", IMG_GetError());
						success = false;
					}
				}
			}
		}
	}
	return success;
}

TransparentWindow::~TransparentWindow()
{
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;

	IMG_Quit();
	SDL_Quit();
}


// Makes a window transparent by setting a transparency color.
bool TransparentWindow::make_window_transparent(COLORREF colorKey) const
{
	// Get window handle (https://stackoverflow.com/a/24118145/3357935)
	SDL_SysWMinfo wm_info;
	SDL_VERSION(&wm_info.version);
	// Initialize wmInfo
	SDL_GetWindowWMInfo(window, &wm_info);
	const HWND h_wnd = wm_info.info.win.window;

	// Change window type to layered (https://stackoverflow.com/a/3970218/3357935)
	SetWindowLong(h_wnd, GWL_EXSTYLE, GetWindowLong(h_wnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	// Set transparency color
	return SetLayeredWindowAttributes(h_wnd, colorKey, 0, LWA_COLORKEY);
}





