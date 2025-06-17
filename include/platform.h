#ifndef PLATFORM_H
#define PLATFORM_H

#include "structure.h"

class Platform : public Structure
{
    public:

        Platform(float x, float y, float width, float height);
        
        bool isPlatform() const override { return true; };
        bool isWall() const override { return false; };
};

#endif
