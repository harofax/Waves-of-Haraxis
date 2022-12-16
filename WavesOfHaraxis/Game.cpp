#include "Game.h"


#define PRINT(x) std::cout << (x) << std::endl

// --- game config ---

// texture atlas paths
const char* texture_atlas_image_path = "res\\spritesheet.png";
const char* texture_atlas_json_path = "res\\spritesheet.json";

// -- game config --

int num_background_planets = 0;
int num_player_ships = 0;
int num_total_enemy_ships = 0;


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

    init_ecs();
    init_planets();
    init_players();
    //init_players();
	
}

void parse_json_data(json::const_reference data)
{
    // read config from json
    num_background_planets = data["config"]["num_bg_planets"];
    num_player_ships = data["config"]["num_player_ships"];
    num_total_enemy_ships = data["config"]["num_enemy_ships"];

    config::enemy_time_between_waves = data["config"]["time_between_waves"];
    config::enemies_per_wave = data["config"]["enemies_per_wave"];

	config::enemy_hp = data["config"]["enemy_base_hp"];
    config::player_hp = data["config"]["player_hp"];

    config::player_speed = data["config"]["player_speed"];
    config::enemy_base_speed = data["config"]["enemy_base_speed"];

    config::bullet_damage = data["config"]["bullet_damage"];
    config::bullet_speed = data["config"]["bullet_speed"];

    config::render_scale = data["config"]["scale"];


    // read sprite info and put in SpriteManager

    int sprite_index = 0;

    const auto& planet_json = data["sprites"]["planets"];

    for (auto& planet_sprite : planet_json)
    {
	    const SDL_Rect planet_rect
    	{
            planet_sprite["rect"]["x"],
            planet_sprite["rect"]["y"],
            planet_sprite["rect"]["w"],
            planet_sprite["rect"]["h"]
        };

	    SpriteManager::sprite_table[sprite_index] = planet_rect;
        sprite_index++;
    }
    SpriteManager::sprite_range_table[SpriteManager::PLANET_SPRITE_RANGE_INDEX] = sprite_index - 1;

    const auto& player_json = data["sprites"]["player"];

    for (auto& player_sprite : player_json)
    {
	    const SDL_Rect player_rect
    	{
            player_sprite["rect"]["x"],
            player_sprite["rect"]["y"],
            player_sprite["rect"]["w"],
            player_sprite["rect"]["h"]
    	};

	    SpriteManager::sprite_table[sprite_index] = player_rect;
        sprite_index++;
    }
    SpriteManager::sprite_range_table[SpriteManager::PLAYER_SPRITE_RANGE_INDEX] = sprite_index - 1;

    const auto& enemy_json = data["sprites"]["enemy"];

    for (auto& enemy_sprite : enemy_json)
    {
	    const SDL_Rect enemy_rect
    	{
            enemy_sprite["rect"]["x"],
            enemy_sprite["rect"]["y"],
            enemy_sprite["rect"]["w"],
            enemy_sprite["rect"]["h"]
        };

	    SpriteManager::sprite_table[sprite_index] = enemy_rect;
    	sprite_index++;
    }

    SpriteManager::sprite_range_table[SpriteManager::ENEMY_SPRITE_RANGE_INDEX] = sprite_index - 1;

    const auto& bullet_json = data["sprites"]["bullet"];

    const SDL_Rect bullet_rect
	{
		bullet_json["rect"]["x"],
        bullet_json["rect"]["y"],
        bullet_json["rect"]["w"],
        bullet_json["rect"]["h"]
    };
    SpriteManager::sprite_table[sprite_index] = bullet_rect;
    SpriteManager::sprite_range_table[SpriteManager::BULLET_SPRITE_INDEX] = sprite_index;


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

	SpriteManager::atlas_texture = load_texture(texture_atlas_image_path);

	if (SpriteManager::atlas_texture == nullptr)
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

void Game::init_ecs()
{
    // -- reserve space for entities --
    world.reserve(ecs::ENTITY_CAPACITY);

	// -- registering components --
    world.register_component<ecs::Sprite>();
    world.register_component<ecs::Bounds>();
    world.register_component<ecs::Position>();
    world.register_component<ecs::Velocity>();
    world.register_component<ecs::Health>();
    world.register_component<ecs::Weapon>();
    world.register_component<ecs::Damaging>();
    world.register_component<ecs::PlayerInput>();
    world.register_component<ecs::Enemy>();
    world.register_component<ecs::Bullet>();
    world.register_component<ecs::Shooting>();
    world.register_component<ecs::Alive>();
    world.register_component<ecs::ToBeRemoved>();
    // todo: collider? maybe just use bounds


        // -- create and register systems --
    world.create_system<ecs::systems::input_system>(world, keyboard);
    world.create_system<ecs::systems::apply_velocity_system>(world);


	world.create_system<ecs::systems::enemy_spawner_system>(world, config::enemies_per_wave, config::enemy_time_between_waves, generator);
    world.create_system<ecs::systems::enemy_mover_system>(world);

	world.create_system<ecs::systems::shooting_system>(world);
    world.create_system<ecs::systems::bullet_system>(world);


    // todo: make generic culling system instead
    world.create_system<ecs::systems::enemy_despawner_system>(world);
    world.create_system<ecs::systems::bullet_despawn_system>(world);

    

    world.create_system<ecs::systems::cull_removed_system>(world);

    world.create_system<ecs::systems::collision_damage_system>(world);
    world.create_system<ecs::systems::enemy_contact_damage_system>(world);
    world.create_system<ecs::systems::kill_dead_system>(world);
    world.create_system<ecs::systems::render_system>(world, renderer);

}

void Game::init_planets()
{
    std::uniform_int_distribution<> x_pos_dist(0, config::desktop_width);
    std::uniform_int_distribution<> y_pos_dist(0, config::desktop_height);

    std::uniform_int_distribution<> sprite_dist(0, SpriteManager::sprite_range_table[SpriteManager::PLANET_SPRITE_RANGE_INDEX]);

    for (int i = 0; i < num_background_planets; i++)
    {
        int pos_x = x_pos_dist(generator);
        int pos_y = y_pos_dist(generator);
        int sprite_index = sprite_dist(generator);

        const auto& planet_rect = SpriteManager::sprite_table[sprite_index];

        const int bounds_w = planet_rect.w *config::render_scale;
        const int bounds_h = planet_rect.h *config::render_scale;

        int x_width = pos_x + bounds_w;
        int y_height = pos_y + bounds_h;

        if (x_width > config::desktop_width)
        {
            pos_x -= planet_rect.w *config::render_scale;
        }
        if (y_height > config::desktop_height)
        {
            pos_y -= planet_rect.h *config::render_scale;
        }

        const auto planet = world.create_entity();

        world.add_component<ecs::Sprite>(planet, sprite_index);
        world.add_component<ecs::Bounds>(planet, bounds_w, bounds_h);
        world.add_component<ecs::Position>(planet, pos_x, pos_y);

    }
}

void Game::init_players()
{
    std::uniform_int_distribution<> x_pos_dist(0, config::desktop_width);
    std::uniform_int_distribution<> y_pos_dist(config::desktop_height / 2, config::desktop_height);

    for (int i = 0; i < num_player_ships; i++)
    {
        int pos_x = x_pos_dist(generator);
        int pos_y = y_pos_dist(generator);

    	const int sprite_index = SpriteManager::sprite_range_table[SpriteManager::PLANET_SPRITE_RANGE_INDEX] + 2;

        const auto& rect = SpriteManager::sprite_table[sprite_index];

        const int bounds_w = rect.w *config::render_scale;
        const int bounds_h = rect.h *config::render_scale;

        const int x_width = pos_x + bounds_w;
        const int y_height = pos_y + bounds_h;

        if (x_width > config::desktop_width)
        {
            pos_x -= rect.w *config::render_scale;
        }
        if (y_height > config::desktop_height)
        {
            pos_y -= rect.h *config::render_scale;
        }

        const auto& player = world.create_entity();

        world.add_component<ecs::Sprite>(player, sprite_index);
        world.add_component<ecs::Bounds>(player, bounds_w, bounds_h);
        world.add_component<ecs::Position>(player, pos_x, pos_y);
        world.add_component<ecs::PlayerInput>(player, true);
        world.add_component<ecs::Weapon>(player, config::bullet_damage, -1);
        world.add_component<ecs::Health>(player, config::player_hp);

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
}


void Game::Draw(float dt, SDL_Renderer* renderer)
{
    //draw_planets(renderer);
    //draw_players(renderer);
}

void Game::Update(float dt)
{
    world.run_systems(dt);
}

Game::~Game()
{
    SDL_DestroyTexture(SpriteManager::atlas_texture);
}


//SDL_Texture* Game::load_texture(std::string path)


