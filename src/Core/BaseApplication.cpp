// System includes
#include <stdio.h>  /* defines FILENAME_MAX */
#include <iostream>
#include <fstream>

#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
//#include "boost/split.hpp"
#else
#include <unistd.h>
#endif

#define GetCurrentDir getcwd
#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

#ifdef OIS

#include "SDL2/SDL_mouse.h"
#else
#include "OISInputManager.h"
#endif

//Ogre includes
#include "OGRE/OgreCamera.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreViewport.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreConfigFile.h"
#include "OGRE/OgreWindowEventUtilities.h"

#include "BaseApplication.h"
#include "src/Core/collisioncamera.h"
#include "src/Graphics/graphics.h"
#include "src/Physics/collisiongroupmanager.h"

//#ifdef __APPLE__
//#import "Core/NSRenderer.h"
//#endif

const int RUN_GAME_LOOP = 1;

//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
    : mRoot(0),
      mCamera(0),
      mSceneMgr(0),
      mWindow(0),
      mResourcesCfg(Ogre::StringUtil::BLANK),
      mPluginsCfg(Ogre::StringUtil::BLANK),
      //      mTrayMgr(0),
      mCameraMan(0),
      //      mDetailsPanel(0),
      mCursorWasVisible(false),
      mShutDown(false),
      mouseButtonDown(false),
      fullScreen(false)
{
}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
    std::cout << "BaseApplication::~BaseApplication(void)" << std::endl;
    //    if (mTrayMgr) delete mTrayMgr;
    //    if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    //    delete mRoot;
}

//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void)
{

    // If can't load config from ogre.cfg file, ask user
    if (!mRoot->restoreConfig())
    {
        // If user clicks cancel or some other error, return false
        if (!mRoot->showConfigDialog())
            return false;
    }
    Ogre::ConfigOptionMap cMap = mRoot->getRenderSystem()->getConfigOptions();
    if(cMap.at("Full Screen").currentValue.compare("Yes")==0) {
        mRoot->getRenderSystem()->setConfigOption("Full Screen", "No");
        fullScreen = true;
    }
    mWindow = mRoot->initialise(true,"Entropy");

    //    mWindow->setVisible(true);
    //    mWindow->setActive(true);

    //EXTERNAL WINDOW SECTION
    //    mWindow = mRoot->initialise(false);
    //    sfmlWindow = new sf::Window(sf::VideoMode(800, 600, 32), "SFML Entropy");
    //    Ogre::NameValuePairList params;
    //    size_t* windowHnd;
    //    std::ostringstream windowHndStr;

#ifdef OIS
    //    windowHnd = (size_t*)sfmlWindow->getSystemHandle();
    //    windowHndStr << *windowHnd;
    //    winHandle = Ogre::StringConverter::toString(&winSizeHandle);
    //    params["macAPI"] = Ogre::String("cocoa");
    //    params["currentGLContext"] = Ogre::String("true");
    SDL_Init(SDL_INIT_VIDEO);
    unsigned long hWnd = 0;
    mWindow->getCustomAttribute("WINDOW", &hWnd);

    sdlWindow = SDL_CreateWindowFrom((void*)hWnd);
    SDL_SetWindowGrab(sdlWindow,SDL_TRUE);
#else

#endif

    //    params["externalWindowHandle"] = windowHndStr.str();
    //    std::cout << "SFML WINDOW HANDLE: " << windowHndStr.str() << std::endl;
    //    mWindow = mRoot->createRenderWindow("Entropy", 800, 600, false , &params);
    //    mWindow->setActive(true);
    //    mWindow->setVisible( true );
    return true;
}


bool BaseApplication::doesFileExist(const char *filename)
{
    std::ifstream ifile(filename);
    return ifile;
}

//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    //    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    mSceneMgr = mRoot->createSceneManager("OctreeSceneManager");
}
//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
    // Create the camera
    //mCamera = mSceneMgr->createCamera("PlayerCam");
    //    std::cout << mCamera->getName() << std::endl;
    // Position it at 500 in Z direction
    //mCamera->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    //mCamera->lookAt(Ogre::Vector3(0,0,-300));
    //mCamera->setNearClipDistance(5);

    //mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
    mCameraMan = new CollisionCamera(mSceneMgr);
    mCameraMan->setCameraPosition(Ogre::Vector3(160, 0, 160));
    mCamera = mCameraMan->getCamera();
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
#ifndef OIS
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

    // Setup basic variables
    OIS::ParamList paramList;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    // Get window handle
    mWindow->getCustomAttribute( "WINDOW", &windowHnd );

    // Fill parameter list
    windowHndStr << windowHnd;
    paramList.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );

    //    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    //    pl.insert(std::make_pair("x11_keyboard_grab", "true"));
    //    pl.insert(std::make_pair("x11_mouse_grab", "true"));

    mInputManager = OIS::InputManager::createInputSystem( paramList );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
#endif
    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);
}

//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{

}
//-------------------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
                Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                        archName, typeName, secName);
        }
    }
    //    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Resources/resources","FileSystem",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

/*
Uint32 gameLoopTimer(Uint32 interval, void* param)
{
    // Create a user event to call the game loop.
    SDL_Event event;

    event.type = SDL_USEREVENT;
    event.user.code = RUN_GAME_LOOP;
    event.user.data1 = 0;
    event.user.data2 = 0;

    SDL_PushEvent(&event);

    return interval;
}

int SDLEventFilter(void* data, SDL_Event* filterEvent)
{
    if (filterEvent->type == SDL_MOUSEBUTTONDOWN) {
        //        std::cout << "MouseButtonDown!" << std::endl;
        return 0;
    }

    return 1;
}
*/

//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
    if (!setup())
        return;

    //        mRoot->startRendering();

#ifdef OIS
    prevX = mWindow->getWidth()*0.5;
    prevY = mWindow->getWidth()*0.5;

    SDL_ShowCursor(0);
    SDL_SetWindowGrab(sdlWindow,SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    if(fullScreen) {
        SDL_SetWindowFullscreen(sdlWindow,SDL_TRUE);
    }
    mRoot->addFrameListener(this);
    try
    {
        mRoot->startRendering();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unkown exception in Render Loop" << std::endl;
    }

#else
    if(fullScreen)
    {
        mWindow->setFullscreen(true, mWindow->getWidth(), mWindow->getHeight());
    }

    mRoot->startRendering();
#endif
    // clean up
    destroyScene();
}

bool BaseApplication::handleInput()
{
#ifdef OIS
    SDL_WarpMouseInWindow(sdlWindow,mWindow->getWidth()*0.5,mWindow->getHeight()*0.5);
    while(SDL_PollEvent(&test_event)) {
        try{
            switch(test_event.type) {
            case SDL_USEREVENT:
                if(test_event.user.code==RUN_GAME_LOOP) {
                    //                        mRoot->renderOneFrame();
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(!mouseButtonDown) {
                    mouseButtonDown = true;

                }
                mousePressed(test_event);
                break;
            case SDL_MOUSEMOTION:
                //                std::cout << test_event.motion.xrel << std::endl;
                mouseMoved(test_event.motion.x,test_event.motion.y,test_event.motion.xrel,test_event.motion.yrel);

                break;
            case SDL_MOUSEBUTTONUP:
                mouseButtonDown = false;
                mouseReleased(test_event);
                break;
            case SDL_KEYDOWN:
                keyPressed(test_event);
                break;
            case SDL_KEYUP:
                keyReleased(test_event);
                break;
            case SDL_QUIT:
                mShutDown = true;
                break;
            default:
                break;
            }
        } catch(...) {

        }
    }
#else
    mKeyboard->capture();
    mMouse->capture();
#endif
    return true;
}

void BaseApplication::input()
{

}

#ifdef OIS

void BaseApplication::keyPressed(SDL_Event event)
{

}

void BaseApplication::keyReleased(SDL_Event event)
{

}

void BaseApplication::mouseMoved(int x, int y, int relX,int relY)
{

}

void BaseApplication::mousePressed(SDL_Event event)
{

}

void BaseApplication::mouseReleased(SDL_Event event)
{

}

#else
/*
bool BaseApplication::mouseMoved(const OIS::MouseEvent &arg)
{
    return true;
}

bool BaseApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool BaseApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool BaseApplication::keyPressed(const OIS::KeyEvent &arg)
{
    return true;
}

bool BaseApplication::keyReleased(const OIS::KeyEvent &arg)
{
    return true;
}
*/
#endif

//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
#ifdef WIN32
    char cCurrentPath[FILENAME_MAX];

    if(!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath) / sizeof(TCHAR)))
    {
        std::cerr << "Entropy::BaseApplication::go - CURRENT DIRECTORY NOT FOUND." << std::endl;
        return false;
    }
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */


    //Add String maniuplation so that the paths are correct

    std::string resourceSearhPath("Contents/WinOS");
    chdir(resourceSearhPath.c_str());
    std::cout << "Working directory: " << cCurrentPath << std::endl;

    std::string resourcePath(cCurrentPath);
    resourcePath.append("/entropy_resources.cfg");

    std::string pluginPath(cCurrentPath);
    pluginPath.append("/plugins.cfg");

#endif

#ifdef __APPLE__

    CFBundleRef mainBundle = CFBundleGetMainBundle();
    //CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    char cCurrentPath[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(mainBundleURL, TRUE, (UInt8 *)cCurrentPath, PATH_MAX))
    {
        std::cerr << "Entropy::BaseApplication::go - CURRENT DIRECTORY NOT FOUND." << std::endl;
        return 0;
    }

    CFRelease(mainBundleURL);
    std::string workingDirectory(cCurrentPath);
    workingDirectory.append("/Contents/MacOS/");
    chdir(workingDirectory.c_str());

    std::string resourcePath(workingDirectory);
    resourcePath.append("../Resources/entropy_resources.cfg");
    std::string pluginPath(workingDirectory);
    pluginPath.append("../Resources/plugins.cfg");
    std::cout << "Current working directory: " << workingDirectory << std::endl;
    std::cout << "Resources path: " << resourcePath << std::endl;

#elif __LINUX__

    chdir("HypnosContent/Contents/LinuxOS");
    std::cout << "chdir" << std::endl;
    std::string resourcePath("../Resources/entropy_resources.cfg");
    std::string pluginPath("../Resources/plugins.cfg");

#endif

    mResourcesCfg = resourcePath.c_str();
    mPluginsCfg = pluginPath.c_str();

    /*
#ifdef _DEBUG
    mResourcesCfg = "/Contents/Resources/entropy_resources.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "/Contents/Resources/entropy_resources.cfg";
    mPluginsCfg = "/Contents/Resources/plugins.cfg";
#endif*/

    mRoot = new Ogre::Root(mPluginsCfg);
    std::cout << "ROOT CREATED!" << std::endl;
    setupResources();
    std::cout << "RESOURCES SETUP!" << std::endl;

    bool carryOn = configure();
    if (!carryOn) return false;
    std::cout << "CARRYON!" << std::endl;

    chooseSceneManager();
    std::cout << "SCENE MANAGER CHOSEN!" << std::endl;
    physics::CollisionGroupManager::createDefaultCollisionGroups();
    createCamera();
    std::cout << "CAMERA CREATED!" << std::endl;
    createViewports();
    std::cout << "VIEWPORTS CREATED!" << std::endl;

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    std::cout << "RESOURCE LISTENER CREATED!" << std::endl;
    // Load resources
    loadResources();
    std::cout << "RESOURCES LOADED!" << std::endl;

    // Create the scene
    createScene();
    std::cout << "SCENE CREATED!" << std::endl;

    createFrameListener();
    std::cout << "FRAMELISTENER CREATED!" << std::endl;

    return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
#ifdef __APPLE__
    handleInput();
#endif
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;
    //Need to capture/update each device
    //    mKeyboard->capture();
    //    mMouse->capture();
    return true;
}

/*
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{

    if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;
    }
    if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        //        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        //        mShutDown = true;
    }

    mCameraMan->injectKeyDown(arg);
    return true;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
    mCameraMan->injectKeyUp(arg);
    return true;
}

bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
    //    if (mTrayMgr->injectMouseMove(arg)) return true;
    mCameraMan->injectMouseMove(arg);
    return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    //    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    //    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    mCameraMan->injectMouseUp(arg, id);
    return true;
}
*/

//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

#ifndef OIS
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
#endif
}

//Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
    std::cout << "void BaseApplication::windowClosed(Ogre::RenderWindow* rw)" << std::endl;
    //Only close for window that created OIS (the main window in these demos)
    if(rw == mWindow)
    {
#ifndef OIS
        if(mInputManager)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
#endif
    }
}
