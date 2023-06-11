#ifndef SIMULATION_H
#define SIMULATIOM_H

#include "raylib.h"
#include "Game.h"
#include "NetworkController.h"
#include "NeuralNetwork.h"

class Simulation
{
    NeuralNetwork *network;
    NetworkController *controller;
    Game *game;
    Ship *player;
    int rayCount = 0;
    std::vector<double> inputs;

public:
    Simulation(NeuralNetwork *_network, int _rayCount)
    {
        network = _network;
        controller = new NetworkController();
        game = new Game(controller, false);
        rayCount = _rayCount;
        player = game->getPlayer();
    }

    ~Simulation()
    {
        free(controller);
        free(game);
    }

    void Update();

    bool isAlive()
    {
        return player->IsAlive();
    }

    std::vector<double> GetAsteroidInputs(Game *game, Vector2 playerPos);
};

#endif