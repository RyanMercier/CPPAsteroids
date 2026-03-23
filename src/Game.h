#ifndef GAME_H
#define GAME_H

#include <vector>
#include <chrono>
#include <memory>
#include <algorithm>
#include <raylib.h>
#include "Config.h"
#include "Ship.h"
#include "Asteroid.h"
#include "KeyBoardController.h"
#include "NetworkController.h"

class Game
{

    Color backgroundColor;
    bool draw = true;
    bool ownsWindow = true; // false when main already created the window

    // Asteroid Control
    int maxAsteroids = Config::Asteroid::INITIAL_MAX;
    int lastDifficultyScore = 0;
    double asteroidUpdateTime = Config::Asteroid::SPAWN_INTERVAL;
    std::chrono::system_clock::time_point lastAsteroidTime = std::chrono::system_clock::now();

    std::unique_ptr<Controller> controller;
    std::unique_ptr<Ship> player;
    std::vector<Asteroid *> asteroids;

    int score = 0;

    // For non-drawn sims, use a fixed timestep instead of GetFrameTime()
    float fixedTimestep = 1.0f / 60.0f;

public:
    Game()
    {
        if (draw)
        {
            Initialize();
        }
        Vector2 startPosition = Vector2{Config::Screen::WIDTH / 2.0f, Config::Screen::HEIGHT / 2.0f};
        player = std::make_unique<Ship>(startPosition, 1.0f);
        controller = std::make_unique<KeyBoardController>(player.get());
    }

    Game(bool _network, bool _draw, float _simSpeed, bool _ownsWindow = true)
    {
        draw = _draw;
        ownsWindow = _ownsWindow;
        if (draw && ownsWindow)
        {
            Initialize();
        }
        backgroundColor = Color{0, 0, 0, 0};
        Vector2 startPosition = Vector2{Config::Screen::WIDTH / 2.0f, Config::Screen::HEIGHT / 2.0f};
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
    int GetShotsFired();

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
