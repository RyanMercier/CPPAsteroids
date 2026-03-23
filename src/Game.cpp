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
    return (score + 1.0f) / (player->GetShotsFired() + 1.0f);
}

int Game::GetShotsFired()
{
    return player->GetShotsFired();
}

void Game::Initialize()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(Config::Screen::WIDTH, Config::Screen::HEIGHT, "C++ Asteroids by Ryan Mercier");

    backgroundColor = Color{0, 0, 0, 0};
}

int Game::Run(float simSpeed)
{
    if (player->IsAlive())
    {
        Update(simSpeed);
        if (draw && (ownsWindow ? !WindowShouldClose() : true))
        {
            Draw();
        }
    }

    return score;
}

void Game::Close()
{
    if (draw && ownsWindow)
    {
        CloseWindow();
    }
}

void Game::SpawnAsteroids(float deltaTime)
{
    asteroidSpawnTimer += deltaTime;
    if ((int)asteroids.size() < maxAsteroids)
    {
        if (asteroidSpawnTimer >= asteroidUpdateTime)
        {
            int buf = (int)Config::Asteroid::SPAWN_BUFFER;
            int exc = (int)Config::Asteroid::SPAWN_EXCLUSION;
            float xPos = GetRandomValue(-buf, Config::Screen::WIDTH + buf);
            float yPos = GetRandomValue(-buf, Config::Screen::HEIGHT + buf);
            while ((xPos > -exc && xPos < Config::Screen::WIDTH + exc) && (yPos > -exc && yPos < Config::Screen::HEIGHT + exc))
            {
                xPos = GetRandomValue(-buf, Config::Screen::WIDTH + buf);
                yPos = GetRandomValue(-buf, Config::Screen::HEIGHT + buf);
            }

            asteroids.push_back(new Asteroid(Vector2{xPos, yPos}, GetRandomValue(-90, 90), GetRandomValue(50, 300), GetRandomValue(10, 100)));
            asteroidSpawnTimer = 0.0f;
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

                    if (asteroidRadius > Config::Asteroid::MIN_SPLIT_RADIUS)
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
                }

                bullet++;
            }

            if (Vector2Distance(asteroid->GetPosition(), player->GetPosition()) <= asteroidRadius + Config::Asteroid::COLLISION_PADDING && player->IsAlive())
            {
                player->SetAlive(false);
                asteroid->SetAlive(false);

                if (asteroidRadius > Config::Asteroid::COLLISION_SPLIT_MIN_RADIUS)
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
    if (score >= lastDifficultyScore + Config::Asteroid::DIFFICULTY_INTERVAL)
    {
        lastDifficultyScore = score;
        asteroids.push_back(new Asteroid(Vector2{-25, -25}, 45, GetRandomValue(50, 300), GetRandomValue(10, 100)));
        maxAsteroids++;
    }

    SpawnAsteroids(deltaTime);
    HandleCollisions(deltaTime);
}

float Game::GetGameTime()
{
    return gameTime;
}

void Game::Update(float simSpeed)
{
    // Use GetFrameTime() when a window is open, fixed timestep otherwise
    float dt = draw ? GetFrameTime() : fixedTimestep;
    float scaledDt = simSpeed * dt;
    gameTime += scaledDt;

    if (player->IsAlive())
    {
        controller->Update(scaledDt);
        player->Update(scaledDt);
    }

    HandleAsteroids(scaledDt);
}

void Game::Draw()
{
    if (draw)
    {
        // When we own the window, manage the full frame ourselves.
        // When main owns the window (training mode), just draw game content -
        // main handles BeginDrawing/EndDrawing so it can add the stats panel.
        if (ownsWindow)
        {
            BeginDrawing();
            ClearBackground(backgroundColor);
        }

        player->Draw();
        for (const auto &asteroid : asteroids)
        {
            asteroid->Draw();
        }

        DrawText(TextFormat("SCORE: %i", score), 50, 50, 24, RAYWHITE);

        if (ownsWindow)
        {
            EndDrawing();
        }
    }
}
