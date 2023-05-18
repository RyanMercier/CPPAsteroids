#include "raylib.h"
#include "Game.h"
#include "NetworkController.h"

int rayCount = 48;

int main()
{
    NetworkController *controller = new NetworkController();
    Game game = Game(controller);
    Ship *player = game.getPlayer();

    BeginDrawing();

    while (!WindowShouldClose())
    {
        game.Run();

        std::list<Asteroid *> asteroids = game.getAsteroids();
        Vector2 playerPos = player->GetPosition();

        // Raycast for network inputs
        float deltaAngle = 360.0f / rayCount;

        for (int i = 0; i < rayCount; i++)
        {
            Vector3 rayVector = {cos(deltaAngle * i * PI / 180.0f), sin(deltaAngle * i * PI / 180.0f), 0};
            Ray r = {{playerPos.x, playerPos.y, 0}, rayVector};
            float shortestDistance = INFINITY;
            Vector2 closestPoint = playerPos;

            for (auto asteroid : asteroids)
            {
                Vector2 asteroidPos = asteroid->GetPosition();
                RayCollision col = GetRayCollisionSphere(r, Vector3{asteroidPos.x, asteroidPos.y, 0}, asteroid->GetRadius());

                if (col.hit && col.distance < shortestDistance)
                {
                    shortestDistance = col.distance - asteroid->GetRadius();
                    closestPoint = {col.point.x, col.point.y};
                }
            }

            DrawLine(playerPos.x, playerPos.y, closestPoint.x, closestPoint.y, RED);
        }

        ClearBackground(BLACK);
    }

    EndDrawing();
    return 0;
}