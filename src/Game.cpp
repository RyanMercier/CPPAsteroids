#include "Game.h"

bool Game::IsAlive()
{
    return player->IsAlive();
}

Vector2 Game::GetPlayerPosition()
{
    return player->GetPosition();
}

Vector2 Game::GetPlayerVelocity()
{
    return player->GetVelocity();
}

float Game::GetPlayerRotation()
{
    return player->GetRotation();
}

std::unique_ptr<Controller> &Game::GetController()
{
    return controller;
}

std::unique_ptr<Ship> &Game::GetPlayer()
{
    return player;
}

std::vector<Asteroid *> Game::GetAsteroids()
{
    return asteroids;
}

int Game::GetScore()
{
    return score;
}

float Game::GetHitrate()
{
    return (player->GetShotsFired() + 1.0f) / score;
}

void Game::Initialize()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(screenWidth, screenHeight, "C++ Asteroids by Ryan Mercier");
    // SetTargetFPS(60);

    backgroundColor = Color{0, 0, 0, 0};
}

int Game::Run(float simSpeed)
{
    if (player->IsAlive())
    {
        Update(simSpeed);
        if (draw && !WindowShouldClose())
        {
            Draw();
        }
    }

    return score;
}

void Game::Close()
{
    if (draw)
    {
        CloseWindow();
    }
}

void Game::SpawnAsteroids()
{
    // Spawn Asteroids
    if (asteroids.size() < maxAsteroids)
    {
        auto currentTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = currentTime - lastAsteroidTime;
        if (elapsed_seconds.count() >= asteroidUpdateTime)
        {
            float xPos = GetRandomValue(-100, screenWidth + 100);
            float yPos = GetRandomValue(-100, screenHeight + 100);
            while ((xPos > -50 && xPos < screenWidth + 50) && (yPos > -50 && yPos < screenHeight + 50))
            {
                xPos = GetRandomValue(-100, screenWidth + 100);
                yPos = GetRandomValue(-100, screenHeight + 100);
            }

            asteroids.push_back(new Asteroid(Vector2{xPos, yPos}, GetRandomValue(-90, 90), GetRandomValue(50, 300), GetRandomValue(10, 100)));
            lastAsteroidTime = currentTime;
        }
    }
}

void Game::HandleCollisions(float deltaTime)
{
    std::vector<Asteroid *> asteroidsToAdd;
    std::vector<Asteroid *> asteroidsToRemove;

    for (auto it = asteroids.begin(); it != asteroids.end(); ++it)
    {
        Asteroid *asteroid = *it;

        if (asteroid && !asteroid->IsAlive())
        {
            asteroidsToRemove.push_back(asteroid);
        }
        else if (asteroid && asteroid->IsAlive())
        {
            float asteroidRadius = asteroid->GetRadius();

            std::vector<Projectile *> bullets = player->GetBullets();
            for (auto bullet = bullets.begin(); bullet != bullets.end();)
            {
                Projectile *p = *bullet;

                if (Vector2Distance(asteroid->GetPosition(), p->GetPosition()) <= asteroidRadius)
                {
                    asteroid->SetAlive(false);
                    p->SetAlive(false);

                    if (asteroidRadius > 30)
                    {
                        int splitCount = GetRandomValue(2, 3);
                        for (int j = 0; j < splitCount; j++)
                        {
                            asteroidsToAdd.push_back(new Asteroid(asteroid->GetPosition(), asteroid->GetDirection() + GetRandomValue(-45, 45), asteroid->GetSpeed(), (asteroid->GetRadius() / splitCount) + GetRandomValue(0, 10)));
                        }
                    }

                    if (player->IsAlive())
                    {
                        score++;
                    }

                    // delete p;
                    // bullet = bullets.erase(bullet);
                }
                // else
                // {
                //     ++bullet;
                // }

                bullet++;
            }

            if (Vector2Distance(asteroid->GetPosition(), player->GetPosition()) <= asteroidRadius + 2 && player->IsAlive())
            {
                player->SetAlive(false);
                asteroid->SetAlive(false);

                if (asteroidRadius > 20)
                {
                    int splitCount = GetRandomValue(5, 15);
                    for (int j = 0; j < splitCount; j++)
                    {
                        asteroidsToAdd.push_back(new Asteroid(asteroid->GetPosition(), GetRandomValue(0, 360), asteroid->GetSpeed() + Vector2Length(player->GetVelocity()), (asteroidRadius / splitCount) + GetRandomValue(5, 15)));
                    }
                }
            }

            asteroid->Update(deltaTime);
        }
    }

    // Remove asteroids marked for deletion and delete them
    for (Asteroid *asteroidToRemove : asteroidsToRemove)
    {
        asteroids.erase(std::remove(asteroids.begin(), asteroids.end(), asteroidToRemove), asteroids.end());
        delete asteroidToRemove;
    }
    asteroidsToRemove.clear();

    // Add new asteroids
    for (Asteroid *asteroidToAdd : asteroidsToAdd)
    {
        asteroids.push_back(asteroidToAdd);
    }
    asteroidsToAdd.clear();
}

void Game::HandleAsteroids(float deltaTime)
{
    // Difficulty Control
    if (score >= lastDifficultyScore + 10)
    {
        lastDifficultyScore = score;
        // Spawn an asteroid headed for the center to discourage sitting in the middle
        asteroids.push_back(new Asteroid(Vector2{-25, -25}, 45, GetRandomValue(50, 300), GetRandomValue(10, 100)));
        maxAsteroids++;
    }

    SpawnAsteroids();
    HandleCollisions(deltaTime); // and update asteroids
}

void Game::Update(float simSpeed)
{
    if (player->IsAlive())
    {
        // this will cause a problem if draw is set to false
        controller->Update(simSpeed * GetFrameTime());
        player->Update(simSpeed * GetFrameTime());

        // controller->Update(simSpeed * GetFrameTime());
        // player->Update(simSpeed * GetFrameTime());
    }

    HandleAsteroids(simSpeed * GetFrameTime());
}

void Game::Draw()
{
    if (draw)
    {
        BeginDrawing();
        ClearBackground(backgroundColor);
        player->Draw();
        for (const auto &asteroid : asteroids)
        {
            asteroid->Draw();
        }

        DrawText(TextFormat("SCORE: %i", score), 50, 50, 24, RAYWHITE);

        EndDrawing();
    }
}