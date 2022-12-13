#pragma once

namespace ecs
{
    // entity ID
    using entity_id = std::uint32_t;
    // to define arrays later
    constexpr size_t MAX_ENTITIES = 2048u;

    // component type ID
    typedef std::uint8_t component_type;
    constexpr size_t MAX_COMPONENTS = 32u;

    static const uint8_t sprite_id = 1 << 0;
    static const uint8_t transform_id = 1 << 1;
    static const uint8_t velocity_id = 1 << 2;
    static const uint8_t health_id = 1 << 3;
    static const uint8_t weapon_id = 1 << 4;
    static const uint8_t damaging_id = 1 << 5;

    // signatures
    using signature = uint8_t;
    static const signature render_sign = sprite_id | transform_id;
    static const signature movement_sign = transform_id | velocity_id;

    // ---- components ----
#pragma region component structs

// tag
    struct Component {};

    // order of members affect size, so
    // I put the smallest last so it packs more efficiently
    struct SpriteComponent : Component
    {
	    int sprite_index;
	    component_type id = sprite_id;
    };

    struct TransformComponent : Component
    {
        int x;
        int y;
        int x_scale;
        int y_scale;
        component_type id = transform_id;
    };

    struct VelocityComponent : Component
    {
        int x_dir;
        int y_dir;
        component_type id = velocity_id;
    };

    struct HealthComponent : Component
    {
        int health;
        component_type id = health_id;
    };

    struct WeaponComponent : Component
    {
        int damage;
        component_type id = weapon_id;
        // only shoot up or down, so made it a bool
        bool shoot_up;
    };

    struct DamagingComponent : Component
    {
        int damage;
        component_type id = damaging_id;
    };

#pragma endregion components

}
