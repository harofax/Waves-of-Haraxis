#pragma once

#include <memory>
#include "component_table.h"
#include "entity_pool.h"
#include "system.h"
#include "components.h"

namespace ecs
{
	class world
	{
	public:
		world(std::size_t comp_count, std::size_t sys_count) : entities(comp_count, sys_count)
		{
			components_data.resize(comp_count);
		}

		template<typename T>
		void register_component()
		{
			// check if component is valid
			check_component_type<T>();
			// make a new table for it                                          v--- the table type
			components_data[T::component_id] = std::make_unique<component_table<T>>(
				entities.get_entity_to_component(T::component_id));
		}

		template<typename T, typename ...Args>
		T* create_system(Args&& ...args)
		{
			auto type = systems.size();
			auto& system = systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			system->init(type, &entities.get_entity_to_managed_entity(type), &entities);

			return static_cast<T*>(system.get());
		}

		void reserve(std::size_t size)
		{
			for (auto& comp_table : components_data)
			{
				if (comp_table)
				{
					comp_table->reserve(size);
				}
			}
			entities.reserve(size);
		}

		entity create_entity()
		{
			return entities.create_entity();
		}

		void remove_entity(entity r_entity)
		{
			for (auto& comp_table : components_data)
			{
				if (comp_table)
				{
					comp_table->try_remove(r_entity);
				}

			}

			for (auto& system : systems)
			{
				system->on_entity_removed(r_entity);
			}

			entities.remove(r_entity);
		}

		void run_systems(float dt)
		{
			for (auto& system : systems)
			{
				system->run(dt);
			}
		}

		template<typename T>
		bool has_component(ecs::entity entity) const
		{
			// check if comp is valid
			check_component_type<T>();

			// check the signature to see if the component id bit is set
			return entities.has_component<T>(entity);
		}

		template<typename ...Ts>
		bool has_components(ecs::entity entity) const
		{
			check_component_types<Ts...>();
			
			return entities.has_components<Ts...>(entity);
		}

		template<typename T>
		T& get_component(ecs::entity entity)
		{
			check_component_type<T>();
			return get_component_table<T>()->get(entity);
		}


		template<typename T>
		const T& get_component(ecs::entity entity) const
		{
			check_component_type<T>();
			return get_component_table<T>()->get(entity);
		}

		// returns a tuple of the components that are requested, so we can
		// iterate through them in systems
		template<typename ...Comps>
		std::tuple<Comps&...> get_components(entity entity)
		{
			check_component_types<Comps...>();
			return std::tie(get_component_table<Comps>()->get(entity)...);
		}

		template<typename ...Comps>
		std::tuple<const Comps&...> get_components(entity entity) const
		{
			check_component_types<Comps...>();
			return std::tie(std::as_const(get_component_table<Comps>()->get(entity))...);
		}

		template<typename T, typename ...Args>
		void add_component(entity entity, Args&&... args)
		{
			check_component_type<T>();
			get_component_table<T>()->add(entity, std::forward<Args>(args)...);

			// notify systems of what happen
			
			for (auto& system : systems)
			{
				system->on_entity_updated(entity);
			}
		}

		template<typename T>
		void remove_component(entity entity)
		{
			check_component_type<T>();
			get_component_table<T>()->remove(entity);

			// notify systems
			for (auto& system : systems)
			{
				system->on_entity_updated(entity);
			}
		}

		template<typename T>
		entity get_owner(const T& component) const
		{
			check_component_type<T>();
			return get_component_table<T>()->get_owner(component);
		}

	private:
		std::vector<std::unique_ptr<base_component_table>> components_data{};
		entity_pool entities;
		std::vector<std::unique_ptr<system>> systems;



		template<typename T>
		constexpr void check_component_type() const
		{
			static_assert(std::is_base_of_v<ecs::Component<T>, T>);
		}

		template<typename  ...Ts>
		constexpr void check_component_types() const
		{
			(check_component_type<Ts>(), ...);
		}

		template<typename T>
		auto get_component_table()
		{
			return static_cast<component_table<T>*>(components_data[T::component_id].get());
		}
		template<typename T>
		auto get_component_table() const
		{
			return static_cast<const component_table<T>*>(components_data[T::component_id].get());
		};

	};
}