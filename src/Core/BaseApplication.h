#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#ifndef OIS
#include "OIS/OISKeyboard.h"
#include "OIS/OISMouse.h"
#ifdef WIN32
#include "WIN32/Win32KeyBoard.h"
#include "WIN32/Win32Mouse.h"
#include "WIN32/Win32Prereqs.h"
#endif
#else
#include "SDL2/SDL.h"
#endif

#include "OGRE/OgreFrameListener.h"
//#include <src/Utility/framelistener.h>
#include "OGRE/OgreWindowEventUtilities.h"

//ForwardDeclares
namespace Ogre {
class OgreCamera;
class OgreEntity;
class OgreLogManager;
class OgreRoot;
class OgreRenderWindow;
}

// Entropy includes
class CollisionCamera;

class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener
        #ifndef OIS
        , public OIS::KeyListener, public OIS::MouseListener
        #endif
{
public:
    BaseApplication(void);
    virtual ~BaseApplication(void);

    virtual void go(void);
    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    bool doesFileExist(const char *filename);
    int mouseX;
    int mouseY;

#ifdef OIS
    virtual void keyPressed(SDL_Event event);
    virtual void keyReleased(SDL_Event event);
    virtual void mouseMoved(int x, int y, int relX, int relY);
    virtual void mousePressed(SDL_Event event);
    virtual void mouseReleased(SDL_Event event);
    SDL_Event test_event;
#else
    virtual bool mouseMoved(const OIS::MouseEvent &arg) = 0;
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
    virtual bool keyPressed(const OIS::KeyEvent &arg) = 0;
    virtual bool keyReleased(const OIS::KeyEvent &arg) = 0;
#endif

    bool mouseButtonDown;

    void input();
    CollisionCamera* mCameraMan;
    //    Uint32 gameLoopTimer(Uint32 interval, void* param);
    virtual bool setup();

protected:
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void);
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);

    bool handleInput();

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    int prevX,prevY;
    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);

    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;

    // OgreBites
    //    OgreBites::SdkTrayManager* mTrayMgr;
    //    OgreBites::ParamsPanel* mDetailsPanel;     // sample details panel
    bool mCursorWasVisible;                    // was cursor visible before dialog appeared
    bool mShutDown;


    bool endGame;
    int relativeMouseX;
    int relativeMouseY;
    bool sdlSetup;
    bool fullScreen;

#ifdef OIS
    SDL_Window* sdlWindow;
#else
    OIS::Mouse        *mMouse;
    OIS::Keyboard     *mKeyboard;
    OIS::InputManager *mInputManager;
#endif
};

#endif // #ifndef __BaseApplication_h_
