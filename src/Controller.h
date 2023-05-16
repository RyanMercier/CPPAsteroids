#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "raylib.h"
#include "Ship.h"

class Controller
{
    Ship *player;

public:
    Controller()
    {
    }

    Controller(Ship *_player)
    {
        player = _player;
    }

    Controller(Controller &copy)
    {
        player = copy.player;
    }

    ~Controller()
    {
    }

    void Update(float _deltaTime);
};

#endif
