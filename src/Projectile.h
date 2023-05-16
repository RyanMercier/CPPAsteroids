#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "raylib.h"
#include "raymath.h"

class Projectile
{
    bool alive = true;
    Vector2 position;
    Vector2 velocity = { 0, 0 };
    float speed = 700;
    float radius = 1.5;

public:
    Projectile(Vector2 _position, float _direction)
    {
        position = _position;
        velocity.x += speed * cos(_direction * PI / 180);
        velocity.y += speed * sin(_direction * PI / 180);
    }

    Vector2 GetPosition()
    {
        return position;
    }

    bool IsAlive()
    {
        return alive;
    }

    void SetAlive(bool _alive)
    {
        alive = _alive;
    }

    void Update(float _deltaTime)
    {
        position = Vector2Add(position, Vector2Scale(velocity, _deltaTime));

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        if (position.x < 0 || position.x > screenWidth || position.y < 0 || position.y > screenHeight)
        {
            alive = false;
        }
    }

    void Draw()
    {
        DrawCircle(position.x, position.y, radius, RAYWHITE);
    }
};

#endif