#include <list>
#include <chrono>
#include <raylib.h>
#include "Ship.h"
#include "Asteroid.h"
#include "Controller.h"

const int screenWidth = 1000;
const int screenHeight = 1000;
Color backgroundColor;
Controller *controller;
Ship *player;
std::list<Asteroid *> asteroids;

// Game Settings
int score = 0;
int difficulty = 0;
int maxAsteroids = 10;
double minAsteroidSpeed = 0.05;
double maxAsteroidSpeed = 0.2;

// Asteroid Control
std::chrono::_V2::system_clock::time_point lastAsteroidTime = std::chrono::system_clock::now();
double asteroidUpdateTime = 0.1; // how many seconds between checking if another asteroid is neccessary

void Initialize()
{
    InitWindow(screenWidth, screenHeight, "C++ Asteroids by Ryan Mercier");
    SetTargetFPS(60);

    backgroundColor = Color{0, 0, 0, 0};
    Vector2 startPosition = (Vector2){screenWidth / 2, screenHeight / 2};
    player = new Ship(startPosition);
    controller = new Controller(player);
}

void HandleAsteroids()
{
    // Difficulty Control
    if (score % 20 == 0)
    {
        maxAsteroids++;
    }

    // Spawn Asteroids
    if (asteroids.size() < maxAsteroids)
    {
        // check asteroid rate
        auto currentTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = currentTime - lastAsteroidTime;
        if (elapsed_seconds.count() >= asteroidUpdateTime)
        {
            int xPos = GetRandomValue(-50, screenWidth + 50);
            int yPos = GetRandomValue(-50, screenHeight + 50);
            while ((xPos > 0 && xPos < screenWidth) && (yPos > 0 && yPos < screenHeight))
            {
                xPos = GetRandomValue(-50, screenWidth + 50);
                yPos = GetRandomValue(-50, screenHeight + 50);
            }

            asteroids.push_back(new Asteroid((Vector2){xPos, yPos}, GetRandomValue(-90, 90), GetRandomValue(50, 300), GetRandomValue(10, 50)));
            lastAsteroidTime = currentTime;
        }
    }

    // Check for Collisions and Clean Up Asteroids
    std::list<Asteroid *>::iterator i = asteroids.begin();
    while (i != asteroids.end())
    {
        // Collsion Check
        float asteroidRadius = (*i)->GetRadius();

        std::list<Projectile *> bullets = player->GetBullets();
        for (Projectile *p : bullets)
        {
            // if bullet hits asteroid
            if (Vector2Distance((*i)->GetPosition(), p->GetPosition()) <= asteroidRadius)
            {
                (*i)->SetAlive(false);
                p->SetAlive(false);
                score++;
            }
        }

        if (!(*i)->IsAlive())
        {
            delete *i;
            asteroids.erase(i++);
        }
        else
        {
            // if asteroid hits player
            if (Vector2Distance((*i)->GetPosition(), player->GetPosition()) <= asteroidRadius + 2)
            {
                player->SetAlive(false);
            }

            (*i)->Update();
            i++;
        }
    }
}

void Update()
{
    controller->Update();
    player->Update();
    HandleAsteroids();
}

void Draw()
{
    BeginDrawing();
    ClearBackground(backgroundColor);
    player->Draw();
    for (Asteroid *a : asteroids)
    {
        a->Draw();
    }
    EndDrawing();
}

int main()
{
    Initialize();

    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }

    CloseWindow();
    return 0;
}