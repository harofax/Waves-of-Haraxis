#pragma once
#include <vector>
#include <numeric>
#include <entity.h>

namespace ecs
{
    template<std::size_t ComponentCapacity, std::size_t SystemCapacity>
    class entity_pool
    {

    public:

        void reserve(std::size_t amount)
        {
            available_entities.resize(amount);
            std::iota(std::begin(available_entities), std::end(available_entities), 0);

            for (auto& entity_to_component_element : entity_to_component_map)
            {
                entity_to_component_element.resize(amount);
            }

            for (auto& entity_to_managed : entity_to_managed_entities)
            {
                entity_to_managed.resize(amount);
            }

            next_entity = 0;
        }


        std::vector<entity_index>& get_entity_to_component(std::size_t type)
        {
            return entity_to_component_map[type];
        }

        std::vector<entity_index>& get_entity_to_managed_entity(std::size_t type)
        {
            return entity_to_managed_entities[type];
        }


        entity create_entity()
        {
            auto entity = ecs::entity();

            // check if there's an unused entity already
            if (available_entities.empty())
            {
                // if not, we create a new spot
                entity = next_entity;
                next_entity++;

                for (auto& entity_to_comp : entity_to_component_map)
                {
                    entity_to_comp.emplace_back(invalid_index);
                }
                for (auto& entity_to_managed_ent : entity_to_managed_entities)
                {
                    // invalid_index is defined in entity.h
                    entity_to_managed_ent.emplace_back(invalid_index);
                }

            }
            else
            {
                // if there is a free one (maybe it was deleted), we grab the first free one
				// using .back() cuz its FAST
                entity = available_entities.back();
                available_entities.pop_back();

                for (auto& entity_to_comp : entity_to_component_map)
                {
                    entity_to_comp[entity] = invalid_index;
                }
                for (auto& entity_to_managed : entity_to_managed_entities)
                {
                    entity_to_managed[entity] = invalid_index;
                }
            }
           

            return entity;
        }

        void remove(entity entity)
        {
            // to remove we add it back into the pool
            available_entities.push_back(entity);
        }

        template<typename T>
        bool has_component(entity entity) const
        {
            return entity_to_component_map[T::component_id][entity] != invalid_index;
        }

        template<typename ...Ts>
        bool has_components(entity entity) const
        {
            return (has_component<Ts>(entity) && ...);
        }


    private:
        entity next_entity = 0;
        std::array<std::vector<entity_index>, ComponentCapacity> entity_to_component_map;
        std::array<std::vector<entity_index>, SystemCapacity> entity_to_managed_entities;
        std::vector<entity> available_entities;
    };


}


