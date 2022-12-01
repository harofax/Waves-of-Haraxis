#pragma once
#include <fstream>
#include <SDL.h>
#include <SDL_syswm.h>
#include <vector>
#include <Windows.h>
#include "json.hpp"
#include "static_vector.hpp"

struct Planet
{
    int x = 0;
    int y = 0;
    int sprite_index = 0;
    Planet(int pos_x, int pos_y, int spr_idx)
    {
        x = pos_x;
        y = pos_y;
        sprite_index = spr_idx;
    }
};

// Get resolution of primary monitor
const int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
const int desktopHeight = GetSystemMetrics(SM_CYSCREEN);

// Window
SDL_Window* gWindow = nullptr;

// Renderer
SDL_Renderer* gRenderer = nullptr;

// screen surface
SDL_Surface* gScreenSurface = nullptr;

SDL_Texture* texture_atlas;
const std::string texture_atlas_json = "res\\spritesheet.json";

std::vector<SDL_Rect> planet_rects;
std::vector<SDL_Rect> player_rects;
std::vector<SDL_Rect> enemy_rects;

int num_background_planets = 0;
std::vector<Planet> planets;

int render_scale = 1;

// Initializes SDL and creates a window
bool init();

// Loads media files
bool loadMedia();

// inits background planets
void init_planets();

// Loads individual image
SDL_Surface* loadSurface(std::string path);

//Load individual image as texture
SDL_Texture* loadTexture(std::string path);

// load json data
nlohmann::json load_json_data(const std::string& json_path);

// parse json data
void parse_json_data(nlohmann::json::const_reference data);


void render_planets();


// handle mouse input
void handle_mouse_input(SDL_Event* e);

//Frees media and shuts down SDL
void close();