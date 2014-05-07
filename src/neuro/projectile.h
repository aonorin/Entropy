#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "neuro/square.h"
#include "Global.h"

namespace Ogre {
    class OverlayContainer;
}

namespace neuro {

class Projectile
{
public:
    enum ProjectiveType{DeRez,Scramble,Ghost};
    Projectile(Vec2f pos, int type, Direction direction, int pid);
    ~Projectile();
    void move();
    int getID();
    Vec2f getPos();

private:
    Vec2f pos;
    Direction direction;
    float speed;
    int pid;
    Ogre::OverlayContainer* projectileIcon;
};

}

#endif // PROJECTILE_H
