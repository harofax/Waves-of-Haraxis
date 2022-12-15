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

    struct SpriteComponent : public Component<SpriteComponent, ComponentType::Sprite>
    {
	    explicit SpriteComponent(int spr_id) : sprite_index(spr_id) {}
	    int sprite_index = -1;
    };

    struct BoundsComponent : public Component<BoundsComponent, ComponentType::Bounds>
    {
        BoundsComponent(int W, int H) : w(W), h(H) {}
        int w;
        int h;
    };

    struct TransformComponent : public Component<TransformComponent, ComponentType::Transform>
    {
	    TransformComponent(int x, int y)
		    : x(x),
		      y(y)
	    {
	    }

	    int x;
        int y;
    };

    struct VelocityComponent : public Component<VelocityComponent, ComponentType::Velocity>
    {
	    VelocityComponent(int x_vel, int y_vel)
		    : x_vel(x_vel),
		      y_vel(y_vel)
	    {
	    }

	    int x_vel;
        int y_vel;
    };

    struct HealthComponent : public Component<HealthComponent, ComponentType::Health>
    {
	    explicit HealthComponent(int health)
		    : health(health)
	    {
	    }

	    int health;
    };

    struct WeaponComponent : public Component<WeaponComponent, ComponentType::Weapon>
    {
	    WeaponComponent(int damage, bool shoot_up)
		    : damage(damage),
		      shoot_up(shoot_up)
	    {
	    }

	    int damage;
        // only shoot up or down, so made it a bool
        bool shoot_up;
    };

    struct DamagingComponent : public Component<DamagingComponent, ComponentType::Damaging>
    {
	    explicit DamagingComponent(int damage)
		    : damage(damage)
	    {
	    }

	    int damage;
    };

}

