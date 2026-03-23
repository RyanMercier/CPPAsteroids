#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <chrono>
#include "raylib.h"
#include "raymath.h"
#include "Config.h"
#include "Projectile.h"

class Ship
{
    bool alive = true;
    Vector2 position = Vector2{0, 0};
    Vector2 velocity = Vector2{0, 0};
    float rotation = Config::Ship::START_ROTATION;
    const float baseSpeed = Config::Ship::BASE_SPEED;
    const float maxSpeed = Config::Ship::MAX_SPEED;
    const float acceleration = Config::Ship::ACCELERATION;
    const float dragAmount = Config::Ship::DRAG;
    const float rotationSpeed = Config::Ship::ROTATION_SPEED;

    float simSpeed = 1.0f;

    std::vector<Vector2> vertices = {Vector2{10, 0}, Vector2{-10, -8}, Vector2{-10, 8}};

    // bullet control
    float shotsPerSecond = Config::Ship::SHOTS_PER_SECOND;
    std::chrono::system_clock::time_point lastShotTime = std::chrono::system_clock::now();
    std::vector<Projectile *> bullets;

    int shotsFired = 0;

public:
    Ship()
    {
        position = Vector2{0, 0};
        velocity = Vector2{0, 0};
    }

    Ship(Vector2 _position, float _simSpeed)
    {
        position = _position;
        velocity = Vector2{0, 0};
        simSpeed = _simSpeed;
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

    bool IsAlive();

    void SetAlive(bool _alive);

    Vector2 GetPosition() const;

    void SetPosition(Vector2 _position);

    Vector2 GetVelocity() const;

    void SetVelocity(Vector2 _velocity);

    float GetRotation() const;

    void Rotate(float amount);

    int GetShotsFired();

    std::vector<Projectile *> GetBullets();

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