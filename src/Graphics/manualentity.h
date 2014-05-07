#ifndef MANUALENTITY_H
#define MANUALENTITY_H

#include "OGRE/OgreSceneNode.h"
#include "manualobject.h"
#include "OGRE/OgreSceneManager.h"
#include "entity.h"
#include "Core/global.h"
#include "boost/signals2.hpp"
//typedef unsigned short uint16;

namespace graphics {

//class Entity;
class Material;

class ManualEntity : public ManualObject
{
public:
    typedef boost::shared_ptr<ManualObject> ptr;
    // manualEntity();
    ManualEntity(Ogre::SceneManager* sceneManager, entropy::uint16 numLod, Ogre::SceneNode *attachTo);
    ManualEntity(Ogre::SceneManager* sceneManager, entropy::uint16 numLod = 1);
    ~ManualEntity();

    void reInitGraphic(Material* mat,Ogre::Vector3Ptr vertices, Ogre::Vector3Ptr normals,voxel::Triangle::vectorPtr triangles);
    Ogre::SceneNode* getNode();
    graphics::Entity::ptr entity();
    void makeVisible();

    static boost::signals2::signal<void (ManualEntity* ent,entropy::uint32 id)> entAdded;
    static boost::signals2::signal<void (ManualEntity* ent,entropy::uint32 id)> entRemoved;

private:
//    Entity *m_entity;
    Entity::ptr m_entity;
    Ogre::SceneNode* m_node;
    Ogre::SceneManager* sceneManager;
};

}
#endif // MANUALENTITY_H
