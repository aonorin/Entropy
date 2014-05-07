#include "manualentity.h"
#include "OGRE/OgreStringConverter.h"
#include "entity.h"
#include "Core/Entropy.h"

using namespace graphics;

boost::signals2::signal<void (ManualEntity* ent,entropy::uint32 id)> ManualEntity::entAdded;
boost::signals2::signal<void (ManualEntity* ent,entropy::uint32 id)> ManualEntity::entRemoved;

ManualEntity::ManualEntity(Ogre::SceneManager* sceneManager, uint16 numLod)
    : ManualObject(Entropy::getSingletonPtr()->mSceneMgr, numLod)
    //    , m_entity(0)
    , m_node(0)
{
    entAdded(this,id());
    m_node = Entropy::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::String("nO").append( Ogre::StringConverter::toString((int) id())));
}

ManualEntity::ManualEntity(Ogre::SceneManager* sceneManager, uint16 numLod, Ogre::SceneNode *attachTo)
    : ManualObject(Entropy::getSingletonPtr()->mSceneMgr, numLod)
    //    , m_entity(0)
    , m_node(0)
{
    entAdded(this,id());
    m_node = attachTo->createChildSceneNode(Ogre::String("nO").append( Ogre::StringConverter::toString((int)id())));
}

ManualEntity::~ManualEntity()
{
    std::cout << "ManualEntity::~ManualEntity()" << std::endl;
    //if (m_entity)
    //{
    //    Entropy::getSingletonPtr()->addQueuedDeleteManualEntity(m_entity);
    //}
    entRemoved(this,id());
    if(m_node)
    {
        delete m_node;
    }
}

void ManualEntity::reInitGraphic(Material *mat, Ogre::Vector3Ptr vertices, Ogre::Vector3Ptr normals, voxel::Triangle::vectorPtr triangles)
{
    ManualObject::reInitGraphic(mat,vertices,normals,triangles);
    if (lods()->numTriangles > 0)
    {
        std::cout << "lods()->numTriangles: " << lods()->numTriangles << std::endl;
        m_entity = graphics::Entity::ptr(new graphics::Entity(Entropy::getSingletonPtr()->mSceneMgr, Ogre::String("mO").append( Ogre::StringConverter::toString((int)id()))));
        if(m_node != 0)
        {
            //            std::cout << "Node not null " << m_node->getName() << std::endl;
            try
            {
                //                m_node->setVisible(false);
                m_node->attachObject(m_entity->_movableObject());
//                float scale = 8.0;
                m_node->scale(GRAPHICS_SCALE, GRAPHICS_SCALE, GRAPHICS_SCALE);
                m_node->setPosition(m_node->getPosition() * GRAPHICS_SCALE);
            }
            catch(std::exception &e)
            {
                std::cerr << "Exception attaching moveableObject: " << e.what() << std::endl;
            }
            catch(...)
            {
                std::cerr << "Unknown Exception attaching moveableObject" << std::endl;
            }
            //            std::cout << "Post m_node->attachObject " << m_node->getName() << std::endl;
        }
        else
        {
            std::cerr << "ManualEntity Node was null" << std::endl;
            Entropy::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::String("nO").append( Ogre::StringConverter::toString((int)id())));
            try
            {
                m_node->attachObject(m_entity->_movableObject());
//                int scale = 12;
                m_node->scale(GRAPHICS_SCALE,GRAPHICS_SCALE,GRAPHICS_SCALE);
                m_node->setPosition(m_node->getPosition()*GRAPHICS_SCALE);
            }
            catch(std::exception &e)
            {
                std::cerr << "Exception attaching moveableObject: " << e.what() << std::endl;
            }
            catch(...)
            {
                std::cerr << "Unknown Exception attaching moveableObject" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "ZERO TRIANGLES? lods()->numTriangles: " << lods()->numTriangles << std::endl;
    }
    // m_node->setPosition(m_aabb.getMinimum());
    //    std::cout << "End  void ManualEntity::reInitGraphic(Material *mat)" << m_node->getName() << std::endl;
}

Ogre::SceneNode* ManualEntity::getNode()
{
    return m_node;
}

graphics::Entity::ptr ManualEntity::entity()
{
    return m_entity;
}

void ManualEntity::makeVisible()
{
    m_node->setVisible(true);
}
