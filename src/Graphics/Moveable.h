#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "OGRE/OgreMovableObject.h"
#include "OGRE/OgreNode.h"
namespace graphics {

//class Ogre::Node;
class Moveable
{
    friend class Ogre::SceneNode;
//protected:
    virtual Ogre::MovableObject *_movableObject() = 0;
};

}

#endif // MOVEABLE_H
