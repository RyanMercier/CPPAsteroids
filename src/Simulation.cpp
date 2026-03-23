#include "Simulation.h"
#include "Config.h"

bool Simulation::IsAlive()
{
    return game->IsAlive();
}

int Simulation::GetScore()
{
    return game->GetScore();
}

float Simulation::GetLifeSpan()
{
    return game->GetGameTime();
}

int Simulation::GetHitrate()
{
    return game->GetHitrate();
}

int Simulation::GetShotsFired()
{
    return game->GetShotsFired();
}

void Simulation::Update()
{
    if (game->IsAlive())
    {
        game->Run(*simSpeedPtr);

        Vector2 playerPos = game->GetPlayerPosition();
        Vector2 playerVel = game->GetPlayerVelocity();

        // Update network inputs - all normalized to [-1, 1]
        float halfW = Config::Screen::WIDTH / 2.0f;
        float halfH = Config::Screen::HEIGHT / 2.0f;
        inputs.push_back(game->GetPlayerRotation() / 180.0 - 1.0);    // [0,360] -> [-1,1]
        inputs.push_back(playerPos.x / halfW - 1.0);                  // [0,WIDTH] -> [-1,1]
        inputs.push_back(playerPos.y / halfH - 1.0);                  // [0,HEIGHT] -> [-1,1]
        inputs.push_back(playerVel.x / Config::Ship::MAX_SPEED);      // [-MAX,MAX] -> [-1,1]
        inputs.push_back(playerVel.y / Config::Ship::MAX_SPEED);      // [-MAX,MAX] -> [-1,1]

        std::vector<double> asteroidInputs = GetAsteroidInputs(playerPos);
        inputs.insert(inputs.end(), asteroidInputs.begin(), asteroidInputs.end());

        // Make decision
        std::vector<double> outputs = network->activate(inputs);
        inputs.clear();

        // Interpret decision and act
        game->GetController()->UpdateInputs((outputs[0] > 0), (outputs[1] > 0), (outputs[2] > 0), (outputs[3] > 0));
    }
}

std::vector<double> Simulation::GetAsteroidInputs(Vector2 playerPos)
{
    std::vector<double> result;

    // Raycast for network inputs - relative to ship rotation
    float deltaAngle = 360.0f / rayCount;
    float playerRotation = game->GetPlayerRotation();
    std::vector<Asteroid *> asteroids = game->GetAsteroids();

    for (int j = 0; j < rayCount; j++)
    {
        float rayAngle = (playerRotation + deltaAngle * j) * PI / 180.0f;
        Vector3 rayVector = {cos(rayAngle), sin(rayAngle), 0};
        Ray r = {{playerPos.x, playerPos.y, 0}, rayVector};
        float shortestDistance = Config::Training::RAYCAST_MAX_DIST;
        Vector2 closestPoint = playerPos;

        for (auto asteroid : asteroids)
        {
            Vector2 asteroidPos = asteroid->GetPosition();
            RayCollision col = GetRayCollisionSphere(r, Vector3{asteroidPos.x, asteroidPos.y, 0}, asteroid->GetRadius());

            if (col.hit && col.distance < shortestDistance)
            {
                shortestDistance = abs(col.distance);
                closestPoint = {col.point.x, col.point.y};
            }
        }

        float halfMax = Config::Training::RAYCAST_MAX_DIST / 2.0f;
        result.push_back(shortestDistance / halfMax - 1.0);  // [0,MAX] -> [-1,1]
    }

    return result;
}