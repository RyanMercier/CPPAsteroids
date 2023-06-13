#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "raylib.h"
#include "Ship.h"

class Controller
{
public:
    Ship *player;

    Controller()
    {
    }

    virtual ~Controller()
    {
    }

    virtual void Update(float _deltaTime) = 0;

    virtual void UpdateInputs(bool _forward, bool _left, bool _right, bool _shoot)
    {
    }
};

#endif
