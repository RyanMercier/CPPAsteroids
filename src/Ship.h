#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <chrono>
#include "raylib.h"
#include "raymath.h"
#include "Projectile.h"

class Ship
{
    bool alive = true;
    Vector2 position = Vector2{0, 0};
    Vector2 velocity = Vector2{0, 0};
    float rotation = 90.0f;
    const float baseSpeed = 20.0f;
    const float maxSpeed = 500.0f;
    const float acceleration = 200.0f;
    const float dragAmount = 100.0f;
    const float rotationSpeed = 650.0f;

    std::vector<Vector2> vertices = {Vector2{10, 0}, Vector2{-10, -8}, Vector2{-10, 8}};

    // bullet control
    float shotsPerSecond = 10;
    std::chrono::system_clock::time_point lastShotTime = std::chrono::system_clock::now();
    std::vector<Projectile *> bullets;

public:
    Ship()
    {
        position = Vector2{0, 0};
        velocity = Vector2{0, 0};
    }

    Ship(Vector2 _position)
    {
        position = _position;
        velocity = Vector2{0, 0};
    }

    Ship(const Ship &copy)
    {
        alive = copy.alive;
        position = copy.position;
        velocity = copy.velocity;
        rotation = copy.rotation;
    }

    ~Ship()
    {
        for (auto bullet : bullets)
        {
            delete bullet;
        }
    }

    bool IsAlive()
    {
        return alive;
    }

    void SetAlive(bool _alive)
    {
        alive = _alive;
    }

    Vector2 GetPosition() const
    {
        return position;
    }

    void SetPosition(Vector2 _position)
    {
        position = _position;
    }

    Vector2 GetVelocity() const
    {
        return velocity;
    }

    void SetVelocity(Vector2 _velocity)
    {
        velocity = _velocity;
    }

    float GetRotation() const
    {
        return rotation;
    }

    void Rotate(float amount)
    {
        rotation += amount;
        if (rotation > 360)
        {
            rotation -= 360;
        }
        else if (rotation < 0)
        {
            rotation += 360;
        }
    }

    std::vector<Projectile *> GetBullets()
    {
        return bullets;
    }

    std::vector<Vector2> GetVertices() const;

    void Accelerate(float _deltaTime);

    void Drag(float _deltaTime);

    void Move(float _deltaTime);

    void TurnLeft(float _deltaTime);

    void TurnRight(float _deltaTime);

    void Shoot();

    void HandleScreenWrap();

    void HandleProjectiles(float _deltaTime);

    void Update(float _deltaTime);

    void Draw();
};

#endif