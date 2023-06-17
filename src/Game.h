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
    int maxAsteroids = 20;
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
        player = std::make_unique<Ship>(startPosition);
        controller = std::make_unique<KeyBoardController>(player.get());
    }

    Game(bool network, bool _draw)
    {
        draw = _draw;
        if (draw)
        {
            Initialize();
        }
        Vector2 startPosition = Vector2{screenWidth / 2.0f, screenHeight / 2.0f};
        player = std::make_unique<Ship>(startPosition);

        if (network)
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

    bool IsAlive()
    {
        return player->IsAlive();
    }

    Vector2 GetPlayerPosition()
    {
        return player->GetPosition();
    }

    Vector2 GetPlayerVelocity()
    {
        return player->GetVelocity();
    }

    float GetPlayerRotation()
    {
        return player->GetRotation();
    }

    std::unique_ptr<Controller> &GetController()
    {
        return controller;
    }

    std::unique_ptr<Ship> &GetPlayer()
    {
        return player;
    }

    std::vector<Asteroid *> GetAsteroids()
    {
        return asteroids;
    }

    int GetScore()
    {
        return score;
    }

    float GetHitrate()
    {
        return player->GetShotsFired() + 1 / score;
    }

    void Initialize()
    {
        SetConfigFlags(FLAG_VSYNC_HINT);
        InitWindow(screenWidth, screenHeight, "C++ Asteroids by Ryan Mercier");
        // SetTargetFPS(60);

        backgroundColor = Color{0, 0, 0, 0};
    }

    int Run()
    {
        if (player->IsAlive())
        {
            Update();
            if (draw && !WindowShouldClose())
            {
                Draw();
            }
        }

        return score;
    }

    void Close()
    {
        if (draw)
        {
            CloseWindow();
        }
    }

    void SpawnAsteroids();

    void HandleCollisions(float _deltaTime);

    void HandleAsteroids(float _deltaTime);

    void Update();

    void Draw();
};

#endif