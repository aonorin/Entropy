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

#include "baseapplication.h"

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
      fullScreen(false),
      mMouse(NULL),
      mKeyboard(NULL),
      mInputManager(NULL),
      running(true)
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
    mWindow = mRoot->initialise(true,"Neurocaster");

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
    //    mCameraMan = new Ogre::Camera("Neurocaster",mSceneMgr);
    //    mCameraMan = new CollisionCamera(mSceneMgr);
    //    mCameraMan->setCameraPosition(Ogre::Vector3(160, 0, 160));
//    mCamera = new Ogre::Camera("Neurocaster",mSceneMgr);

    mCamera = mSceneMgr->createCamera("PlayerCam");
    // set its position, direction
    mCamera->setPosition(Ogre::Vector3(0,10,500));
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    // set the near clip distance
    mCamera->setNearClipDistance(5);

//    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{

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

    mInputManager = OIS::InputManager::createInputSystem( paramList );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

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

//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
    if (!setup())
        return;

    if(fullScreen)
    {
        mWindow->setFullscreen(true, mWindow->getWidth(), mWindow->getHeight());
    }

    running = true;
    mRoot->startRendering();

    // clean up
    destroyScene();
}

bool BaseApplication::handleInput()
{
    if(running && mKeyboard && mMouse)
    {
        mKeyboard->capture();
        mMouse->capture();
    }

    return true;
}

void BaseApplication::input()
{

}

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
    resourcePath.append("/neurocaster_resources.cfg");

    std::string pluginPath(cCurrentPath);
    pluginPath.append("/plugins.cfg");

#elif __APPLE__

    CFBundleRef mainBundle = CFBundleGetMainBundle();
    //CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    char cCurrentPath[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(mainBundleURL, TRUE, (UInt8 *)cCurrentPath, PATH_MAX))
    {
        std::cerr << "Neurocaster::BaseApplication::go - CURRENT DIRECTORY NOT FOUND." << std::endl;
        return 0;
    }

    CFRelease(mainBundleURL);
    std::string workingDirectory(cCurrentPath);
    workingDirectory.append("/Contents/MacOS/");
    chdir(workingDirectory.c_str());

    std::string resourcePath(workingDirectory);
    resourcePath.append("../Resources/neurocaster_resources.cfg");
    std::string pluginPath(workingDirectory);
    pluginPath.append("../Resources/plugins.cfg");
    std::cout << "Current working directory: " << workingDirectory << std::endl;
    std::cout << "Resources path: " << resourcePath << std::endl;

#elif __LINUX__

    chdir("NeurocasterContent/Contents/LinuxOS");
    std::cout << "chdir" << std::endl;
    std::string resourcePath("../Resources/neurocaster_resources.cfg");
    std::string pluginPath("../Resources/plugins.cfg");

#endif

    mResourcesCfg = resourcePath.c_str();
    mPluginsCfg = pluginPath.c_str();

    mRoot = new Ogre::Root(mPluginsCfg);
    std::cout << "ROOT CREATED!" << std::endl;
    setupResources();
    std::cout << "RESOURCES SETUP!" << std::endl;

    bool carryOn = configure();
    if (!carryOn) return false;
    std::cout << "CARRYON!" << std::endl;

    chooseSceneManager();
    std::cout << "SCENE MANAGER CHOSEN!" << std::endl;

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
    handleInput();

    if(mWindow->isClosed())
        return false;

    if(mShutDown)
    {
        quitGame();
        return false;
    }

    return true;
}

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
    //Only close for window that created OIS
    if(rw == mWindow)
    {
        quitGame();
    }
}

void BaseApplication::quitGame()
{
    running = false;
    mShutDown = true;
#ifndef OIS
    if(mInputManager)
    {
        if(mMouse)
        {
            mInputManager->destroyInputObject(mMouse);
            mMouse = NULL;
        }

        if(mKeyboard)
        {
            mInputManager->destroyInputObject(mKeyboard);
            mKeyboard = NULL;
        }

        OIS::InputManager::destroyInputSystem(mInputManager);
        mInputManager = NULL;


    }
#endif

    mRoot->shutdown();
}
