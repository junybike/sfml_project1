#ifndef WALL_H
#define WALL_H

#include "structure.h"

class Wall : public Structure
{
    public:

        Wall(float x, float y, float width, float height);

        bool isPlatform() const override { return false; };
        bool isWall() const override { return true; };
};

#endif