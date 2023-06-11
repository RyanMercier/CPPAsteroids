#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include "Controller.h"
#include <iostream>

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
        std::cout << "controller alive" << std::endl;
    }

    NetworkController(Controller &copy)
    {
        player = copy.player;
    }

    void UpdateInputs(bool _forward, bool _left, bool _right, bool _shoot)
    {
        forward = _forward;
        left = _left;
        right = _right;
        shoot = _shoot;
    }

    virtual void Update(float _deltaTime)
    {
        // Handle Inputs
        std::cout << "8" << std::endl;
        if (forward)
        {
            player->Accelerate(_deltaTime);
            std::cout << "9" << std::endl;
        }

        if (left)
        {
            player->TurnLeft(_deltaTime);
            std::cout << "10" << std::endl;
        }

        if (right)
        {
            player->TurnRight(_deltaTime);
            std::cout << "11" << std::endl;
        }

        if (shoot)
        {
            player->Shoot();
            std::cout << "12" << std::endl;
        }
    }
};

#endif