#include "actors.h"

// Entropy includes
#include "src/Physics/physicsobject.h"
#include "src/Physics/physics.h"
#include "src/Graphics/entity.h"
#include "src/Core/Entropy.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreMaterialManager.h"
#include "btBulletCollisionCommon.h"

namespace gameplay {

    namespace actors {

        void update()
        {
            //std::cout << "gameplay::actors::update()" << std::endl;
            // physics::World::world_upgrade_write_t lock(physics::world.mutex);
            // physics::World::world_write_t writeLock(lock);

            for(int i = 0; i < detail::actors.size(); ++i)
            {
                detail::actors[i]->update();
            }
        }

        void updateTransform()
        {
            //std::cout << "gameplay::actors::updateTransform()" << std::endl;
            // physics::World::world_read_t lock(physics::world.mutex);
            //physics::World::world_write_t writeLock(lock);

            for(int i = 0; i < detail::actors.size(); ++i)
            {
                detail::actors[i]->updateTransform();
            }
        }

        void setActors(std::vector<physics::PhysicsActor*> actorVector)
        {
            detail::actors = actorVector;
        }

        void addActor(physics::PhysicsActor* physicsActor)
        {
            detail::actors.push_back(physicsActor);
        }

        void removeActor(std::string name, bool deleteActor)
        {
            for(int i = 0; i < detail::actors.size(); ++i)
            {
                physics::PhysicsActor* actor = detail::actors.at(i);
                if(actor->getName().compare(name) == 0)
                {
                    detail::actors.erase(detail::actors.begin() + i);
                    if(deleteActor)
                        delete actor;

                    break;
                }
            }
        }

        void createActor(std::string name, std::string modelName, std::string materialName,
                            int x, int y, int z)
        {
            Ogre::SceneManager* sceneManager = Entropy::getSingletonPtr()->mSceneMgr;
            Ogre::Entity* entity = sceneManager->createEntity(modelName);
            entity->setMaterial(
                Ogre::MaterialPtr(
                    Ogre::MaterialManager::getSingletonPtr()->getByName(materialName)
                )
            );

            Ogre::SceneNode* node = sceneManager->getRootSceneNode()->createChildSceneNode(name);
            node->attachObject(entity);
            Ogre::AxisAlignedBox box = entity->getBoundingBox();

            btCollisionObject* object = physics::createCollisionBox(
                        physics::Vec3(box.getCenter()),
                        physics::Vec3(box.getSize() / 2)
            );

            physics::PhysicsActor* actor = new physics::PhysicsActor(name, object, node);
            node->setPosition(x, y, z);
            btTransform transform = object->getWorldTransform();
            transform.setOrigin(btVector3(x, y, z));
            object->setWorldTransform(transform);
            detail::actors.push_back(actor);
        }

        physics::PhysicsActor* getActor(std::string actorName)
        {
            for(int i = 0; i < detail::actors.size(); ++i)
            {
                if(actorName.compare(detail::actors[i]->getName()) == 0)
                {
                    return detail::actors[i];
                }
            }

            return 0;
        }

        void clearActors()
        {
            for(int i = 0; i < detail::actors.size(); ++i)
            {
                delete detail::actors[i];
            }

            detail::actors.clear();
        }

        namespace detail {
            std::vector<physics::PhysicsActor*> actors;
        } // detail namespace

    } // actors namespace

} // gameplay namespace
