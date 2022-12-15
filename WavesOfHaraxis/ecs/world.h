#pragma once

#include <memory>
#include "component_table.h"
#include "entity_pool.h"
#include "system.h"
#include "components.h"

namespace ecs
{
	template<std::size_t ComponentCapacity, std::size_t SystemCapacity>
	class world
	{
	public:

		template<typename T>
		void register_component()
		{
			// check if component is valid
			check_component_type<T>();
			// make a new table for it                                          v--- the table type
			components_data[T::component_id] = std::make_unique<component_table<T, ComponentCapacity, SystemCapacity>>(
				entities.get_entity_to_component(T::component_id), entities.get_entity_signatures());
		}

		template<typename T, typename ...Args>
		T* create_system(Args&& ...args)
		{
			auto type = systems.size();
			auto& system = systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			system->init(type, &entities.get_entity_to_managed_entity(type));

			return static_cast<T*>(system.get());
		}

		void reserve(std::size_t size)
		{
			for (auto i = static_cast<std::size_t>(0); i < ComponentCapacity; ++i)
			{
				if (components_data[i])
				{
					components_data[i]->reserve(size);
				}
				entities.reserve(size);
			}
		}

		entity create_entity()
		{
			return entities.create_entity();
		}

		void remove_entity(entity r_entity)
		{
			for (auto i = static_cast<size_t>(0); i < ComponentCapacity; ++i)
			{
				if (components_data[i])
				{
					components_data[i]->try_remove(r_entity);
				}

				for (auto& system : systems)
				{
					system->on_entity_removed(r_entity);
				}

				entities.remove(r_entity);
			}
		}

		void run_systems(float dt)
		{
			for (auto& system : systems)
			{
				system->run(dt);
			}
		}

		template<typename T>
		bool has_component(entity entity) const
		{
			// check if comp is valid
			check_component_type<T>();

			// check the signature to see if the component id bit is set
			return entities.get_signature(entity)[T::component_id];
		}

		template<typename ...Ts>
		bool has_components(entity entity) const
		{
			check_component_types<Ts...>();
			auto signatures = std::bitset<ComponentCapacity>();
			// fold this hecking thing
			(signatures.set(Ts::component_id), ...);
			return (signatures & entities.get_signature(entity)) == signatures;
		}

		template<typename T>
		T& get_component(entity entity)
		{
			check_component_type<T>();
			return get_component_table<T>()->get(entity);
		}


		template<typename T>
		const T& get_component(entity entity) const;

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
			const auto& signature = entities.get_signature(entity);
			for (auto& system : systems)
			{
				system->on_entity_updated(entity, signature);
			}
		}

		template<typename T>
		void remove_component(entity entity)
		{
			check_component_type<T>();
			get_component_table<T>()->remove(entity);

			// notify systems
			const auto& signature = entities.get_signature(entity);
			for (auto& system : systems)
			{
				system->on_entity_updated(entity, signature);
			}
		}

		template<typename T>
		entity get_owner(const T& component) const
		{
			check_component_type<T>();
			return get_component_table<T>()->get_owner(component);
		}

	private:
		std::array<std::unique_ptr<base_component_table>, ComponentCapacity> components_data{};
		entity_pool<ComponentCapacity, SystemCapacity> entities{};
		std::vector<std::unique_ptr<system<ComponentCapacity, SystemCapacity>>> systems{};



		template<typename T>
		void check_component_type() const
		{
			static_assert(std::is_base_of_v<ecs::Component<T>, T>);
		}

		template<typename  ...Ts>
		void check_component_types() const
		{
			(check_component_type<Ts>(), ...);
		}

		template<typename T>
		auto get_component_table()
		{
			return static_cast<component_table<T, ComponentCapacity, SystemCapacity>*>(components_data[T::component_id].get());
		}
		template<typename T>
		auto get_component_table() const
		{
			return static_cast<const component_table<T, ComponentCapacity, SystemCapacity>*>(components_data[T::component_id].get());
		};

	};
}