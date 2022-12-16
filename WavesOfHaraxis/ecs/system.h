#pragma once

#include <functional>
#include <vector>
#include "entity.h"
#include "entity_pool.h"
// for inheriting systems
#include "components.h"
#include "world.h"
#include "game_config.h"

namespace ecs
{

	class world;

	class system
	{
	public:
		system(world& context) : world_context(context)
		{
			// set default signature check for systems that dont check for it
			check_signature = [this](ecs::entity entity)
			{
				return false;
			};
		}
		virtual ~system() = default;

		virtual void run(float dt) = 0;


	protected:
		template<typename ...Ts>
		void set_signature()
		{
			check_signature = [this](ecs::entity entity)
			{
				return entities->has_components<Ts...>(entity);
			};
		}

		const std::vector<entity>& get_managed_entities() const
		{
			return managed_entities;
		}

		world& world_context;

		virtual void on_managed_entity_added([[maybe_unused]] entity entity) {}
		virtual void on_managed_entity_removed([[maybe_unused]] entity entity) {}

	private:
		friend ecs::world;

		std::function<bool(ecs::entity)> check_signature;
		std::size_t system_type;
		std::vector<entity> managed_entities;
		std::vector<entity_index>* entity_to_managed_entity = nullptr;
		const entity_pool* entities = nullptr;

		void init(std::size_t type, std::vector<entity_index>* entity_to_managed_entity_map,
			const entity_pool* entities_pool)
		{
			system_type = type;
			entity_to_managed_entity = entity_to_managed_entity_map;
			entities = entities_pool;
		}

		// check if entity is still relevant to current system
		void on_entity_updated(entity entity)
		{
			auto match = check_signature(entity);
			auto managed = (*entity_to_managed_entity)[entity] != invalid_index;

			if (match && !managed)
			{
				add_entity(entity);
			}
			else if (!match && managed)
			{
				remove_entity(entity);
			}
		}

		void on_entity_removed(entity entity)
		{
			if ((*entity_to_managed_entity)[entity] != invalid_index)
			{
				remove_entity(entity);
			}
		}

		void add_entity(entity entity)
		{
			(*entity_to_managed_entity)[entity] = static_cast<entity_index>(managed_entities.size());
			managed_entities.emplace_back(entity);
			on_managed_entity_added(entity);
		}

		void remove_entity(entity entity)
		{
			on_managed_entity_removed(entity);
			const auto index = (*entity_to_managed_entity)[entity];

			(*entity_to_managed_entity)[managed_entities.back()] = index;
			(*entity_to_managed_entity)[entity] = invalid_index;

			managed_entities[index] = managed_entities.back();
			managed_entities.pop_back();
		}


	};
}