#ifndef GAME_H
#define GAME_H

#include <vector>
#include <chrono>
#include <memory>
#include <algorithm>
#include <raylib.h>
#include "Ship.h"
#include "Asteroid.h"
#include "KeyBoardController.h"
#include "NetworkController.h"

class Game
{

    // Window Settings
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    Color backgroundColor;
    bool draw = true;

    // Asteroid Control
    int maxAsteroids = 5;
    int lastDifficultyScore = 0; // the last score max asteroids was increased at
    int scorePerDifficultyIncrease = 10;
    double minAsteroidSpeed = 0.05;
    double maxAsteroidSpeed = 0.2;
    double asteroidUpdateTime = 0.1; // how many seconds between checking if another asteroid is neccessary
    std::chrono::system_clock::time_point lastAsteroidTime = std::chrono::system_clock::now();

    std::unique_ptr<Controller> controller;
    std::unique_ptr<Ship> player;
    std::vector<Asteroid *> asteroids;

    int score = 0;

public:
    Game()
    {
        if (draw)
        {
            Initialize();
        }
        Vector2 startPosition = Vector2{screenWidth / 2.0f, screenHeight / 2.0f};
        player = std::make_unique<Ship>(startPosition, 1.0f);
        controller = std::make_unique<KeyBoardController>(player.get());
    }

    Game(bool _network, bool _draw, float _simSpeed)
    {
        draw = _draw;
        if (draw)
        {
            Initialize();
        }
        Vector2 startPosition = Vector2{screenWidth / 2.0f, screenHeight / 2.0f};
        player = std::make_unique<Ship>(startPosition, _simSpeed);

        if (_network)
        {
            controller = std::make_unique<NetworkController>(player.get());
        }
        else
        {
            controller = std::make_unique<KeyBoardController>(player.get());
        }
    }

    ~Game()
    {
        for (int i = 0; i < asteroids.size(); i++)
        {
            delete asteroids[i];
        }

        Close();
    }

    bool IsAlive();

    Vector2 GetPlayerPosition();
    Vector2 GetPlayerVelocity();
    float GetPlayerRotation();

    std::unique_ptr<Controller> &GetController();
    std::unique_ptr<Ship> &GetPlayer();

    std::vector<Asteroid *> GetAsteroids();

    int GetScore();
    float GetHitrate();

    void Initialize();

    int Run(float simSpeed);

    void Close();

    void SpawnAsteroids();

    void HandleCollisions(float deltaTime);

    void HandleAsteroids(float deltaTime);

    void Update(float simSpeed);

    void Draw();
};

#endif