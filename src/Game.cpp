#include "Game.h"

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
        // check asteroid rate
        auto currentTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = currentTime - lastAsteroidTime;
        if (elapsed_seconds.count() >= asteroidUpdateTime)
        {
            // make sure to spawn out of bounds
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

    // Check for Collisions and Clean Up Asteroids
    for (auto i = asteroids.begin(); i != asteroids.end();)
    {
        if (!(*i)->IsAlive())
        {
            delete *i;
            i = asteroids.erase(i);
        }
        else
        {
            // Collsion Check
            float asteroidRadius = (*i)->GetRadius();

            // Check for bullet Collisions
            std::list<Projectile *> bullets = player->GetBullets();
            for (Projectile *p : bullets)
            {
                // if bullet hits asteroid
                if (Vector2Distance((*i)->GetPosition(), p->GetPosition()) <= asteroidRadius)
                {
                    (*i)->SetAlive(false);
                    p->SetAlive(false);

                    if (asteroidRadius > 25)
                    {
                        // handle splitting
                        int splitCount = GetRandomValue(2, 3);
                        for (int j = 0; j < splitCount; j++)
                        {
                            asteroids.push_back(new Asteroid((*i)->GetPosition(), (*i)->GetDirection() + GetRandomValue(-45, 45), (*i)->GetSpeed(), ((*i)->GetRadius() / splitCount) + GetRandomValue(0, 10)));
                        }
                    }

                    if (player->IsAlive())
                    {
                        score++;
                    }
                }
            }

            // Check for Player Collisions
            if (Vector2Distance((*i)->GetPosition(), player->GetPosition()) <= asteroidRadius + 2 && player->IsAlive())
            {
                player->SetAlive(false);
                (*i)->SetAlive(false);

                // handle splitting
                if (asteroidRadius > 20)
                {
                    int splitCount = GetRandomValue(5, 15);
                    for (int j = 0; j < splitCount; j++)
                    {
                        asteroids.push_back(new Asteroid((*i)->GetPosition(), GetRandomValue(0, 360), (*i)->GetSpeed() + Vector2Length(player->GetVelocity()), ((*i)->GetRadius() / splitCount) + GetRandomValue(5, 15)));
                    }
                }
            }

            (*i)->Update(_deltaTime);
            i++;
        }
    }
}

void Game::Update()
{
    float deltaTime = GetFrameTime();

    if (player->IsAlive())
    {
        controller->Update(deltaTime);
        player->Update(deltaTime);
    }

    HandleAsteroids(deltaTime);
}

void Game::Draw()
{
    BeginDrawing();
    ClearBackground(backgroundColor);
    player->Draw();
    for (Asteroid *a : asteroids)
    {
        a->Draw();
    }

    DrawText(TextFormat("SCORE: %i", score), 50, 50, 24, RAYWHITE);

    EndDrawing();
}