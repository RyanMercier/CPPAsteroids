#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "Controller.h"

class KeyBoardController : public Controller
{
public:
    KeyBoardController(Ship *_player)
    {
        player = _player;
    }

    KeyBoardController(Controller &copy)
    {
        player = copy.player;
    }

    virtual void Update(float _deltaTime)
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

        if (IsKeyDown(KEY_SPACE))
        {
            player->Shoot();
        }
    }
};

#endif