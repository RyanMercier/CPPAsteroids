#ifndef ASTEROID_H
#define ASTEROID_H

#include "raylib.h"
#include "raymath.h"

class Asteroid
{
    bool alive = true;
    Vector2 position;
    Vector2 velocity;
    float radius = 1.5;
    float rotation;

public:
    Asteroid(Vector2 _position, float _direction, float speed, float _radius)
    {
        position = _position;
        velocity.x += speed * cos(_direction * PI / 180);
        velocity.y += speed * sin(_direction * PI / 180);
        radius = _radius;
        rotation = GetRandomValue(0, 1800) / 10;
    }

    Vector2 GetPosition()
    {
        return position;
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

    void Update()
    {
        position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        if (position.x < -100 || position.x > screenWidth + 100 || position.y < -100 || position.y > screenHeight + 100)
        {
            alive = false;
        }
    }

    void Draw()
    {
        DrawPolyLines(position, 5 + (int)(radius / 5), radius, rotation, RAYWHITE);
    }
};

#endif