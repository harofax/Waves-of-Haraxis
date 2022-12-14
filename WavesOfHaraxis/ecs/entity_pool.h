#pragma once
#include <vector>
#include <bitset>
#include <numeric>
#include <entity.h>

namespace ecs
{
    template<std::size_t NumComponents, std::size_t NumSystems>
    class entity_pool
    {

    public:

        void reserve(std::size_t amount)
        {
            available_entities.resize(amount);
            // fill available_entities with increasing entity ids
            std::iota(std::begin(available_entities), std::end(available_entities), 0);
            entity_signatures.resize(amount);
        }

        std::vector<std::bitset<NumComponents>>& get_entity_signatures()
        {
            return entity_signatures;
        }

        const std::bitset<NumComponents>& get_signature(entity entity) const
        {
            return entity_signatures[entity];
        }

        entity create_entity()
        {
            auto entity = uint32_t();

            // check if there's an unused entity already
            if (available_entities.empty())
            {
                // if not, we create a new spot
                entity = static_cast<ecs::entity>(entity_signatures.size());
                entity_signatures.emplace_back();
            }
        	else
            {
                // if there is a free one (maybe it was deleted), we grab the first free one
                // using .back() cuz its FAST
                entity = available_entities.back();
                available_entities.pop_back();
                entity_signatures[entity].reset();
            }

            return entity;
        }

        void remove(entity entity)
        {
            // to remove we add it back into the pool
            available_entities.push_back(entity);
        }

    private:
        std::vector<std::bitset<NumComponents>> entity_signatures;
        std::vector<entity> available_entities;
    };

    
}


