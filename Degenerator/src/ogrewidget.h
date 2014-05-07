#ifndef OGREWIDGET_H
#define OGREWIDGET_H

#include <QtGui>

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <QWidget>
#include <QApplication>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "ColouredRectangle.h"
#include "CameraMan.h"

#include "mainwindow.h"
class MainWindow;

#define QTLIB	0
#define OISLIB	1

#define INPUTLIB QTLIB

namespace Degenerator {

class OverlayWidget : public QWidget
{
    Q_OBJECT

public:
    OverlayWidget(QWidget *parent);
    ~OverlayWidget();
    void setMessageString(QString msg)
    {
        if(msgstr != msg)
        {
            msgstr = msg;
            this->repaint();
        }
    }

protected:
    QString msgstr;
    void paintEvent(QPaintEvent* evt);
};

class OgreWidget : public QWidget
{
    Q_OBJECT

public:
    //OgreWidget();
    OgreWidget(MainWindow *parent);
    ~OgreWidget();
    virtual void	go(Ogre::Root* root, Ogre::SceneManager* mgr = NULL);
    QPaintEngine*	paintEngine() const; // Turn off QTs paint engine for the Ogre widget.
    Ogre::SceneManager* getSceneManager() {return mSceneMgr;}
    Ogre::String	mFPS;
    bool		mSystemInitialized;
    virtual void	updateOgre(float timeSinceLastFrame);
    virtual void	setViewportBackground(Ogre::ColourValue top, Ogre::ColourValue bottom);
    Ogre::SceneNode* getMeshNode();
    MainWindow* mw;
    int renderCount;
    bool cameraSetup;

public slots:
    void renderMesh();
    void exportMesh();

protected:
    bool setup();
    bool configure(void);
    void chooseSceneManager(void);
    void createCamera(void);
    void createScene(void);
    void destroyScene(void);
    void createViewports(void);
    void setupResources(void);
    void createResourceListener(void);
    void loadResources(void);
    bool manualInitialize(const std::string &desiredRenderer);

    Ogre::Root*	    mRoot;
    Ogre::Camera*	    mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String	    mResourcesCfg;
    Ogre::String	    mPluginsCfg;
    Ogre::Real	    mTimeSinceLastFrame;
    QOgre::CameraMan*   mCameraMan;
    bool mShutDown;
    bool mPassedSM;

private:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    bool updateExternalSystems();

    Ogre::Vector2		    mAbsolute;
    Ogre::Vector2		    mRelative;
    Degenerator::OverlayWidget*	    mOverlayWidget;

    Ogre::SceneNode* meshNode;
    Ogre::MeshPtr meshPtr;

};


}
#endif // OGREWIDGET_H
