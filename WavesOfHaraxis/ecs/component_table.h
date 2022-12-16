#pragma once
#include <vector>
#include "entity.h"

namespace ecs
{
	class base_component_table
	{
	public:
		virtual ~base_component_table() = default;

		virtual void reserve(std::size_t size) = 0;
		virtual bool try_remove(entity entity) = 0;
	};

	template<typename T>
	class component_table : public base_component_table
	{
	public:
		component_table(std::vector<entity_index>& ent_to_component) :
			entity_to_component(ent_to_component)
		{

		}

		virtual void reserve(std::size_t size) override
		{
			component_to_entity.reserve(size);
			components.reserve(size);
		}

		T& get(entity entity)
		{
			return components[entity_to_component[entity]];
		}

		const T& get(entity entity) const
		{
			return components[entity_to_component[entity]];
		}

		template<typename... Args>
		void add(entity entity, Args&&... args)
		{
			auto index = static_cast<entity_index>(components.size());
			components.emplace_back(std::forward<Args>(args)...);
			component_to_entity.emplace_back(entity);
			entity_to_component[entity] = index;
		}

		void remove(entity entity)
		{
			auto index = entity_to_component[entity];

			// move -> popback => fast

			// update components table
			components[index] = std::move(components.back());
			components.pop_back();

			// update entity_to_component map
			entity_to_component[component_to_entity.back()] = index;
			entity_to_component[entity] = invalid_index;
			//entity_to_component.erase(entity);

			// update component_to_entity table
			component_to_entity[index] = component_to_entity.back();
			component_to_entity.pop_back();
		}

		// checks if entity has the component before trying to remove it
		virtual bool try_remove(entity entity) override
		{
			// check if the bit for the component is set in the signature
			if (entity_to_component[entity] != invalid_index)
			{
				remove(entity);
				return true;
			}

			return false;
		}

		entity get_owner(const T& component) const
		{
			auto begin = components.data();
			auto index = static_cast<std::size_t>(&component - begin);
			return component_to_entity[index];
		}

	private:
		std::vector<T> components;
		std::vector<entity> component_to_entity;
		std::vector<entity_index>& entity_to_component;
	};
}