#pragma once
#include <numeric>

namespace ecs
{
    // ---- components ----
    template<typename T>
    class Component
    {
    public:
        static const std::size_t component_id;
    };

    inline std::size_t generate_component_id()
    {
        static auto counter = static_cast<std::size_t>(0);
        return counter++;
    }

    template<typename T>
    const std::size_t Component<T>::component_id = generate_component_id();

    struct Sprite : public Component<Sprite>
    {
	    explicit Sprite(int spr_id) : sprite_index(spr_id) {}
	    int sprite_index = -1;
    };

    struct Bounds : public Component<Bounds>
    {
        Bounds(float width, float height) : w(width), h(height) {}
        float w;
        float h;
    };

    struct Position : public Component<Position>
    {
	    Position(float x, float y)
		    : x(x),
		      y(y)
	    {
	    }

	    float x;
        float y;
    };

    struct Velocity : public Component<Velocity>
    {
	    Velocity(float x_vel, float y_vel)
		    : x_vel(x_vel),
		      y_vel(y_vel)
	    {
	    }

	    float x_vel;
        float y_vel;
    };

    struct Health : public Component<Health>
    {
	    explicit Health(int health)
		    : health(health)
	    {
	    }

	    int health;
    };

    struct Weapon : public Component<Weapon>
    {
	    Weapon(int damage, bool shoot_up)
		    : damage(damage),
		      shoot_up(shoot_up)
	    {
	    }

	    int damage;
        // only shoot up or down, so made it a bool
        bool shoot_up;
    };

    struct Damaging : public Component<Damaging>
    {
	    explicit Damaging(int damage)
		    : damage(damage)
	    {
	    }

	    int damage;
    };

    struct PlayerInput : public Component<PlayerInput>
    {
	    PlayerInput(bool can_move) : can_move(can_move)
	    {
	    }

	    bool can_move;
    };

}

