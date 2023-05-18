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

    ~Controller()
    {
    }

    virtual void Update(float _deltaTime) = 0;
};

#endif
