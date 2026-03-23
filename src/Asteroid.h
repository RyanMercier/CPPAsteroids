#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"
#include "raymath.h"
#include "Config.h"

class Asteroid
{
    bool alive = true;
    Vector2 position;
    float direction;
    float speed;
    Vector2 velocity = {0, 0};
    float radius = 1.5;
    float rotation;

public:
    Asteroid(Vector2 _position, float _direction, float _speed, float _radius)
    {
        position = _position;
        direction = _direction;
        velocity.x += _speed * cos(_direction * PI / 180);
        velocity.y += _speed * sin(_direction * PI / 180);
        speed = _speed;
        radius = _radius;
        rotation = GetRandomValue(0, 1800) / 10;
    }

    Vector2 GetPosition()
    {
        return position;
    }

    float GetDirection()
    {
        return direction;
    }

    float GetSpeed()
    {
        return speed;
    }

    float GetRadius()
    {
        return radius;
    }

    bool IsAlive()
    {
        return alive;
    }

    void SetAlive(bool _alive)
    {
        alive = _alive;
    }

    void Update(float deltaTime)
    {
        position = Vector2Add(position, Vector2Scale(velocity, deltaTime));

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        float buf = Config::Asteroid::OFFSCREEN_BUFFER;
        if (position.x < -buf || position.x > screenWidth + buf || position.y < -buf || position.y > screenHeight + buf)
        {
            alive = false;
        }
    }

    void Draw()
    {
        DrawPolyLines(position, Config::Asteroid::POLY_BASE_SIDES + (int)(radius / Config::Asteroid::POLY_RADIUS_DIVISOR), radius, rotation, RAYWHITE);
    }
};

#endif