#include "OGRE/OgreSceneManager.h"

// Entropy includes
#include "graphics.h"
#include "src/Graphics/OgreDebugDraw/DebugDrawer.h"

namespace graphics {

bool debugDrawingEnabled = false;

bool getDebugDrawingEnabled()
{
    return debugDrawingEnabled;
}

void setDebugDrawingEnabled(bool drawingEnabled, Ogre::SceneManager* sceneManager)
{
    if(drawingEnabled)
    {
        detail::createDebugDraw(sceneManager);
    }

    else
    {
        if(debugDrawingEnabled)
        {
            finishDebugDraw();
            DebugDrawer::getSingletonPtr()->setEnabled(drawingEnabled);
            detail::destroyDebugDraw(sceneManager);
        }
    }

    debugDrawingEnabled = drawingEnabled;
}

void beginDebugDraw()
{
    // Draw shit here before build
    DebugDrawer::getSingleton().build();
}

void finishDebugDraw()
{
    DebugDrawer::getSingleton().clear();
}

namespace detail {

    BulletDebugDraw bulletDebugDraw;

    void createDebugDraw(Ogre::SceneManager* sceneManager)
    {
        new DebugDrawer(sceneManager, 0.5f);
        DebugDrawer::getSingletonPtr()->setEnabled(true);
    }

    void destroyDebugDraw(Ogre::SceneManager* sceneManager)
    {
        delete DebugDrawer::getSingletonPtr();
    }

} // detail namespace

} // graphics namespace
