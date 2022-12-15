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
	Uint64 previous_tick = SDL_GetPerformanceCounter();
	float dt = 0.0f;

	is_running = true;

	// event handler
	SDL_Event event;

	while (is_running)
	{

		

		SDL_PumpEvents();
		keyboard.UpdateKeys();

		if (keyboard.is_key_pressed(SDL_SCANCODE_ESCAPE))
		{
			is_running = false;
		}

		// tick stuff
		{
			const Uint64 new_tick = SDL_GetPerformanceCounter();
			const Uint64 diff = new_tick - previous_tick;
			dt = static_cast<float>(diff) / static_cast<float>(SDL_GetPerformanceFrequency());
			previous_tick = new_tick;
		}

		// clear to magenta (our color key -> transparent)
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderClear(renderer);

		Update(dt);
		//Draw(dt, renderer);

		SDL_RenderPresent(renderer);
	}
}

void TransparentWindow::Quit()
{
	is_running = false;
}

SDL_Texture* TransparentWindow::load_texture(const char* path) const
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
			desktop_width, desktop_height, SDL_WINDOW_BORDERLESS);

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





