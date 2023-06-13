#include "Game.h"
#include <iostream>

void Game::HandleAsteroids(float _deltaTime)
{
    // Difficulty Control
    if (score >= lastDifficultyScore + 20)
    {
        lastDifficultyScore = score;
        maxAsteroids++;
    }

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

                    if (asteroidRadius > 25)
                    {
                        int splitCount = GetRandomValue(2, 3);
                        for (int j = 0; j < splitCount; j++)
                        {
                            asteroids.push_back(new Asteroid(asteroid->GetPosition(), asteroid->GetDirection() + GetRandomValue(-45, 45), asteroid->GetSpeed(), (asteroid->GetRadius() / splitCount) + GetRandomValue(0, 10)));
                        }
                    }

                    if (player->IsAlive())
                    {
                        score++;
                    }

                    delete p;
                    bullet = bullets.erase(bullet);
                }
                else
                {
                    ++bullet;
                }
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
                        asteroids.push_back(new Asteroid(asteroid->GetPosition(), GetRandomValue(0, 360), asteroid->GetSpeed() + Vector2Length(player->GetVelocity()), (asteroidRadius / splitCount) + GetRandomValue(5, 15)));
                    }
                }
            }

            asteroid->Update(_deltaTime);
        }
    }

    // Remove asteroids marked for deletion and delete them
    for (Asteroid *asteroidToRemove : asteroidsToRemove)
    {
        asteroids.erase(std::remove(asteroids.begin(), asteroids.end(), asteroidToRemove), asteroids.end());
        delete asteroidToRemove;
    }
}

void Game::Update()
{
    if (player->IsAlive())
    {
        controller->Update(GetFrameTime());
        player->Update(GetFrameTime());
    }

    HandleAsteroids(GetFrameTime());
}

void Game::Draw()
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