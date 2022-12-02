#pragma once
#include "TransparentWindow.h"
#include <random>
#include "json.hpp"

using json = nlohmann::json;

class Game : public TransparentWindow
{
public:
	Game(const char* window_name);
	~Game();

	bool load_media();
	void init_planets();

	std::random_device rand;
	std::mt19937 generator{(rand())};

	//SDL_Texture* load_texture(std::string path);

	virtual void Update(float dt) final;
	void draw_planets(SDL_Renderer* renderer);
	virtual void Draw(float dt, SDL_Renderer* renderer) final;

private:
	SDL_Texture* texture_atlas {nullptr};
	//json load_json_data(const char* json_path) const;
};

