#ifndef FRESNELWATER_H
#define FRESNELWATER_H

#include "OGRE/OgreRenderTargetListener.h"
#include "OGRE/OgreTextureManager.h"
#include "OGRE/OgreMovablePlane.h"
#include "OGRE/OgreVector3.h"
#include "OGRE/OgreRenderTarget.h"
#include "Graphics/manualentity.h"
#include "map"
#include "OGRE/OgreCompositorManager.h"
#include "OGRE/OgreCompositorInstance.h"
#include "src/Physics/waterobject.h"

namespace voxel
{
namespace environment
{

class FresnelWater : public Ogre::RenderTargetListener
{
public:
    typedef boost::shared_ptr<FresnelWater> ptr;
    FresnelWater(Ogre::Vector3 pos, Ogre::Real width = 4000, Ogre::Real height = 4000);

    void preRenderTargetUpdate(const Ogre::RenderTargetEvent &evt);
    void postRenderTargetUpdate(const Ogre::RenderTargetEvent &evt);
//    void addEnt(graphics::ManualEntity* ent,entropy::uint32 id);
//    void removeEnt(graphics::ManualEntity* ent,entropy::uint32 id);
    void setupWater();
    void updateWater();
    void setEnabled(bool enabled);
    bool getUnderwater();

private:

    void deferredSetEnabled();

    bool setup;
    Ogre::Vector3 pos;
    Ogre::Real width, height;
    Ogre::RenderTarget* mRefractionTarget;
    Ogre::RenderTarget* mReflectionTarget;
    Ogre::MovablePlane* mWaterPlane;
    Ogre::Entity* mWater;
    Ogre::SceneNode* waterNode;

    Ogre::MovablePlane* mUnderWaterPlane;
    Ogre::Entity* mUnderWater;
    Ogre::SceneNode* mUnderWaterNode;

    static int staticID;
    int id;
    std::vector<Ogre::Entity*> surfaceEnts;
    std::vector<Ogre::Entity*> submergedEnts;
    bool underWater;
    physics::WaterObject waterPhysics;

    Ogre::CompositorInstance* underwaterCompositor;
//    std::vector<graphics::ManualEntity*> mSurfaceEnts;
//    std::vector<graphics::ManualEntity*> mSubmergedEnts;
};

}

}
#endif // FRESNELWATER_H
