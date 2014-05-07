#ifndef __Entropy_h_
#define __Entropy_h_

// Boost includes
#include "boost/function.hpp"

// Entropy includes
#include "BaseApplication.h"
//#include "locale"

#include "boost/signals2.hpp"
#include "OGRE/OgreShadowCameraSetup.h"
#include "src/Graphics/manualentity.h"
//#include "boost/thread/mutex.hpp"
//#include "boost/thread/locks.hpp"
#include "src/Utility/ois2sdl.h"

namespace boost {
class shared_mutex;
}

//using namespace Ogre;

// Forward declarations
class Update;
class Terminal;
class Update;
class Inventory;
class GameMenu;
class EntropyLog;
class QuestMenu;
class ChatMenu;
class EntropyCursor;
class Console;
class InputLoop;

namespace graphics {
class ShadowListener;
}

namespace voxel {
class TerrainGenerator;
}

namespace physics {
class PhysicsActor;
} // physics namespace

class Entropy : public BaseApplication, public Ogre::Singleton<Entropy>
{
public:
    Entropy();
    virtual ~Entropy();
    void update();
    static Entropy& getSingleton(void);
    static Entropy* getSingletonPtr(void);
    void addQeueudFunction(boost::function<void (void)> func);
    typedef unsigned short uint16;
    void addQeueudFunction(boost::function<void (uint16, int)> func,uint16 lod,int id);
    void addQeueudDeleteOverlays(Ogre::OverlayElement* overlaysElement);
    void addQueuedDeleteManualObject(std::string mo);
    void addQueuedDeleteManualEntity(graphics::ManualEntity* me);
    virtual void startNewGame();
    virtual void continueGame();
    virtual void quitGame();
    float mouseX;
    float mouseY;
    static void setPlayerPosition(float x, float y, float z);

    virtual void mouseMoved(int x, int y, int relX, int relY);
#ifdef OIS
    virtual void keyPressed(SDL_Event event);
    virtual void keyReleased(SDL_Event event);
    virtual void mousePressed(SDL_Event event);
    virtual void mouseRleased(SDL_Event event);
#else
    bool mouseMoved(const OIS::MouseEvent &arg);
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
#endif

    boost::signals2::signal<void ()> sig_newGame;
    boost::signals2::signal<void ()> sig_contineGame;
    boost::signals2::signal<void ()> sig_quitGame;

    boost::shared_ptr<voxel::TerrainGenerator> terrainGenerator;

protected:
    void setupShadows();
    virtual void createScene();
    virtual void destroyScene(void);

    void keyPressed(utility::EntKeyCode key);
    void keyReleased(utility::EntKeyCode key);
    void mousePressed(utility::EntKeyCode button);
    void mouseReleased(utility::EntKeyCode button);

    //void keyPressed(SDL_Keycode key);
    //void keyReleased(SDL_Keycode key);
    //void mousePressed(Uint8 button);
    //void mouseReleased(Uint8 button);

    bool wDown;
    bool aDown;
    bool sDown;
    bool dDown;
    int wTimer;
    int aTimer;
    int sTimer;
    int dTimer;
    int moveSpeed;
    bool startMenu;
    Update* updateThread;
    bool hacking;
    Terminal* terminal;
    boost::shared_mutex functionLock;
    std::deque< boost::function<void (void)> > queuedFunctions;
    struct GraphicsStruct {
        boost::function<void (uint16, int)> func;
        uint16 lod;
        int id;
    };
    std::deque<graphics::ManualEntity* > manualEntitiesForDeletion;
    std::deque< GraphicsStruct > graphicsQueuedFunctions;
    std::deque< Ogre::OverlayElement* > queuedDeleteOverlays;
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool frameStarted(const Ogre::FrameEvent &evt);
    virtual bool frameEnded(const Ogre::FrameEvent &evt);

    GameMenu* gameMenu;
    EntropyLog* log;
    Inventory* inventory;
    QuestMenu* questMenu;
    ChatMenu* chatMenu;
    EntropyCursor* cursor;
    Console* console;
    InputLoop* inputLoop;
    std::locale loc;
    std::deque<std::string> removedManualObjects;
    Ogre::ShadowCameraSetupPtr shadowCameraSetup;
    graphics::ShadowListener* shadowListener;
};

#endif // #ifndef __Entropy_h_
