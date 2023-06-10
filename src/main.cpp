#include <vector>
#include <random>
#include <iostream>
#include "raylib.h"
#include "Game.h"
#include "NetworkController.h"
#include "NeuralNetwork.h"
#include "Population.h"

int rayCount = 12;
int numInputs = rayCount + 5;
int numHidden = rayCount + 5;
int numOutputs = 4;
double mutationRate = 0.01;
double crossoverRate = 0.7;
int populationSize = 10;
int numGenerations = 10;

int main()
{
    // Create initial population
    Population population = Population(populationSize, numInputs, numHidden, numOutputs, mutationRate, crossoverRate);

    /////////// ADD SORTING AND A FITNESS FUNCTION!!!

    // Evolutionary loop
    for (int generation = 0; generation < numGenerations; generation++)
    {
        // Evaluate fitness for each individual in the population
        for (int i = 0; i < populationSize; i++)
        {
            // Run game
            NetworkController *controller = new NetworkController();
            Game game = Game(controller, false);
            Ship *player = game.getPlayer();
            std::vector<double> inputs;

            while (player->IsAlive())
            {
                game.Run();

                Vector2 playerPos = player->GetPosition();
                Vector2 playerVel = player->GetVelocity();

                // Update network inputs
                inputs.push_back(player->GetRotation() / 100.0 - 0.5);
                inputs.push_back(playerPos.x / 1000.0 - 0.5);
                inputs.push_back(playerPos.y / 1000.0 - 0.5);
                inputs.push_back(playerVel.x / 100.0 - 0.5);
                inputs.push_back(playerVel.y / 100.0 - 0.5);

                // Raycast for network inputs
                float deltaAngle = 360.0f / rayCount;
                std::list<Asteroid *> asteroids = game.getAsteroids();

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

                    inputs.push_back(shortestDistance / 1000.0 - 0.5);
                }

                // Make decision
                std::vector<double> outputs = population.networks[i].activate(inputs);

                // for (int j = 0; j < numInputs; j++)
                // {
                //     std::cout << inputs[j] << std::endl;
                // }

                // std::cout << std::endl;

                // for (int j = 0; j < 4; j++)
                // {
                //     std::cout << outputs[j] << std::endl;
                // }

                inputs.clear();

                // Interpret decision and act
                controller->UpdateInputs((outputs[0] > 0), (outputs[1] > 0), (outputs[2] > 0), (outputs[3] > 0));
            }

            // FITNESS FUNCTION
            population.networks[i].fitness = game.getScore();
            game.Close();

            // GENETIC ALGORITHM
            population.Reproduce();
        }
    }

    return 0;
}