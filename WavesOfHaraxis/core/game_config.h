#pragma once
namespace ecs
{
	constexpr size_t ENTITY_CAPACITY = 1024u;
	constexpr size_t COMPONENT_CAPACITY = 32u;
	constexpr size_t SYSTEM_CAPACITY = 32u;
}

namespace config
{
	// Get resolution of primary monitor
	const int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	const int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	inline int render_scale = 2;

	inline float player_speed = 1.f;
	inline float enemy_base_speed = 1.f;

	inline int player_hp = 1;

	inline int enemy_time_between_waves = 0;
	inline int enemies_per_wave = 0;
	inline int enemy_hp = 1;

	inline int bullet_damage = 1;
	inline float bullet_speed = 500.f;
}


