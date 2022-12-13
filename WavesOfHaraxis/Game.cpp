#include "Game.h"

#include <bitset>

#define PRINT(x) std::cout << (x) << std::endl

// --- game config ---

// texture atlas paths
const char* texture_atlas_image_path = "res\\spritesheet.png";
const char* texture_atlas_json_path = "res\\spritesheet.json";

// -- game config --

int num_background_planets = 0;
int num_player_ships = 0;
int num_enemy_ships = 0;
int render_scale = 2;

// --- data ---

// won't use more than 16 sprites 
SDL_Rect sprite_table[16];
//std::map<int, SDL_Rect> sprite_map;


// 0 -> planet, 1 -> player, 2 -> enemy
const int PLANET_SPRITE_RANGE_INDEX = 0;
const int PLAYER_SPRITE_RANGE_INDEX = 1;
const int ENEMY_SPRITE_RANGE_INDEX = 2;
int spriteRangeTable[3];

Game::Game(const char* window_name) : TransparentWindow(window_name)
{
	std::cout << "GAME INIT" << std::endl;

	if (!load_media())
	{
        printf("Could not load media properly");
	}
	else if (!load_config())
	{
        printf("Could not load config file properly");
	}

    init_stuff();
    init_planets();
    init_players();
	
}

void parse_json_data(json::const_reference data)
{
    // config according to json data
    num_background_planets = data["config"]["num_bg_planets"];
    num_player_ships = data["config"]["num_player_ships"];
    num_enemy_ships = data["config"]["num_enemy_ships"];

    render_scale = data["config"]["scale"];

    int sprite_index = 0;

    const auto planet_json = data["sprites"]["planets"];

    for (auto planet_sprite : planet_json)
    {
        SDL_Rect planet_rect;
        planet_rect.x = planet_sprite["rect"]["x"];
        planet_rect.y = planet_sprite["rect"]["y"];
        planet_rect.w = planet_sprite["rect"]["w"];
        planet_rect.h = planet_sprite["rect"]["h"];

        sprite_table[sprite_index] = planet_rect;
        sprite_index++;
    }
    spriteRangeTable[PLANET_SPRITE_RANGE_INDEX] = sprite_index - 1;

    const auto player_json = data["sprites"]["player"];

    for (auto player_sprite : player_json)
    {
        SDL_Rect player_rect;
        player_rect.x = player_sprite["rect"]["x"];
        player_rect.y = player_sprite["rect"]["y"];
        player_rect.w = player_sprite["rect"]["w"];
        player_rect.h = player_sprite["rect"]["h"];

        sprite_table[sprite_index] = player_rect;
        sprite_index++;
    }
    spriteRangeTable[PLAYER_SPRITE_RANGE_INDEX] = sprite_index - 1;

    const auto enemy_json = data["sprites"]["enemy"];

    for (auto enemy_sprite : enemy_json)
    {
        SDL_Rect enemy_rect;
        enemy_rect.x = enemy_sprite["rect"]["x"];
        enemy_rect.y = enemy_sprite["rect"]["y"];
        enemy_rect.w = enemy_sprite["rect"]["w"];
        enemy_rect.h = enemy_sprite["rect"]["h"];

        sprite_table[sprite_index] = enemy_rect;
    	sprite_index++;
    }

    spriteRangeTable[ENEMY_SPRITE_RANGE_INDEX] = sprite_index - 1;
}

bool load_json_data(const char* json_path, json& data_out)
{
	std::ifstream json_file(json_path);

    if(!json_file.good())
    {
        printf("Couldn't open file %s", json_path);
        return false;
    }

	data_out = json::parse(json_file);

	return true;
}

bool Game::load_media()
{
	bool success = true;

	texture_atlas = load_texture(texture_atlas_image_path);

	if (texture_atlas == nullptr)
	{
		printf("failed to load spritesheet at %s", texture_atlas_image_path);
		success = false;
	}

    return success;
}

bool Game::load_config()
{
	json data;
	if (!load_json_data(texture_atlas_json_path, data))
	{
        printf("Could not load JSON.\n");
        return false;
	}
    parse_json_data(data);

	return true;
}

void Game::init_stuff()
{
    int shift = 2;

	std::bitset<8> one(1);
    PRINT(one);

    uint8_t mask = 1 << shift;
    std::bitset<8> onemask(mask);
    PRINT("ID LOOKS LIKE THIS:");
    PRINT(onemask);

    uint8_t unmask = 1 >> shift;
    std::bitset<8> unmaskbit(unmask);
    PRINT("REVERSE?? shift back:");
    unsigned long index;
    BitScanReverse(&index, mask);
    PRINT(index);

    for (int i = 0; i < 0; i++)
    {
	    const auto e = world.create_entity();

    	PRINT("id: " );
        PRINT(e);
    }
}

void Game::init_planets()
{
    std::uniform_int_distribution<> x_pos_dist(0, desktopWidth);
    std::uniform_int_distribution<> y_pos_dist(0, desktopHeight);

    std::uniform_int_distribution<> sprite_dist(0, spriteRangeTable[PLANET_SPRITE_RANGE_INDEX]);

    for (int i = 0; i < num_background_planets; i++)
    {
        int pos_x = x_pos_dist(generator);
        int pos_y = y_pos_dist(generator);
        int sprite_index = sprite_dist(generator);

        auto& planet_rect = sprite_table[sprite_index];

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

        //Planet random_planet
        //{
        //    pos_x,
        //    pos_y,
        //    sprite_index
        //};
        //
        //planet_table[i] = random_planet;
    }
}

void Game::init_players()
{
    std::uniform_int_distribution<> x_pos_dist(0, desktopWidth);
    std::uniform_int_distribution<> y_pos_dist(desktopHeight / 2, desktopHeight);

    for (int i = 0; i < num_player_ships; i++)
    {
        int pos_x = x_pos_dist(generator);
        int pos_y = y_pos_dist(generator);
        int sprite_index = spriteRangeTable[PLANET_SPRITE_RANGE_INDEX] + 2;

        auto& rect = sprite_table[sprite_index];

        int x_width = pos_x + rect.w * render_scale;
        int y_height = pos_y + rect.h * render_scale;

        if (x_width > desktopWidth)
        {
            pos_x -= rect.w * render_scale;
        }
        if (y_height > desktopHeight)
        {
            pos_y -= rect.h * render_scale;
        }

        //PlayerShip player
        //{
        //    pos_x,
        //    pos_y,
        //    sprite_index
        //};
        //printf("player#%d intialized, pos: (%d, %d)\n", i, pos_x, pos_y);
        //player_table[i] = player;
    }

}

void Game::init_enemies()
{
    std::uniform_int_distribution<> x_pos_dist(0, desktopWidth);
    std::uniform_int_distribution<> y_pos_dist(0, desktopHeight);
}

void Game::draw_planets(SDL_Renderer* renderer)
{
	//for (Planet& planet : planet_table)
	//{
    //    auto planet_atlas_rect = sprite_table[planet.sprite_index];
    //
    //    SDL_Rect render_rect
    //    {
    //        planet.x,
    //        planet.y,
    //        planet_atlas_rect.w * render_scale,
    //        planet_atlas_rect.h * render_scale
    //    };
    //
    //    //SDL_Rect debug;
    //    //debug.x = 142;
    //    //debug.y = 144;
    //    //debug.w = 1;
    //    //debug.h = 1;
    //    //
    //    //SDL_Rect debug_dest;
    //    //debug_dest.x = planet.x;
    //    //debug_dest.y = planet.y;
    //    //
    //    //debug_dest.h = render_scale * 2;
    //    //debug_dest.w = render_scale * 2;
    //    //SDL_RenderCopy(gRenderer, texture_atlas, &debug, &debug_dest);
    //
    //    SDL_RenderCopy(renderer, texture_atlas, &planet_atlas_rect, &render_rect);
	//}
}

void Game::draw_players(SDL_Renderer* renderer)
{
	//for (PlayerShip player : player_table)
	//{
    //    auto player_atlas_rect = sprite_table[player.sprite_index];
    //
    //    SDL_Rect render_rect
    //    {
    //        player.x,
    //        player.y,
    //        player_atlas_rect.w * render_scale,
    //        player_atlas_rect.h * render_scale
    //    };
    //
    //    SDL_RenderCopy(renderer, texture_atlas, &player_atlas_rect, &render_rect);
	//}
}

void Game::Draw(float dt, SDL_Renderer* renderer)
{
    //draw_planets(renderer);
    //draw_players(renderer);
}

void Game::Update(float dt)
{
	// TODO: add gameplay lol
}

Game::~Game()
{
    SDL_DestroyTexture(texture_atlas);
}


//SDL_Texture* Game::load_texture(std::string path)


