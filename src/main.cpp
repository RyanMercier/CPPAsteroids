#include <raylib.h>
#include "Ship.h"
#include "Controller.h"

const int screenWidth = 1000;
const int screenHeight = 1000;
Color backgroundColor;
Controller *controller;
Ship *player;

void Initialize()
{
    InitWindow(screenWidth, screenHeight, "C++ Asteroids by Ryan Mercier");
    SetTargetFPS(60);

    backgroundColor = Color{0, 0, 0, 0};
    Vector2 startPosition = (Vector2){screenWidth / 2, screenHeight / 2};
    player = new Ship(startPosition);
    controller = new Controller(player);
}

void Update()
{
    controller->Update();
    player->Update();
}

void Draw()
{
    BeginDrawing();
    ClearBackground(backgroundColor);
    player->Draw();
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