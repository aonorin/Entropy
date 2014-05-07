#include "entity.h"

using namespace graphics;

Entity::Entity(Ogre::SceneManager *sceneManager, std::string src)
    : m_entity(sceneManager->createEntity(src))
    , sceneManager(sceneManager)
{

}

Entity::Entity(Ogre::SceneManager *sceneManager, const Ogre::Mesh* _mesh)
    : m_entity(sceneManager->createEntity(_mesh->getName()))
    , sceneManager(sceneManager)
{
}

Entity::~Entity()
{
    sceneManager->destroyEntity(m_entity);
}

Ogre::MovableObject *Entity::_movableObject()
{
    return m_entity;
}

void Entity::visible(bool vis)
{
    m_entity->setVisible(vis);
}

bool Entity::visible()
{
    return m_entity->isVisible();
}

void Entity::subEntityVisible(entropy::uint16 ind, bool vis)
{
    m_entity->getSubEntity(ind)->setVisible(vis);
}

bool Entity::subEntityVisible(entropy::uint16 ind)
{
    return m_entity->getSubEntity(ind)->isVisible();
}

Ogre::Node* Entity::getParentNode()
{
    return m_entity->getParentNode();
}
