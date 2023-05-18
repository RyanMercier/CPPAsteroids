#ifndef GAME_H
#define GAME_H

#include <list>
#include <chrono>
#include <raylib.h>
#include "Ship.h"
#include "Asteroid.h"
#include "KeyBoardController.h"

class Game
{

    // Window Settings
    const int screenWidth = 1000;
    const int screenHeight = 1000;
    Color backgroundColor;

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
    std::list<Asteroid *> asteroids;

    int score = 0;

public:
    Game()
    {
        Initialize();
        controller = new KeyBoardController(player);
    }

    Game(Controller *_controller)
    {
        Initialize();
        controller = _controller;
        controller->player = player;
    }

    ~Game()
    {
    }

    Controller *getController()
    {
        return controller;
    }

    Ship *getPlayer()
    {
        return player;
    }

    std::list<Asteroid *> getAsteroids()
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
        Vector2 startPosition = Vector2{screenWidth / 2.0f, screenHeight / 2.0f};
        player = new Ship(startPosition);
    }

    int Run()
    {
        if (!WindowShouldClose())
        {
            Update();
            Draw();
        }

        else
        {
            CloseWindow();
        }

        return score;
    }

    void HandleAsteroids(float _deltaTime);

    void Update();

    void Draw();
};

#endif