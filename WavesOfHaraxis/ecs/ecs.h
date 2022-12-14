#pragma once
#include <bitset>
#include <static_vector.hpp>
#include "ecs_system.h"
#include <variant>

namespace ecs
{

    // component type ID
    typedef std::uint8_t component_type;
    constexpr size_t MAX_COMPONENTS = 32u;

    static const component_type sprite_id = 1 << 0;
    static const component_type transform_id = 1 << 1;
    static const component_type velocity_id = 1 << 2;
    static const component_type health_id = 1 << 3;
    static const component_type weapon_id = 1 << 4;
    static const component_type damaging_id = 1 << 5;

    // signatures
    using signature = uint32_t;
    static const signature render_sign = sprite_id | transform_id;
    static const signature movement_sign = transform_id | velocity_id;

    // entity ID
    typedef std::uint32_t entity_id;
    // to define arrays later
    constexpr size_t MAX_ENTITIES = 1024u;

    struct entity
    {
        entity_id id;
        signature component_signature;
    };

    // ---- components ----
#pragma region component structs

// tag
    struct Component
    {
        component_type id;

        Component(const component_type comp_id)
        {
	        id = comp_id;
        }
    };

    struct SpriteComponent : Component
    {
        SpriteComponent(const int sprite_index) : Component(sprite_id), sprite_index(sprite_index)
        {
        }
	    int sprite_index = -1;

    };

    struct TransformComponent : Component
    {
	    TransformComponent( int x, int y, int x_scale, int y_scale)
		    : Component(transform_id),
		      x(x),
		      y(y),
		      x_scale(x_scale),
		      y_scale(y_scale)
	    {
	    }

	    int x = -1;
        int y = -1;
        int x_scale = -1;
        int y_scale = -1;
    };

    struct VelocityComponent : Component
    {
	    VelocityComponent(int x_dir, int y_dir)
		    : Component(velocity_id),
		      x_dir(x_dir),
		      y_dir(y_dir)
	    {
	    }

	    int x_dir = -1;
        int y_dir = -1;
    };

    struct HealthComponent : Component
    {
	    explicit HealthComponent(const int health)
		    : Component(health_id), health(health)
	    {
	    }

	    int health = -1;
    };

    struct WeaponComponent : Component
    {
	    WeaponComponent(int damage, bool shoot_up)
		    : Component(weapon_id),
		      damage(damage),
		      shoot_up(shoot_up)
	    {
	    }

	    int damage = -1;
        // only shoot up or down, so made it a bool
        bool shoot_up;
    };

    struct DamagingComponent : Component
    {
	    DamagingComponent(int damage)
		    : Component(damaging_id),
		      damage(damage)
	    {
	    }

	    int damage;
    };

#pragma endregion components

    template<typename T, size_t Capacity>
    using static_vector = itlib::static_vector<T, Capacity>;

    using components_variant = std::variant<
										    static_vector<SpriteComponent, MAX_ENTITIES>,
										    static_vector<TransformComponent, MAX_ENTITIES>,
										    static_vector < VelocityComponent, MAX_ENTITIES>,
                                            static_vector < HealthComponent, MAX_ENTITIES >,
                                            static_vector < WeaponComponent, MAX_ENTITIES >,
                                            static_vector<DamagingComponent,  MAX_ENTITIES>
										    >;

    class world
    {
    public:
        world() : sprite_components(), transform_components(), velocity_components(), health_components(),
            weapon_components(),
            damaging_components()
        {

            next_entity = 0;

            component_data.emplace(sprite_id,  sprite_components);
            component_data.emplace(transform_id,  transform_components);
            component_data.emplace(velocity_id,  velocity_components);
            component_data.emplace(health_id,  health_components);
            component_data.emplace(weapon_id,  weapon_components);
            component_data.emplace(damaging_id,  damaging_components);

            

        }

        entity create_entity()
	    {
            assert(next_entity < MAX_ENTITIES && "REACHED MAX ENTITIES");

            constexpr signature blank{1u};

            const entity new_entity = { next_entity++, blank};
            return new_entity;
	    }

        template<typename AddType>
        void add_component(entity entity, AddType comp_t)
        {
            // id == 1 << 2, 1 << 3, 1 << 4 etc
            assert(entity.id < MAX_ENTITIES && "INVALID ENTITY ID");

            auto& comp = reinterpret_cast<Component>(comp_t);

            // Get a reference to the vector associated with the key
            //auto comp_array = std::get<static_vector<Component, MAX_ENTITIES>>(component_data.at(comp.id));
            std::visit([comp](auto&& arr)
                {
                    arr.emplace_back(comp);
            }, component_data.at(comp.id));

            std::bitset<8> entity_comps(entity.component_signature);
            std::cout << entity_comps << std::endl;

        	entity.component_signature |= comp.id;

            std::bitset<8> post_add(entity.component_signature);
            std::cout << post_add << std::endl;

        }

        void print_components()
        {
            for (auto& [key, control] : component_data)
            {
                std::cout << key << std::endl;
                std::visit([](auto&& c)
                    {
                        for (auto& e : c)
                        {
                            std::cout << "id: " << e.id << std::endl;
                        }
                    }, control);
            }
        }

        void id_to_array(entity_id id, signature)
	    {
		    
	    }

        

        //template<typename T>
        //void add_component_data(itlib::static_vector<T, MAX_ENTITIES> &table)
	    //{
        //    static_assert(std::is_base_of<Component, T>::value, "Must be valid component type");
        //    component_data.emplace_back(table);
	    //}

        
    private:

        entity_id next_entity = 0;
        signature entity_signatures[MAX_ENTITIES] = {};

        std::vector<ecs::system::system*> systems;

	    // -- component data static vectors --
        static_vector< SpriteComponent,      MAX_ENTITIES > sprite_components;   
        static_vector< TransformComponent,   MAX_ENTITIES > transform_components;
        static_vector< VelocityComponent,    MAX_ENTITIES > velocity_components; 
        static_vector< HealthComponent,      MAX_ENTITIES > health_components;   
        static_vector< WeaponComponent,      MAX_ENTITIES > weapon_components;   
        static_vector< DamagingComponent ,   MAX_ENTITIES > damaging_components; 

        std::unordered_map<component_type, components_variant> component_data;

        //std::vector<itlib::static_vector<Component*, MAX_ENTITIES>> component_data;
        
    };
}

