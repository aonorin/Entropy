#include "deferred.h"
#include "DeferredShading.h"
#include "GeomUtils.h"

namespace graphics {

namespace deferred {

DeferredShadingSystem* system = 0;

void initialize(Ogre::Viewport *vp, Ogre::SceneManager *sm, Ogre::Camera *cam)
{
    system = new DeferredShadingSystem(vp, sm, cam);
    system->initialize();
}

void cleanup()
{
    if(system)
    {
        delete system;
        system = 0;
    }
}

} // deferred namespace

} // graphics namespace
