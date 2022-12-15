#pragma once
namespace ecs
{
	constexpr size_t ENTITY_CAPACITY = 1024u;
	constexpr size_t COMPONENT_CAPACITY = 32u;
	constexpr size_t SYSTEM_CAPACITY = 32u;
}

// Get resolution of primary monitor
const int desktop_width = GetSystemMetrics(SM_CXSCREEN);
const int desktop_height = GetSystemMetrics(SM_CYSCREEN);

constexpr float player_speed = 200.0f;