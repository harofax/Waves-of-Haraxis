#pragma once

namespace ecs
{
	#include <static_vector.hpp>
    // entity ID
    typedef std::uint32_t entity_id;
    // to define arrays later
    constexpr size_t MAX_ENTITIES = 1024u;

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
    using signature = uint8_t;
    static const signature render_sign = sprite_id | transform_id;
    static const signature movement_sign = transform_id | velocity_id;

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

    // order of members affect size, so
    // I put the smallest last so it packs more efficiently

    struct SpriteComponent : Component
    {
        SpriteComponent(const int sprite_index) : Component(sprite_id), sprite_index(sprite_index)
        {
        }
	    int sprite_index = -1;

    };

    struct TransformComponent : Component
    {
	    TransformComponent(component_type comp_id, int x, int y, int x_scale, int y_scale)
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

    class world
    {
    public:
	    world(): sprite_components(), transform_components(), velocity_components(), health_components(),
	             weapon_components(),
	             damaging_components()
	    {
            
            next_entity = 0;
            component_data.emplace_back(sprite_components);
            component_data.emplace_back(transform_components);
            component_data.emplace_back(velocity_components);
            component_data.emplace_back(health_components);
            component_data.emplace_back(weapon_components);
            component_data.emplace_back(damaging_components);
	    }

        entity_id create_entity()
	    {
            assert(next_entity < MAX_ENTITIES && "REACHED MAX ENTITIES");
            return next_entity++;
	    }

        void add_component(entity_id id, Component comp)
	    {
            assert(id < MAX_ENTITIES && "INVALID ENTITY ID");
            unsigned long index;
            BitScanReverse(&index, comp.id);

	    }

        
    private:

        entity_id next_entity = 0;
        signature entity_signatures[MAX_ENTITIES] = {};
    public:
	    // -- component data static vectors --
        itlib::static_vector< SpriteComponent,      MAX_ENTITIES > sprite_components;
        itlib::static_vector< TransformComponent,   MAX_ENTITIES > transform_components;
        itlib::static_vector< VelocityComponent,    MAX_ENTITIES > velocity_components;
        itlib::static_vector< HealthComponent,      MAX_ENTITIES > health_components;
        itlib::static_vector< WeaponComponent,      MAX_ENTITIES > weapon_components;
        itlib::static_vector< DamagingComponent ,   MAX_ENTITIES > damaging_components;

        itlib::static_vector<itlib::static_vector<Component, MAX_ENTITIES>, MAX_COMPONENTS> component_data;
    };
}

