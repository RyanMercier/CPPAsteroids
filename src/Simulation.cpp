#include "Simulation.h"
#include <iostream>

void Simulation::Update()
{
    if (player->IsAlive())
    {
        std::cout << "1" << std::endl;
        game->Run();
        std::cout << "2" << std::endl;

        Vector2 playerPos = player->GetPosition();
        Vector2 playerVel = player->GetVelocity();

        // Update network inputs
        inputs.push_back(player->GetRotation() / 100.0 - 0.5);
        inputs.push_back(playerPos.x / 1000.0 - 0.5);
        inputs.push_back(playerPos.y / 1000.0 - 0.5);
        inputs.push_back(playerVel.x / 100.0 - 0.5);
        inputs.push_back(playerVel.y / 100.0 - 0.5);

        std::vector<double> asteroidInputs = GetAsteroidInputs(game, playerPos);
        inputs.insert(inputs.end(), asteroidInputs.begin(), asteroidInputs.end());

        // Make decision
        std::vector<double> outputs = network->activate(inputs);
        inputs.clear();

        // Interpret decision and act
        controller->UpdateInputs((outputs[0] > 0), (outputs[1] > 0), (outputs[2] > 0), (outputs[3] > 0));
    }

    else
    {
        // FITNESS FUNCTION
        network->fitness = game->getScore();
        game->Close();
    }
}

std::vector<double> Simulation::GetAsteroidInputs(Game *game, Vector2 playerPos)
{
    std::vector<double> result;

    // Raycast for network inputs
    float deltaAngle = 360.0f / rayCount;
    std::vector<Asteroid *> asteroids = game->getAsteroids();

    for (int j = 0; j < rayCount; j++)
    {
        Vector3 rayVector = {cos(deltaAngle * j * PI / 180.0f), sin(deltaAngle * j * PI / 180.0f), 0};
        Ray r = {{playerPos.x, playerPos.y, 0}, rayVector};
        float shortestDistance = 1000; // Arbitrary number in this case about screenwidth
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

        result.push_back(shortestDistance / 1000.0 - 0.5);
    }

    return result;
}