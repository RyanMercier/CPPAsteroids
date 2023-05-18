#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include "Controller.h"

class NetworkController : public Controller
{
    bool forward = false;
    bool left = false;
    bool right = false;
    bool shoot = false;

public:
    NetworkController()
    {
    }

    NetworkController(Ship *_player)
    {
        player = _player;
    }

    NetworkController(Controller &copy)
    {
        player = copy.player;
    }

    void UpdateInputs(bool _forward, bool _left, bool _right, bool _shoot)
    {
    }

    virtual void Update(float _deltaTime)
    {
        // Handle Inputs

        if (forward)
        {
            player->Accelerate(_deltaTime);
        }

        if (left)
        {
            player->TurnLeft(_deltaTime);
        }

        if (right)
        {
            player->TurnRight(_deltaTime);
        }

        if (shoot)
        {
            player->Shoot();
        }
    }
};

#endif