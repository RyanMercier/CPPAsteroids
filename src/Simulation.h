#ifndef SIMULATION_H
#define SIMULATION_H

#include <memory>
#include <vector>
#include <chrono>
#include "raylib.h"
#include "Game.h"
#include "NetworkController.h"
#include "NeuralNetwork.h"

class Simulation
{
    std::unique_ptr<NeuralNetwork> network;
    std::unique_ptr<Game> game;
    std::unique_ptr<Controller> controller;
    int rayCount = 0;
    std::vector<double> inputs;
    float simSpeed;

    std::chrono::system_clock::time_point simsStartTime;

public:
    Simulation(std::unique_ptr<NeuralNetwork> _network, int _rayCount, bool _draw, float _simSpeed)
        : network(std::move(_network)), rayCount(_rayCount), simSpeed(_simSpeed)
    {
        game = std::make_unique<Game>(true, _draw, simSpeed);
        inputs = std::vector<double>();
    }

    ~Simulation()
    {
    }

    void Update();

    bool IsAlive();

    int GetScore();

    int GetLifeSpan();

    int GetHitrate();

    std::vector<double> GetAsteroidInputs(Vector2 playerPos);
};

#endif