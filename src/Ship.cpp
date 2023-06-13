#include "Ship.h"

// Actions
void Ship::TurnLeft(float _deltaTime)
{
    Rotate(-rotationSpeed * _deltaTime);
}

void Ship::TurnRight(float _deltaTime)
{
    Rotate(rotationSpeed * _deltaTime);
}

void Ship::Accelerate(float _deltaTime)
{
    // Apply ship acceleration in the direction of its facing angle
    Vector2 accelerationVector = Vector2{
        acceleration * cosf(rotation * PI / 180),
        acceleration * sinf(rotation * PI / 180)};

    velocity = Vector2Add(velocity, Vector2Scale(accelerationVector, _deltaTime));
}

void Ship::Drag(float _deltaTime)
{
    // Apply negative acceleration in the direction of its movement angle
    Vector2 dragVector = Vector2Normalize(velocity);
    dragVector = Vector2Scale(dragVector, -dragAmount);
    velocity = Vector2Add(velocity, Vector2Scale(dragVector, _deltaTime));

    // Check if velocity magnitude is less than threshold, and if so, set velocity to zero
    // This prevents drag from causing the ship to move backwards
    if (Vector2Length(velocity) < 1)
    {
        velocity = Vector2{0.0f, 0.0f};
    }
}

void Ship::Move(float _deltaTime)
{
    if (Vector2Length(velocity) > 0)
    {
        // add base speed
        Vector2 moveVector = Vector2Normalize(velocity);
        moveVector = Vector2Scale(moveVector, baseSpeed);
        moveVector = Vector2Add(moveVector, velocity);

        // limit max speed
        moveVector = Vector2ClampValue(moveVector, 0, maxSpeed);

        position = Vector2Add(position, Vector2Scale(moveVector, _deltaTime));
    }
}

void Ship::Shoot()
{
    // check fire rate
    float shotTime = 1.0f / shotsPerSecond;
    auto currentTime = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = currentTime - lastShotTime;
    if (elapsed_seconds.count() >= shotTime)
    {
        bullets.push_back(new Projectile(position, rotation));
        lastShotTime = currentTime;
    }
}

// Helpers

void Ship::HandleProjectiles(float _deltaTime)
{
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        if (!(*it)->IsAlive())
        {
            it = bullets.erase(it);
        }
        else
        {
            (*it)->Update(_deltaTime);
            (*it)->Draw();
            ++it;
        }
    }
}

void Ship::HandleScreenWrap()
{
    // handle screenwrapping
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    if (position.x < 0)
    {
        position.x = screenWidth;
    }

    if (position.x > screenWidth)
    {
        position.x = 0;
    }

    if (position.y < 0)
    {
        position.y = screenHeight;
    }

    if (position.y > screenHeight)
    {
        position.y = 0;
    }
}

std::vector<Vector2> Ship::GetVertices() const
{
    // rotate triangle
    Vector2 v1 = Vector2Rotate(vertices[0], rotation * PI / 180);
    Vector2 v2 = Vector2Rotate(vertices[1], rotation * PI / 180);
    Vector2 v3 = Vector2Rotate(vertices[2], rotation * PI / 180);

    // translate to position
    return {Vector2Add(v1, position), Vector2Add(v2, position), Vector2Add(v3, position)};
}

// Update

void Ship::Update(float _deltaTime)
{
    HandleScreenWrap();
    HandleProjectiles(_deltaTime);
    Drag(_deltaTime);
    Move(_deltaTime);
}

void Ship::Draw()
{
    if (alive)
    {
        std::vector<Vector2> verts = GetVertices();
        DrawTriangleLines(verts[0], verts[1], verts[2], RAYWHITE);
    }
}