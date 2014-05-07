#ifndef ENTITY_H
#define ENTITY_H

#include "Moveable.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreMesh.h"
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubEntity.h>
typedef unsigned short uint16;
#include "Core/global.h"
//typedef unsigned short uint16;

//namespace Ogre
//{
//class Entity;
//class MovableObject;
//class Ogre::MeshPtr;
//}

namespace graphics {

class Entity : public Moveable
{
public:
    typedef boost::shared_ptr<Entity> ptr;
    Entity(Ogre::SceneManager *sceneManager, std::string src);
    Entity(Ogre::SceneManager *sceneManager, const Ogre::Mesh *_mesh);
    ~Entity();

    void visible(bool vis);
    bool visible();
    void subEntityVisible(entropy::uint16 ind, bool vis);
    bool subEntityVisible(entropy::uint16 ind);
//protected:
    Ogre::MovableObject *_movableObject();
    Ogre::Node* getParentNode();
private:
    Ogre::Entity *m_entity;
    Ogre::SceneManager *sceneManager;
};

}
#endif // ENTITY_H
