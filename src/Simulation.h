#ifndef SIMULATION_H
#define SIMULATION_H

#include <memory>
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

public:
    Simulation(std::unique_ptr<NeuralNetwork> _network, int _rayCount)
        : network(std::move(_network)), rayCount(_rayCount)
    {
        game = std::make_unique<Game>(true, true);
        inputs = std::vector<double>();
    }

    ~Simulation()
    {
        std::cout << "Simulation Destructor" << std::endl;
    }

    void Update();

    bool isAlive()
    {
        return game->IsAlive();
    }

    std::vector<double> GetAsteroidInputs(Vector2 playerPos);
};

#endif