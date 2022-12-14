#pragma once
#include <numeric>

namespace ecs
{
    // ---- components ----

    enum class ComponentType
    {
        Transform,
	    Sprite,
        Bounds,
        Velocity,
        Health,
        Weapon,
        Damaging,

    };

    template<typename T, auto ID>
    struct Component
    {
        static constexpr auto component_id = static_cast<std::size_t>(ID);
    };

    struct SpriteComponent : Component<SpriteComponent, ComponentType::Sprite>
    {
	    int sprite_index = -1;
    };

    struct BoundsComponent : Component<BoundsComponent, ComponentType::Bounds>
    {
        int w;
        int h;
    };

    struct TransformComponent : Component<TransformComponent, ComponentType::Transform>
    {
	    int x;
        int y;
    };

    struct VelocityComponent : Component<VelocityComponent, ComponentType::Velocity>
    {
	    int x_vel;
        int y_vel;
    };

    struct HealthComponent : Component<HealthComponent, ComponentType::Health>
    {
	    int health;
    };

    struct WeaponComponent : Component<WeaponComponent, ComponentType::Weapon>
    {
	    int damage;
        // only shoot up or down, so made it a bool
        bool shoot_up;
    };

    struct DamagingComponent : Component<DamagingComponent, ComponentType::Damaging>
    {
	    int damage;
    };

}

