#include "Controller.h"

void Controller::Update(float _deltaTime)
{
    // Handle Inputs

    if (IsKeyDown(KEY_W))
    {
        player->Accelerate(_deltaTime);
    }

    if (IsKeyDown(KEY_A))
    {
        player->TurnLeft(_deltaTime);
    }

    if (IsKeyDown(KEY_D))
    {
        player->TurnRight(_deltaTime);
    }

    if (IsKeyReleased(KEY_SPACE))
    {
        player->Shoot();
    }
}