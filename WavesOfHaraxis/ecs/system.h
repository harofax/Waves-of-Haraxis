#pragma once
#include <bitset>
#include <unordered_map>
#include <vector>
#include "entity.h"

namespace ecs
{
	template<std::size_t ComponentCapacity, std::size_t SystemCapacity>
	class world;

	template<std::size_t ComponentCapacity, std::size_t SystemCapacity>
	class system
	{
	public:
		system(world<ComponentCapacity, SystemCapacity>& context) : world_context(context) {}
		virtual ~system() = default;

		virtual void run(float dt) = 0;


	protected:
		template<typename ...Ts>
		void set_signature()
		{
			(signature.set(Ts::component_id), ...);
		}

		[[nodiscard]] const std::vector<entity>& get_managed_entities() const
		{
			return managed_entities;
		}

		world<ComponentCapacity, SystemCapacity>& world_context;

		virtual void on_managed_entity_added([[maybe_unused]] entity entity){}
		virtual void on_managed_entity_removed([[maybe_unused]] entity entity){}

	private:
		friend ecs::world<ComponentCapacity, SystemCapacity>;

		std::bitset<ComponentCapacity> signature;
		std::vector<entity> managed_entities;
		std::unordered_map<entity, entity_index> entity_to_managed_entity;

		// check if entity is still relevant to current system
		void on_entity_updated(entity entity, const std::bitset<ComponentCapacity>& components)
		{
			auto match = (signature & components) == signature;
			auto managed = entity_to_managed_entity.contains(entity);

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
			if (entity_to_managed_entity.contains(entity))
			{
				remove_entity(entity);
			} 
		}

		void add_entity(entity entity)
		{
			entity_to_managed_entity[entity] = static_cast<entity_index>(managed_entities.size());
			managed_entities.emplace_back(entity);
			on_managed_entity_added(entity);
		}

		void remove_entity(entity entity)
		{
			on_managed_entity_removed(entity);
			auto index = entity_to_managed_entity[entity];

			entity_to_managed_entity[managed_entities.back()] = index;
			entity_to_managed_entity.erase(entity);

			managed_entities[index] = managed_entities.back();
			managed_entities.pop_back();
		}


	};
}
