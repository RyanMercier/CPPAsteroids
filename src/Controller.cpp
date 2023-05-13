#include "Controller.h"

void Controller::Update()
{
    // Handle Inputs

    if (IsKeyDown(KEY_W))
    {
        player->Accelerate();
    }

    if (IsKeyDown(KEY_A))
    {
        player->TurnLeft();
    }

    if (IsKeyDown(KEY_D))
    {
        player->TurnRight();
    }

    if (IsKeyReleased(KEY_SPACE))
    {
        player->Shoot();
    }
}