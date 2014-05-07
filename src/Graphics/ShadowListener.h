#ifndef SHADOWLISTENER_H
#define SHADOWLISTENER_H

#include "OGRE/OgreSceneManager.h"

namespace graphics {

class ShadowListener : public Ogre::SceneManager::Listener
{
public:

    ShadowListener(Ogre::SceneManager *sceneMgr);
    virtual ~ShadowListener();

    void setupShadows();
    virtual void shadowTexturesUpdated(size_t numberOfShadowTextures);
    virtual void shadowTextureCasterPreViewProj(Ogre::Light* light, Ogre::Camera* camera);
    virtual void shadowTextureReceiverPreViewProj(Ogre::Light* light, Ogre::Frustum* frustum);

protected:

    Ogre::ShadowCameraSetupPtr shadowCameraSetup;
    Ogre::SceneManager* sceneMgr;
};

} // graphics namespace

#endif // SHADOWLISTENER_H
