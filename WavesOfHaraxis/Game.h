#pragma once
#include "core/TransparentWindow.h"
#include <random>
#include "ext/json.hpp"
#include <fstream>
#include "world.h"
#include "components.h"
#include "core/game_config.h"


using json = nlohmann::json;


class Game final : public TransparentWindow
{
public:
	Game(const char* window_name);
	~Game();

	bool load_media();
	bool load_config();

	void init_ecs();
	void init_planets();
	void init_players();
	void init_enemies();

	std::random_device rand;
	std::mt19937 generator{(rand())};

	//SDL_Texture* load_texture(std::string path);

	virtual void Update(float dt) final;
	virtual void Draw(float dt, SDL_Renderer* renderer) final;

private:
	SDL_Texture* texture_atlas {nullptr};
	//json load_json_data(const char* json_path) const;

	ecs::world<ecs::COMPONENT_CAPACITY, ecs::SYSTEM_CAPACITY> world;
};

