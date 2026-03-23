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
    float *simSpeedPtr;

    std::chrono::system_clock::time_point simsStartTime = std::chrono::system_clock::now();

public:
    Simulation(std::unique_ptr<NeuralNetwork> _network, int _rayCount, bool _draw, float *_simSpeedPtr, bool _ownsWindow = true)
        : network(std::move(_network)), rayCount(_rayCount), simSpeedPtr(_simSpeedPtr)
    {
        game = std::make_unique<Game>(true, _draw, *_simSpeedPtr, _ownsWindow);
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

    int GetShotsFired();

    std::vector<double> GetAsteroidInputs(Vector2 playerPos);
};

#endif