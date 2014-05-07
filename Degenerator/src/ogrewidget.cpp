// System includes
#include <stdio.h>  /* defines FILENAME_MAX */
#include <sstream>

#ifdef __WINDOWS__
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

// Ogre includes
#include "ogrewidget.h"

// Qt includes
#include <QPaintEvent>
#include <QTextOption>
#include <QVBoxLayout>

#include "meshgenerator.h"
#include "OgreMeshSerializer.h"
#include "OgreMaterialManager.h"

namespace Degenerator {

OverlayWidget::OverlayWidget(QWidget *parent=0)
    : QWidget(parent)
{
    msgstr = QApplication::translate("Widget", "Initializing OGRE...");
    setContextMenuPolicy( Qt::PreventContextMenu );
}

OverlayWidget::~OverlayWidget()
{

}

//----------------------------------------------------------------------------------------
void OverlayWidget::paintEvent(QPaintEvent* evt)
{
    QPainter painter(this);
    painter.setClipRect(0,0,width(),height());
    painter.setBrush(QBrush(QColor(0,0,0)));
    painter.fillRect(QRectF(0,0,width(),height()), QColor(0,0,0));
    painter.setPen(QColor(210,210,210));
    painter.drawText(QRectF(0,0,width(),height()),msgstr,QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
}

//-------------------------------------------------------------------------------------
OgreWidget::OgreWidget(MainWindow *parent)
    : QWidget(parent),
      mw(parent),
      meshNode(0),
      renderCount(0),
      cameraSetup(false),
      mRoot(0),
      mCamera(0),
      mSceneMgr(0),
      mWindow(0),
      mShutDown(false),
      mPassedSM(false),
      mSystemInitialized(false),
      mResourcesCfg(Ogre::StringUtil::BLANK),
      mPluginsCfg(Ogre::StringUtil::BLANK)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_PaintOnScreen);
    setMinimumSize(240,240);
    resize(800,600);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    mOverlayWidget = new OverlayWidget(this);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(mOverlayWidget);
    setLayout(layout);
    mOverlayWidget->setMessageString("Application -> Start Rendering");
}

//-------------------------------------------------------------------------------------
OgreWidget::~OgreWidget()
{
}

//-------------------------------------------------------------------------------------
bool OgreWidget::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg

    //    if (!mRoot->restoreConfig())
    //    {
    //        manualInitialize("OpenGL");
    //        std::cout << "Couldn't Restore previous Configurations" << std::endl;
    // If user clicks cancel or some other error, return false
    //if (!mRoot->showConfigDialog())
    //    return false;
    //    }
    manualInitialize("OpenGL");

    Ogre::NameValuePairList viewConfig;
    Ogre::String winHandle;
#ifdef WIN32
    // Windows code
    winHandle += Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
#elif __APPLE__&&__MACH__
    // Mac code, tested on Mac OSX 10.6 using Qt 4.7.4 and Ogre 1.7.3
    winHandle  = Ogre::StringConverter::toString(winId());
//    HIViewRef view1 = (HIViewRef) mDrawArea->winId();
//    WindowRef win1 = HIViewGetWindow(view1);
//    std::cout << "HIView: " << win1 << std::endl;
//    std::cout << "QWIDGET WINID: " << winHandle << std::endl;
#else
    // Unix code
    QX11Info info = x11Info();
    winHandle  = Ogre::StringConverter::toString((unsigned long)(info.display()));
    winHandle += ":";
    winHandle += Ogre::StringConverter::toString((unsigned int)(info.screen()));
    winHandle += ":";
    winHandle += Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
#endif

#ifndef __APPLE__&&__MACH__
    // code for Windows and Linux
    viewConfig["parentWindowHandle"] = winHandle;
    mWindow = mOgreRoot->createRenderWindow( "QOgreWidget_RenderWindow",
                                             this->width(),
                                             this->height(),
                                             false,
                                             &viewConfig );

    mWindow->setActive(true);
    WId ogreWinId = 0x0;
    mWindow->getCustomAttribute( "WINDOW", &ogreWinId );

    assert( ogreWinId );

    // bug fix, extract geometry
    QRect geo = this->frameGeometry ( );

    // create new window
    this->create( ogreWinId );

    // set geometrie infos to new window
    this->setGeometry (geo);

#else
    // code for Mac
    viewConfig["externalWindowHandle"] = winHandle;
    viewConfig["macAPI"] = "cocoa";
    viewConfig["macAPICocoaUseNSView"] = "true";
    // The line below utilizes the root's timer in order to generate a unique name for each widget instance
//    mWindow = mRoot->createRenderWindow("OgreQtWidgetWindow",800,600, false, &viewConfig);
    mWindow = mRoot->initialise(true,"Degenerator");
    mWindow->setActive(true);
    //makeCurrent();
#endif

    // The line below utilizes the root's timer in order to generate a unique name for each widget instance
    //mWindow = ogreRoot->createRenderWindow(Ogre::StringConverter::toString(mRoot->getTimer()->getMicroseconds()),width(), height(), false, &viewConfig);
    return true;
}


bool OgreWidget::manualInitialize(const std::string &desiredRenderer)
{
    Ogre::RenderSystem *renderSystem;
    bool ok = false;
    Ogre::RenderSystemList renderers = Ogre::Root::getSingleton().getAvailableRenderers();

    // See if the list is empty (no renderers available)
    if(renderers.empty()) {
        std::cout << "NO RENDERERS!!!!" << std::endl;
        return false;
    }

    for(Ogre::RenderSystemList::iterator it = renderers.begin();
        it != renderers.end(); it++)
    {
        renderSystem = (*it);
        /*
        if(strstr(&(*renderSystem->getName()), &(*desiredRenderer)))
        {
            ok = true;
            break;
        }
        */
        if(strcmp(renderSystem->getName().c_str(),desiredRenderer.c_str())==0) {
            ok = true;
            break;
            //std::cout << "CHOSEN RENDER SYSTEM = " << renderSystem->getName().c_str() << std::endl;
        }
    }

    if(!ok) {
        // We still don't have a renderer; pick
        // up the first one from the list
        std::cout << "CHOSEN RENDER SYSTEM = " << (*renderers.begin())->getName() << std::endl;
        renderSystem = (*renderers.begin());
    }
    std::cout << "CHOSEN RENDER SYSTEM = " << renderSystem->getName() << std::endl;
    Ogre::Root::getSingleton().setRenderSystem(renderSystem);

    // Manually set some configuration options (optional)
    renderSystem->setConfigOption("Video Mode", "800 x 600");

    return true;

}

//-------------------------------------------------------------------------------------
void OgreWidget::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mOverlayWidget->setMessageString("Creating Scene Manager");
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}
//-------------------------------------------------------------------------------------
void OgreWidget::createCamera(void)
{
    mOverlayWidget->setMessageString("Creating Camera");
    // Create the camera
    mCamera = mSceneMgr->createCamera(Ogre::StringConverter::toString(mRoot->getTimer()->getMicroseconds()));

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(0.1);
    mCameraMan = new QOgre::CameraMan(mCamera);
}
//-------------------------------------------------------------------------------------
QPaintEngine* OgreWidget::paintEngine() const
{
    // We don't want another paint engine to get in the way for our Ogre based paint engine.
    // So we return nothing.
    return NULL;
}
//-------------------------------------------------------------------------------------
void OgreWidget::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void OgreWidget::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
                Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mOverlayWidget->setMessageString("Creating viewports...");
}
//-------------------------------------------------------------------------------------
void OgreWidget::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);
    mOverlayWidget->setMessageString("Loading resource paths from \n" + QString::fromStdString(mResourcesCfg));
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
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            mOverlayWidget->setMessageString("Loading resource section '"
                                             + QString::fromStdString(secName)
                                             + "' (" + QString::fromStdString(typeName) + ")");
        }
    }

    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Data","FileSystem",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);
}
//-------------------------------------------------------------------------------------
void OgreWidget::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void OgreWidget::paintEvent(QPaintEvent *e)
{

}
//-------------------------------------------------------------------------------------
void OgreWidget::resizeEvent(QResizeEvent *e)
{
    if(e->isAccepted())
    {
        const QSize &newSize = e->size();
        if(mRoot && mCamera)
        {
            mWindow->resize(newSize.width(), newSize.height());
            mWindow->windowMovedOrResized();

            Ogre::Real aspectRatio = Ogre::Real(newSize.width()) / Ogre::Real(newSize.height());
            mCamera->setAspectRatio(aspectRatio);
        }
    }
}
//-------------------------------------------------------------------------------------
void OgreWidget::mouseMoveEvent(QMouseEvent *e)
{
    std::cout << "Moving mouse!" << std::endl;
    if (mSystemInitialized)
    {
        std::cout << "Moving mouse!" << std::endl;
        Ogre::Vector2 oldPos = mAbsolute;
        mAbsolute = Ogre::Vector2(e->pos().x(), e->pos().y());
        mRelative = mAbsolute - oldPos;
        mCameraMan->injectMouseMove(mRelative);
    }
}

//-------------------------------------------------------------------------------------
void OgreWidget::mousePressEvent(QMouseEvent *e)
{
     std::cout << "mouse pressed!" << std::endl;
    if (mSystemInitialized)
        mCameraMan->injectMouseDown(e);
}

//-------------------------------------------------------------------------------------
void OgreWidget::mouseReleaseEvent(QMouseEvent *e)
{
     std::cout << "mouse release!" << std::endl;
    if (mSystemInitialized)
        mCameraMan->injectMouseUp(e);
}
//-------------------------------------------------------------------------------------
void OgreWidget::wheelEvent(QWheelEvent *e)
{
     std::cout << "Moving wheel!" << std::endl;
    if (mSystemInitialized)
        mCameraMan->injectMouseWheel(e);
}

//-------------------------------------------------------------------------------------
void OgreWidget::keyPressEvent(QKeyEvent *e)
{
     std::cout << "key pressed!" << std::endl;
    if (mSystemInitialized)
        mCameraMan->injectKeyDown(e);
}

//-------------------------------------------------------------------------------------
void OgreWidget::keyReleaseEvent(QKeyEvent *e)
{
     std::cout << "key released!" << std::endl;
    if (mSystemInitialized)
        mCameraMan->injectKeyUp(e);
}
//-------------------------------------------------------------------------------------
void OgreWidget::updateOgre(float timeSinceLastFrame)
{
    mTimeSinceLastFrame = timeSinceLastFrame;
    if(meshNode!=0) {

    }
    repaint();
    if (this->size() != this->parentWidget()->size())
        resize(this->parentWidget()->size());
}

//-------------------------------------------------------------------------------------
bool OgreWidget::updateExternalSystems()
{
    return true;
}

//-------------------------------------------------------------------------------------
void OgreWidget::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

//-------------------------------------------------------------------------------------
void OgreWidget::go(Ogre::Root* root, Ogre::SceneManager* mgr)
{
#ifdef __WINDOWS__
    char cCurrentPath[FILENAME_MAX];

    if(!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath) / sizeof(TCHAR)))
    {
        std::cerr << "Entropy::BaseApplication::go - CURRENT DIRECTORY NOT FOUND." << std::endl;
        return;
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    std::cout << "Working directory: " << cCurrentPath << std::endl;

    std::string resourcePath(cCurrentPath);
    resourcePath.append("Contents/Resources/entropy_resources.cfg");
    std::string pluginPath(cCurrentPath);
    pluginPath.append("Contents/Resources/plugins.cfg");

#endif

#ifdef __APPLE__

    CFBundleRef mainBundle = CFBundleGetMainBundle();
    //CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    char cCurrentPath[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(mainBundleURL, TRUE, (UInt8 *)cCurrentPath, PATH_MAX))
    {
        std::cerr << "Entropy::BaseApplication::go - CURRENT DIRECTORY NOT FOUND." << std::endl;
        return;
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

#endif

    mResourcesCfg = resourcePath.c_str();
    mPluginsCfg = pluginPath.c_str();

    /*
#ifdef _DEBUG
    mResourcesCfg = "../Resources/entropy_resources.cfg";
    mPluginsCfg = "../Resources/plugins.cfg";
#else
    //mResourcesCfg = "config.cfg";
    //mPluginsCfg = "plugins.cfg";
    mResourcesCfg = "entropy_resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
    */

    mRoot = root;
    if (mgr)
    {
        mSceneMgr = mgr;
        mPassedSM = true;
    }

    if (!setup())
        return;
    //mRoot->startRendering();
    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------
bool OgreWidget::setup(void)
{
    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    if (!mSceneMgr)
        chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    // Create any resource listeners (for loading screens)
    createResourceListener();

    // Load resources
    loadResources();

    mSystemInitialized = true;
    mOverlayWidget->hide();

    if (!mPassedSM)
        createScene();

    return true;
}
//-------------------------------------------------------------------------------------
void OgreWidget::createScene(void)
{
    mOverlayWidget->setMessageString("Creating Scene");
    // create our model, give it the shader material, and place it at the origin
    //Ogre::Entity *ent = mSceneMgr->createEntity("The Big Head", "ogrehead.mesh");
    //Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    //node->attachObject(ent);

    //mCameraMan->setTarget(ent->getParentSceneNode());

    mCamera->setFixedYawAxis(true);
    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);
    Ogre::ColourValue blender	    = Ogre::ColourValue(0.223529, 0.223529, 0.223529, 1.0);
    Ogre::ColourValue top	    = Ogre::ColourValue(0.224, 0.224, 0.224, 1.0);
    Ogre::ColourValue bottom	    = Ogre::ColourValue(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0);
    setViewportBackground(blender, blender);
}
//-------------------------------------------------------------------------------------
void OgreWidget::setViewportBackground(Ogre::ColourValue top, Ogre::ColourValue bottom)
{
    // Create background material
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Background", "General");
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    // Create background rectangle covering the whole screen
    QOgre::ColouredRectangle2D* rect = new QOgre::ColouredRectangle2D();
    rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    rect->setMaterial("Background");

    // Set the colours
    rect->setColours(top, bottom, top, bottom);

    // Render the background before everything else
    rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

    // Use infinite AAB to always stay visible
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    rect->setBoundingBox(aabInf);

    // Attach background to the scene
    Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Background");
    node->attachObject(rect);
}

//-------------------------------------------------------------------------------------

//====================================================================================================
//DEGENERATOR METHODS
//====================================================================================================

void OgreWidget::renderMesh() {
    if(meshNode!=0) {
        mSceneMgr->destroyEntity("DegeneratorObject");
        mSceneMgr->getRootSceneNode()->removeChild(meshNode);
    }

    std::string materialName = mw->getMaterialName();
    renderCount++;
    std::stringstream ss;
    ss << renderCount;
    std::string meshName = "DegeneratorMesh";
    meshName.append(ss.str());
    std::string objectName = "DegeneratorObject";
    std::cout << "Loading new mesh." << std::endl;

    AStruct aStruct = mw->generateMesh();
    std::cout << "Vertex count: " << aStruct.vertices.size() << std::endl;
    std::cout << "Index count: " << aStruct.indices.size() << std::endl;
    std::cout << "TexCoord count: " << aStruct.texCoords.size() << std::endl;

    meshPtr = MeshGenerator::getSingletonPtr()->createMesh(
                meshName,
                "DegenGroup",
                mSceneMgr,
                aStruct.vertices,
                aStruct.indices,
                aStruct.texCoords,
                aStruct.normals,
                materialName,
                aStruct.mode
                );
    while(meshPtr.getPointer()->isLoading()) {
        std::cout << meshPtr.getPointer()->isLoading() << std::endl;
    }
    std::cout << "New Mesh Loaded" << std::endl;

    Ogre::Entity *ent = mSceneMgr->createEntity("DegeneratorObject", meshName);
    ent->setMaterialName(materialName);
    meshNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    meshNode->setPosition(0,0,0);
    meshNode->attachObject(ent);
    meshNode->setVisible(true);
    if(!cameraSetup) {
        mCameraMan->getCamera()->move(Ogre::Vector3(0,50,-200));
        mCameraMan->getCamera()->lookAt(0,0,0);
        cameraSetup = true;
    }
}

void OgreWidget::exportMesh()
{
    Ogre::MeshSerializer serializer;
    if (mw->fileName.isEmpty())
        return;
    else {
        serializer.exportMesh(meshPtr.getPointer(),mw->fileName.toStdString());
    }
}

Ogre::SceneNode* OgreWidget::getMeshNode() {
    return meshNode;
}

}
