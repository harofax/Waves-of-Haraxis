// SDL window with transparent background v1.2
#include <iostream>
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_image.h>
#include <vector>
#include <Windows.h>

#define SCALE 4

// Window
SDL_Window* gWindow = nullptr;

// Renderer
SDL_Renderer* gRenderer = nullptr;

constexpr int NUM_CREATURE_SPRITES = 6;
// array for the creatures/entities
//SDL_Texture* creature_textures[NUM_CREATURE_SPRITES];
SDL_Texture* texture_atlas;

// dynamic array for planets, as they are dynamically loaded
// and I wanna be able to just plop them in the folder and have more
std::vector<SDL_Texture*> planet_textures;

// screen surface
SDL_Surface* gScreenSurface = nullptr;

// Initializes SDL and creates a window
bool init();

// Loads media files
bool loadMedia();

// Loads individual image
SDL_Surface* loadSurface(std::string path);

//Load individual image as texture
SDL_Texture* loadTexture(std::string path);

SDL_Texture* loadCreatures(SDL_Texture* creature_texture_array, std::string creature_path);

std::vector<SDL_Texture*> load_planets(std::string planet_path);

//Frees media and shuts down SDL
void close();
void destroy_texture_array(SDL_Texture* texture_array);
void destroy_texture_list(std::vector<SDL_Texture*> texture_list);

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

// Get resolution of primary monitor
const int desktopWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
const int desktopHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

void handle_mouse_input(SDL_Event* e);

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

    //int glorpo_height = 0;
    //int glorpo_width = 0;
    //
    //SDL_QueryTexture(creature_texture_sheet, nullptr, nullptr, &glorpo_width, &glorpo_height);
    //
    //SDL_Rect glorpo_dest;
    //glorpo_dest.x = desktopWidth / 2;
    //glorpo_dest.y = desktopHeight / 2;
    //glorpo_dest.h = CREATURE_SPRITE_SIZE;
    //glorpo_dest.w = CREATURE_SPRITE_SIZE;
    //
    //glorpo_dest.w *= SCALE;
    //glorpo_dest.h *= SCALE;


    //int bag_height = 0;
    //int bag_width = 0;
    //
    //SDL_QueryTexture(bag_texture, nullptr, nullptr, &bag_width, &bag_height);
    //
    //SDL_Rect inventoryBagView;
    //inventoryBagView.x = 0;
    //inventoryBagView.y = desktopHeight - (bag_height * SCALE);
    //inventoryBagView.h = bag_height * SCALE;
    //inventoryBagView.w = bag_width * SCALE;



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
                        //glorpo_dest.x += 5;
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            }
        }
        // Clear
        SDL_RenderClear(gRenderer);

        //SDL_RenderSetViewport(gRenderer, &glorpo_dest);
        //const SDL_Rect* currentClip = &gSpriteClips[frame / 6];
        //// Render texture
        //SDL_RenderCopy(gRenderer, creature_texture_sheet, currentClip, nullptr);
        //
        //SDL_RenderSetViewport(gRenderer, &inventoryBagView);
        //
        //SDL_RenderCopy(gRenderer, bag_texture, nullptr, nullptr);

        // Render the square to the screen
        SDL_RenderPresent(gRenderer);

        //++frame;
        //
        //if (frame / 6 >= IDLE_ANIMATION_FRAMES)
        //{
        //    frame = 0;
        //}


        //move_randomly(glorpo_dest);
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

    bool creature_load_success = true;
    creature_load_success = loadCreatures(*creature_textures, "res\\creatures");

    if (!creature_load_success)
    {
        printf("failed to load creature sprites!");
        success = false;
    }

    planet_textures = load_planets("res\\planets");

    if (planet_textures.empty())
    {
        printf("Failed to load planet textures!");
        success = false;
    }
    return success;
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


void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    
    SDL_DestroyTexture(creature_texture_atlas);

    destroy_texture_list(planet_textures);

    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    IMG_Quit();
    SDL_Quit();
}

void destroy_texture_list(std::vector<SDL_Texture*> texture_list)
{
	for (auto& creature_texture : texture_list)
}



