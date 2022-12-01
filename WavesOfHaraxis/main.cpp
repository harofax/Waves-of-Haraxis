// SDL window with transparent background v1.2
#include "main.h"

#include <random>


using json = nlohmann::json;

// Makes a window transparent by setting a transparency color.
bool MakeWindowTransparent(SDL_Window* window, COLORREF colorKey)
{
    // Get window handle (https://stackoverflow.com/a/24118145/3357935)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hWnd = wmInfo.info.win.window;

    // Change window type to layered (https://stackoverflow.com/a/3970218/3357935)
    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    // Set transparency color
    return SetLayeredWindowAttributes(hWnd, colorKey, 0, LWA_COLORKEY);
}

int main(int argc, char** argv)
{

    if (!init())
    {
        printf("Failed to initialize!\n");
        return 1;
    }
    else
    {
        if (!loadMedia())
        {
            printf("Failed to load media files!\n");
            return 1;
        }
    }

    init_planets();

    std::cout << "desktop width: " << desktopWidth << "\ndesktop height: " << desktopHeight << std::endl;

    // Loop until user quits
    bool quit = false;

    // Event handler
    SDL_Event event;

    // Current anim frame
    int frame = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }

            //handle_mouse_input(&event, glorpo_dest);
            //handle_mouse_input(&event, inventoryBagView);

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_SPACE:
                        init_planets();
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            }
        }
        // Clear
        SDL_RenderClear(gRenderer);

        render_planets();
        //// Render texture
        //SDL_RenderCopy(gRenderer, creature_texture_sheet, currentClip, nullptr);


        SDL_RenderPresent(gRenderer);

    }

    close();
    return 0;
}

void handle_mouse_input(SDL_Event* e, SDL_Rect& glorpo_pos)
{
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        bool inside = true;

        if (x < glorpo_pos.x)
        {
            inside = false;
        }
        else if (x > glorpo_pos.x + glorpo_pos.w)
        {
            inside = false;
        }
        else if (y < glorpo_pos.y)
        {
            inside = false;
        }
        else if (y > glorpo_pos.y + glorpo_pos.h)
        {
            inside = false;
        }



        if (inside)
        {
            switch (e->type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    //drag = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    //drag = false;
                    break;
                case SDL_MOUSEMOTION:
                    //if (drag)
                    //{
                    //    glorpo_pos.x = x - (CREATURE_SPRITE_SIZE * SCALE) / 2;
                    //    glorpo_pos.y = y - (CREATURE_SPRITE_SIZE * SCALE) / 2;
                    //}
                    break;
            }
        }
    }
}

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("Waves of Haraxis",
            0, 0,
            desktopWidth, desktopHeight, SDL_WINDOW_BORDERLESS);

        if (gWindow == nullptr)
        {
            printf("Window could not get created! SDL_Error: %s\n", SDL_GetError());

            success = false;
        }
        else
        {
            // Add window transparency (Magenta will be see-through / colorkey)
            MakeWindowTransparent(gWindow, RGB(255, 0, 255));

            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            if (gRenderer == nullptr)
            {
                printf("SDL_Renderer could not get created. SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                // Set background color to magenta and clear screen
                SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);

                constexpr int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_Image could not initialize! SDL_Image error: %s\n", IMG_GetError());
                    success = false;
                }
            }


        }
    }



    return success;
}


bool loadMedia()
{
    bool success = true;

    texture_atlas = loadTexture("res\\spritesheet.png");

    if (texture_atlas == nullptr)
    {
        printf("failed to load sprite sheet!");
        success = false;
    } else
    {
        // try to load json data
	    json data = load_json_data(texture_atlas_json);

        // parse data and set up stuff
        parse_json_data(data);
    }

    return success;
}

json load_json_data(const std::string& json_path)
{
    // load json file
    std::ifstream json_file(json_path);

	// parse file
    json data = json::parse(json_file);

    return data;
}

void parse_json_data(json::const_reference data)
{
    // config according to json data
    num_background_planets = data["config"]["num_bg_planets"];
    render_scale = data["config"]["scale"];

    const auto planet_json = data["sprites"]["planets"];

    for (auto planet_sprite : planet_json)
    {
        SDL_Rect planet_rect;
        planet_rect.x = planet_sprite["rect"]["x"];
        planet_rect.y = planet_sprite["rect"]["y"];
        planet_rect.w = planet_sprite["rect"]["w"];
        planet_rect.h = planet_sprite["rect"]["h"];

        planet_rects.push_back(planet_rect);

    }

    const auto player_json = data["sprites"]["player"];

    for (auto player_sprite : player_json)
    {
        SDL_Rect player_rect;
        player_rect.x = player_sprite["rect"]["x"];
        player_rect.y = player_sprite["rect"]["y"];
        player_rect.w = player_sprite["rect"]["w"];
        player_rect.h = player_sprite["rect"]["h"];

        player_rects.push_back(player_rect);
    }

    const auto enemy_json = data["sprites"]["enemy"];

    for (auto enemy_sprite : enemy_json)
    {
        SDL_Rect enemy_rect;
        enemy_rect.x = enemy_sprite["rect"]["x"];
        enemy_rect.y = enemy_sprite["rect"]["y"];
        enemy_rect.w = enemy_sprite["rect"]["w"];
        enemy_rect.h = enemy_sprite["rect"]["h"];

        enemy_rects.push_back(enemy_rect);
    }
}


void init_planets()
{
    planets.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_distr(0, desktopWidth);
    std::uniform_int_distribution<> y_distr(0, desktopHeight);
    std::uniform_int_distribution<> sprite_distr(0, planet_rects.size() - 1);



    for (int i = 0; i < num_background_planets; i++)
    {

        int pos_x = x_distr(gen);
        int pos_y = y_distr(gen);
        int sprite_idx = sprite_distr(gen);

        auto& planet_rect = planet_rects[sprite_idx];

        int x_width = pos_x + planet_rect.w * render_scale;
        int y_height = pos_y + planet_rect.h * render_scale;

        if (x_width > desktopWidth)
        {
            pos_x -= planet_rect.w * render_scale;
        }
    	if (y_height > desktopHeight)
        {
            pos_y -= planet_rect.h * render_scale;
        }
        

        Planet random_planet(pos_x, pos_y, sprite_idx);

        planets.push_back(random_planet);
    }
}

SDL_Surface* loadSurface(std::string path)
{
    SDL_Surface* optimizedSurface = nullptr;


    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == nullptr)
    {
        printf("Unable to load image %s. SDL Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == nullptr)
        {
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

SDL_Texture* loadTexture(std::string path)
{
    // output texture
    SDL_Texture* newTexture = nullptr;

    // Load image
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr)
    {
        printf("Unable to load image %s. SDL Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

        if (newTexture == nullptr)
        {
            printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), IMG_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void render_planets()
{
	for (Planet planet : planets)
	{
        auto planet_atlas_rect = planet_rects[planet.sprite_index];

		SDL_Rect render_rect;
		render_rect.x = planet.x;
        render_rect.y = planet.y;
        render_rect.w = planet_atlas_rect.w * render_scale;
        render_rect.h = planet_atlas_rect.h * render_scale;

        //SDL_Rect debug;
        //debug.x = 142;
        //debug.y = 144;
        //debug.w = 1;
        //debug.h = 1;
        //
        //SDL_Rect debug_dest;
        //debug_dest.x = planet.x;
        //debug_dest.y = planet.y;
        //
        //debug_dest.h = render_scale * 2;
        //debug_dest.w = render_scale * 2;
        //SDL_RenderCopy(gRenderer, texture_atlas, &debug, &debug_dest);

        SDL_RenderCopy(gRenderer, texture_atlas, &planet_atlas_rect, &render_rect);
	}
}


void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    
    SDL_DestroyTexture(texture_atlas);


    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    IMG_Quit();
    SDL_Quit();
}



