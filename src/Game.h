#ifndef GAME_H
#define GAME_H

#include <vector>
#include <chrono>
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

    Controller *controller;
    Ship *player;
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
        player = new Ship(startPosition);
        controller = new KeyBoardController(player);
    }

    Game(Controller *_controller, bool _draw)
    {
        // draw = _draw;
        if (draw)
        {
            Initialize();
        }
        Vector2 startPosition = Vector2{screenWidth / 2.0f, screenHeight / 2.0f};
        player = new Ship(startPosition);
        controller = _controller;
        controller->player = player;
    }

    ~Game()
    {
        free(controller);
        free(player);
        for (int i = 0; i < asteroids.size(); i++)
        {
            free(asteroids[i]);
        }
    }

    Controller *getController()
    {
        return controller;
    }

    Ship *getPlayer()
    {
        return player;
    }

    std::vector<Asteroid *> getAsteroids()
    {
        return asteroids;
    }

    int getScore()
    {
        return score;
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
        if (!WindowShouldClose())
        {
            Update();
            if (draw)
            {
                Draw();
            }

            else if (!player->IsAlive())
            {
                CloseWindow();
                return score;
            }
        }

        else
        {
            CloseWindow();
        }

        return score;
    }

    void Close()
    {
        CloseWindow();
    }

    void HandleAsteroids(float _deltaTime);

    void Update();

    void Draw();
};

#endif