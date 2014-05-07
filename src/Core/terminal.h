#ifndef TERMINAL_H
#define TERMINAL_H

#include <OGRE/OgreSingleton.h>
#include <time.h>
#include "math.h"
#include "OGRE/OgreRenderTargetListener.h"

//Forward Declerations
namespace Ogre {
    class OverlayManager;
    class Overlay;
    class OverlayContainer;
    class OverlayElement;
    class Rectangle2D;
}

namespace neuro {
    class Room;
    class Map;
    class Port;
    class Square;
    class Hero;
}

using namespace Ogre;

class Terminal : public Ogre::Singleton<Terminal>, Ogre::RenderTargetListener
{
public:

    Ogre::OverlayManager*    _overlayMgr;
    Ogre::Overlay*           _overlay;
    Ogre::Overlay*           _overlay2;
    Ogre::Overlay*           _overlay3;
//    Ogre::Overlay*           _overlay4;
    Ogre::Overlay*           relativeOverlay;
    Ogre::OverlayContainer*  _panel;
    Ogre::OverlayContainer*  _panel2;
    Ogre::OverlayContainer*  _panel3;
//    Ogre::OverlayContainer*  _panel4;
    Ogre::OverlayContainer*  relativePanel;
    Ogre::OverlayElement* heroBox;

    Terminal();
    ~Terminal();

    void initTerminal(
            const std::string& ID,
            const std::string& text,
            Ogre::Real x, Ogre::Real y,
            Ogre::Real width, Ogre::Real height,
            const Ogre::ColourValue& color);

    void removeTextBox(const std::string& ID);
    void closeTerminal();
    void updateTerminal(int time);
    const std::string& getText(const std::string& ID);
    void setMapView(bool mapView);
    void moveHero(int ch);
    void button1();
    void button2();
    void button3();
    void button4();
    static Terminal& getSingleton(void);
    static Terminal* getSingletonPtr(void);
    float pixelSize;

    Ogre::OverlayElement* occupiedRoom;
    Ogre::OverlayContainer* healthBar;
    Ogre::OverlayContainer* suspicionBar;
    bool hacking;

    void preViewportUpdate(const RenderTargetViewportEvent &evt);
    void postViewportUpdate(const RenderTargetViewportEvent &evt);
    void preRenderTargetUpdate(const RenderTargetEvent &evt);
    void postRenderTargetUpdate(const RenderTargetEvent &evt);

    Ogre::SceneNode* createTexturedRect(std::string object_name, std::string texture_name, float left, float top, float right, float bottom);

private:
    float scale;
    int cols;
    int lines;
    int roomsX;
    int roomsY;
    std::string mapString;
    boost::shared_ptr<neuro::Map> map;
    int hallwayID;

    bool scrollLeft,scrollRight,scrollUp,scrollDown;
    bool isScrollRoom;
    bool roomScrollSwitch;
    void scrollPlayer();
    void scrollRoom();
    bool mapView;
    Ogre::CompositorInstance* asciiCompositor;

    //NEW NON-OVERLAY VERSION
    Ogre::SceneNode* background;
};

#endif // TERMINAL_H
