#pragma once
#include <vector>
#include <bitset>
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
            entity_signatures.resize(amount);

            for (auto& entity_to_component_element : entity_to_component_map)
            {
                entity_to_component_element.resize(amount);
            }

            for (auto& entity_to_managed : entity_to_managed_entities)
            {
                entity_to_managed.resize(amount);
            }

        }

        std::vector<std::bitset<ComponentCapacity>>& get_entity_signatures()
        {
            return entity_signatures;
        }

        const std::bitset<ComponentCapacity>& get_signature(entity entity) const
        {
            return entity_signatures[entity];
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
                entity = static_cast<ecs::entity>(entity_signatures.size());
                entity_signatures.emplace_back();

                for (auto& entity_to_comp : entity_to_component_map)
                {
                    entity_to_comp.emplace_back();
                }
                for (auto& entity_to_managed_ent : entity_to_managed_entities)
                {
                    // invalid_index is defined in entity.h
                    entity_to_managed_ent.emplace_back(invalid_index);
                }

                return entity;

            }
           
            // if there is a free one (maybe it was deleted), we grab the first free one
            // using .back() cuz its FAST
            entity = available_entities.back();
            available_entities.pop_back();
            entity_signatures[entity].reset();

            for (auto& entity_to_managed : entity_to_managed_entities)
            {
                entity_to_managed[entity] = invalid_index;
            }
           

            return entity;
        }

        void remove(entity entity)
        {
            // to remove we add it back into the pool
            available_entities.push_back(entity);
        }

    private:
        std::vector<std::bitset<ComponentCapacity>> entity_signatures;
        std::array<std::vector<entity_index>, ComponentCapacity> entity_to_component_map;
        std::array<std::vector<entity_index>, SystemCapacity> entity_to_managed_entities;
        std::vector<entity> available_entities;
    };


}


