#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Graphics/BulletDebugDraw.h"

namespace Ogre {
    class OgreSceneManager;
}

namespace graphics {

    bool getDebugDrawingEnabled();
    void setDebugDrawingEnabled(bool drawingEnabled, Ogre::SceneManager* sceneManager);
    void beginDebugDraw();
    void finishDebugDraw();

    namespace detail {
        void createDebugDraw(Ogre::SceneManager* sceneManager);
        void destroyDebugDraw(Ogre::SceneManager* sceneManager);
        extern BulletDebugDraw bulletDebugDraw;
    } // detail namespace

} // graphics namespace

#endif // GRAPHICS_H
