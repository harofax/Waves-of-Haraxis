#pragma once
#include "core/TransparentWindow.h"
#include <random>
#include "ext/json.hpp"
#include <fstream>
#include "world.h"
#include "components.h"


using json = nlohmann::json;

constexpr auto COMPONENT_CAPACITY = static_cast<std::size_t>(32);
constexpr auto SYSTEM_CAPACITY = static_cast<std::size_t>(32);

constexpr auto ENTITY_CAPACITY = static_cast<std::size_t>(1024);

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
	void draw_planets(SDL_Renderer* renderer);
	void draw_players(SDL_Renderer* renderer);
	virtual void Draw(float dt, SDL_Renderer* renderer) final;

private:
	SDL_Texture* texture_atlas {nullptr};
	//json load_json_data(const char* json_path) const;

	ecs::world<COMPONENT_CAPACITY, SYSTEM_CAPACITY> world;
};

