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

    std::chrono::system_clock::time_point simsStartTime;

public:
    Simulation(std::unique_ptr<NeuralNetwork> _network, int _rayCount, bool _draw)
        : network(std::move(_network)), rayCount(_rayCount)
    {
        game = std::make_unique<Game>(true, _draw);
        inputs = std::vector<double>();
    }

    ~Simulation()
    {
    }

    void Update();

    bool isAlive()
    {
        return game->IsAlive();
    }

    int GetScore()
    {
        return game->GetScore();
    }

    int GetLifeSpan()
    {
        auto currentTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = currentTime - simsStartTime;
        return elapsed_seconds.count();
    }

    int GetHitrate()
    {
        return game->GetHitrate();
    }

    std::vector<double> GetAsteroidInputs(Vector2 playerPos);
};

#endif