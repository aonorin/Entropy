#include "fresnelwater.h"
#include "OGRE/OgreResourceGroupManager.h"
#include "OGRE/OgreMeshManager.h"
#include "Core/Entropy.h"
#include "OGRE/OgreHardwarePixelBuffer.h"
#include "Core/collisioncamera.h"
#include "Utility/Math.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderWindow.h"

namespace voxel
{
namespace environment
{
int FresnelWater::staticID = 0;

FresnelWater::FresnelWater(Ogre::Vector3 pos, Ogre::Real width, Ogre::Real height) :
    id(staticID++),
    pos(pos),
    width(width),
    height(height),
    setup(false),
    underWater(true),
    underwaterCompositor(0),
    waterPhysics(btVector3(pos.x, -30, pos.z), btVector3(width, pos.y + 30, height))
{
    setupWater();
}

void FresnelWater::updateWater()
{
    if(setup)
    {
        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->getByName("Examples/FresnelReflectionRefraction");
        if(material.get()->isLoaded())
        {
            Ogre::GpuProgramParametersSharedPtr params = material.get()->getTechnique(0)->getPass(0)->getVertexProgramParameters();
            Ogre::GpuProgramParametersSharedPtr fParams = material.get()->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
            if(params->_findNamedConstantDefinition("cFarDistance"))
            {
                std::cout << "FOUND!" << std::endl;
                params->setNamedConstant("cFarDistance",Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE);
            }

            Ogre::Vector3 ep(Entropy::getSingletonPtr()->mCameraMan->getPosition().x(),Entropy::getSingletonPtr()->mCameraMan->getPosition().y(),Entropy::getSingletonPtr()->mCameraMan->getPosition().z());
            if(params->_findNamedConstantDefinition("eyePositionM"))
            {
                params->setNamedConstant("eyePositionM",ep);
            }
            if(params->_findNamedConstantDefinition("lightPositionM"))
            {
                Ogre::Vector3 lp = Ogre::Vector3(0, utility::math::SQUARE_ROOT_ZERO_POINT_SEVEN_FIVE - 0.4, 0) * 2;
                lp += ep;
                params->setNamedConstant("lightPositionM",lp);
            }

            /*
            if(Entropy::getSingletonPtr()->mCameraMan->getPosition().y() >= pos.y)
            {
                if(underWater)
                {
                    mWater->setVisible(true);
                    mUnderWater->setVisible(false);
                    std::cout << "underwaterCompositor->setEnabled(false);" << std::endl;
                    underwaterCompositor->setEnabled(false);
                }
                underWater = false;
            }
            else
            {
                if(!underWater)
                {
                    mUnderWater->setVisible(true);
                    mWater->setVisible(false);
//                    Ogre::CompositorManager::getSingletonPtr()->addCompositor(Entropy::getSingletonPtr()->mRoot->getRenderSystem()->_getViewport(),"UnderwaterQuadComp");
//                    Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(Entropy::getSingletonPtr()->mRoot->getRenderSystem()->_getViewport(),"UnderwaterQuadComp",true);
                    std::cout << "underwaterCompositor->setEnabled(true);" << std::endl;
                    underwaterCompositor->setEnabled(true);

                }
                underWater = true;
            }*/
        }
        else
        {
            std::cout << "NOT FOUND!" << std::endl;
        }
    }
    else
    {
        std::cout << "not loaded" << std::endl;
    }
}

void FresnelWater::setupWater()
{
    setup = true;
    // create our water plane mesh
    mWaterPlane = new Ogre::MovablePlane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("water", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                  *mWaterPlane, width, height, 10, 10, true, 1, 3, 5, Ogre::Vector3::UNIT_Z);

    // create a water entity using our mesh, give it the shader material, and attach it to the origin
    mWater = Entropy::getSingletonPtr()->mSceneMgr->createEntity("Water", "water");
    mWater->setMaterialName("Examples/FresnelReflectionRefraction");
    waterNode = Entropy::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::String("waterplanenode").append( Ogre::StringConverter::toString(id)));
    waterNode->attachObject(mWater);
    waterNode->setPosition(pos);
    waterNode->attachObject(mWaterPlane);

    // create UNDER water plane mesh
    Ogre::Vector3 upsideDown(1,-1,1);
    mUnderWaterPlane = new Ogre::MovablePlane(upsideDown*Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("underwater", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                  *mUnderWaterPlane, width, height, 10, 10, true, 1, 3, 5, Ogre::Vector3::UNIT_Z);

    // create a water entity using our mesh, give it the shader material, and attach it to the origin
    mUnderWater = Entropy::getSingletonPtr()->mSceneMgr->createEntity("UnderWater", "underwater");
    mUnderWater->setMaterialName("EntropyUnderwater");
    mUnderWaterNode = Entropy::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::String("underwaterplanenode").append( Ogre::StringConverter::toString(id)));
    mUnderWaterNode->attachObject(mUnderWater);
    mUnderWaterNode->setPosition(pos + Ogre::Vector3(0, -0.5, 0));
    mUnderWaterNode->attachObject(mUnderWaterPlane);

    Ogre::CompositorManager &compMan = Ogre::CompositorManager::getSingleton();
    underwaterCompositor = compMan.addCompositor(Entropy::getSingletonPtr()->mWindow->getViewport(0),"UnderwaterQuadComp");
}

void FresnelWater::preRenderTargetUpdate(const Ogre::RenderTargetEvent &evt)
{

    //    material.get()->getTechnique(0)->getPass(0)->getVertexProgramParameters().get()->setNamedConstant("lightPosManual",Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE);

    /*
    mWater->setVisible(false);  // hide the water

    if (evt.source == mReflectionTarget)  // for reflection, turn on camera reflection and hide submerged entities
    {
        Entropy::getSingletonPtr()->mCameraMan->getCamera()->enableReflection(mWaterPlane);
        Ogre::SceneManager::MovableObjectIterator iterator = Entropy::getSingletonPtr()->mSceneMgr->getMovableObjectIterator("Entity");
        while(iterator.hasMoreElements())
        {
              Ogre::Entity* e = static_cast<Ogre::Entity*>(iterator.getNext());
              if(e->getParentNode()->getPosition().y<pos.y)
              {
                  e->setVisible(false);
                  submergedEnts.push_back(e);
              }
        }
    }
    else if (evt.source == mRefractionTarget)  // for refraction, hide surface entities
    {
        Ogre::SceneManager::MovableObjectIterator iterator = Entropy::getSingletonPtr()->mSceneMgr->getMovableObjectIterator("Entity");
        Ogre::Entity* e = static_cast<Ogre::Entity*>(iterator.getNext());
        if(e->getParentNode()->getPosition().y>=pos.y)
        {
            e->setVisible(false);
            surfaceEnts.push_back(e);
        }
    }
    */
}

void FresnelWater::postRenderTargetUpdate(const Ogre::RenderTargetEvent &evt)
{
    /*
    mWater->setVisible(true);  // unhide the water

    if (evt.source == mReflectionTarget)  // for reflection, turn off camera reflection and unhide submerged entities
    {
        Entropy::getSingletonPtr()->mCameraMan->getCamera()->disableReflection();
        std::vector<Ogre::Entity*>::const_iterator it = submergedEnts.begin();
        while(it!=submergedEnts.end())
        {
            (*it)->setVisible(true);
            ++it;
        }
    }
    else if (evt.source == mRefractionTarget)
    {
        std::vector<Ogre::Entity*>::const_iterator it = surfaceEnts.begin();
        while(it!=surfaceEnts.end())
        {
            (*it)->setVisible(true);
            ++it;
        }
    }
    submergedEnts.clear();
    surfaceEnts.clear();
    */
}

void FresnelWater::setEnabled(bool enabled)
{
    if(setup)
    {
        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->getByName("Examples/FresnelReflectionRefraction");
        if(material.get()->isLoaded() && underwaterCompositor)
        {
            underWater = enabled;
            Entropy::getSingletonPtr()->addQeueudFunction(boost::bind(&FresnelWater::deferredSetEnabled, this));
        }
    }
}

void FresnelWater::deferredSetEnabled()
{
    if(underWater)
    {
        mUnderWater->setVisible(true);
        mWater->setVisible(false);
//                    Ogre::CompositorManager::getSingletonPtr()->addCompositor(Entropy::getSingletonPtr()->mRoot->getRenderSystem()->_getViewport(),"UnderwaterQuadComp");
//                    Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(Entropy::getSingletonPtr()->mRoot->getRenderSystem()->_getViewport(),"UnderwaterQuadComp",true);
        std::cout << "underwaterCompositor->setEnabled(true);" << std::endl;
        underwaterCompositor->setEnabled(true);
    }

    else
    {
        mWater->setVisible(true);
        mUnderWater->setVisible(false);
        std::cout << "underwaterCompositor->setEnabled(false);" << std::endl;
        underwaterCompositor->setEnabled(false);
    }
}

bool FresnelWater::getUnderwater()
{
    return underWater;
}

/*
void FresnelWater::addEnt(graphics::ManualEntity *ent, entropy::uint32 id)
{
    if(ent->getNode()->getPosition().y<waterNode->getPosition().y)
    {
        submergedEnts[id] = ent;
    }
    else
    {
        surfaceEnts[id] = ent;
    }
}

void FresnelWater::removeEnt(graphics::ManualEntity *ent, entropy::uint32 id)
{
    if(ent->getNode()->getPosition().y<waterNode->getPosition().y)
    {
        if(submergedEnts.find(id)!=submergedEnts.end())
        {
            submergedEnts.erase(id);
        }
    }
    else
    {
        if(surfaceEnts.find(id)!=surfaceEnts.end())
        {
            surfaceEnts.erase(id);
        }
    }
}
*/
}

}

