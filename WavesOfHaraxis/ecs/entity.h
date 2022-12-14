#pragma once
#include <cstdint>
#include <limits>

namespace ecs
{
	using entity = uint32_t;
	using entity_index = uint32_t;
	static constexpr auto invalid_index = std::numeric_limits<entity_index>::max();
}
