#include "neurocaster.h"
#include "boost/thread/shared_mutex.hpp"

// Ogre includes
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreFontManager.h"
#include "OGRE/OgreMaterial.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreOverlayManager.h"
#include "OGRE/OgreMeshManager.h"

//#include "Core/update.h"
//#include "Hypnos/src/script/luafunctions.h"

//Neurocaster includes
#include "Menu/gamemenu.h"
#include "update.h"
#include "gameplay/neuron.h"
#include "gameplay/astar.h"
#include "src/Menu/playermenu.h"
#include "src/Menu/playeractiongui.h"
#include "utility/util.h"
#include "src/graphics/abilityicongenerator.h"

typedef struct _sig{
    double* output; //Pointer to array equal to M
    double* input;
    double* b; //supplied filter coefficients
    double* si; //temp storage for signal samples
} sig;

int M = 3;

void verb(sig *s)
{
    for(int n=0;n<M;++n)
    {
        s->si[0] = s->input[n];
        s->output[n] = s->b[0] * s->si[0] + s->b[1] * s->si[1]  +  s->b[2] * s->si[2];
        s->si[2] = s->si[1];
        s->si[1] = s->si[0];
    }
}

Neurocaster::Neurocaster() :
    current_zoom(0.5),
    current_pan_x(0),
    current_pan_y(0),
    zoom_in(false),
    zoom_out(false),
    pan_left(false),
    pan_right(false),
    pan_up(false),
    pan_down(false),
    ascii_effect(false),
    playerMenu(NULL)
{

}

Neurocaster::~Neurocaster()
{

}

void Neurocaster::createScene()
{
    std::cout << "CREATING SCENE!" << std::endl;

    Ogre::FontPtr font = Ogre::FontManager::getSingleton().create("ocra", "General");
    font->setParameter("type", "truetype");
    font->setParameter("source", "OCRAEXT.ttf");
    // font->setParameter("source", "bluehigh.ttf");
    font->setParameter("size", "20");
    font->setParameter("resolution", "120");
    font->load();

    //Console Font
    Ogre::FontPtr consolefont = Ogre::FontManager::getSingleton().create("ocra", "General");
    consolefont->setParameter("type", "truetype");
    consolefont->setParameter("source", "OCRAEXT.ttf");
    consolefont->setParameter("size", "24");
    consolefont->setParameter("resolution", "90");
    consolefont->load();

    //Console Font
    Ogre::FontPtr asciiFont = Ogre::FontManager::getSingleton().create("asciiFont", "Essential");
    asciiFont->setParameter("type", "truetype");
    asciiFont->setParameter("source", "OCRAEXT.ttf");
    asciiFont->setParameter("size", "14");
    asciiFont->setParameter("resolution", "180");
    asciiFont->load();
    Ogre::MaterialPtr asciiFontMaterial = asciiFont.getPointer()->getMaterial();

    gui::NeuroStyle::setAsCurrentStyle();
    neuro::astar::initializeWalkMaps();

    Ogre::CompositorManager &compMan = Ogre::CompositorManager::getSingleton();
    asciiCompositor = compMan.addCompositor(mCamera->getViewport(),"AsciiComp");
    asciiCompositor->setEnabled(ascii_effect);

    neuro::util::moveCamera(current_zoom);

    scene_manager = neuro::SceneManager::Ptr(new neuro::SceneManager());
    scene_manager->init();
    gameMenu = new GameMenu();
    playerMenu = new gui::PlayerMenu(Ogre::Vector2(0.25, 0.25), Ogre::Vector2(0.75, 0.75));
    neuro::loadAbilityNameResources();
    playerActionGui = new gui::PlayerActionGui(Ogre::Vector2(0.025, 0.9125), Ogre::Vector2(0.4, 0.95));
    update_loop = new Update();
    mCamera->setProjectionType(PT_ORTHOGRAPHIC);

    //    moveCamera(2);

}

bool Neurocaster::mouseMoved(const OIS::MouseEvent &arg)
{
    mouseMoved(arg.state.X.abs, arg.state.Y.abs, arg.state.X.rel, arg.state.Y.rel);
    current_zoom += (((float)arg.state.Z.rel) * -0.001);
    neuro::util::moveCamera(current_zoom,current_pan_x,current_pan_y);
    return true;
}

bool Neurocaster::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if(gameMenu->isVisible())
    {
        gameMenu->mousePressed();
    }

    else if(playerMenu->isVisible())
    {
        playerMenu->mousePressed();
    }

    else if(playerActionGui->getLastMouseOverWidget())
    {
        playerActionGui->mousePressed();
    }

    else
    {

//        getCurrentMap()->getPlayer()->useAbility(getCurrentMap()->getSelectedTile());
        switch(getCurrentMap()->getSelectedTile()->getChar())
        {
        case neuro::Tile::PORT:
            playerActionGui->setContext(neuro::Ability::CRYPTOGRAPHY,neuro::Ability::WHITE_HAT);
            break;
        case neuro::Tile::ENEMYPORT:
            playerActionGui->setContext(neuro::Ability::CRYPTOGRAPHY,neuro::Ability::BLACK_HAT);
            break;
        case neuro::Tile::NODE:
            playerActionGui->setContext(neuro::Ability::INFECTION,neuro::Ability::WHITE_HAT);
            break;
        case neuro::Tile::ENEMYNODE:
            playerActionGui->setContext(neuro::Ability::INFECTION,neuro::Ability::BLACK_HAT);
            break;
        default:
            playerActionGui->setContext(neuro::Ability::CYBERNETICS,neuro::Ability::WHITE_HAT);
            getCurrentMap()->getPlayer()->findPath(getCurrentMap()->tile_hilight->getPos());
            break;
        }
    }

    return true;
}

bool Neurocaster::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if(playerMenu->isVisible())
        playerMenu->mouseReleased();
    else if(playerActionGui->getLastMouseOverWidget())
        playerActionGui->mouseReleased();

    //    mouseReleased(utility::mouseMap.at(id));
    return true;
}

bool Neurocaster::keyPressed(const OIS::KeyEvent &arg)
{
    switch(arg.key)
    {
    case OIS::KC_Z:
        zoom_in = true;
        break;

    case OIS::KC_X:
        zoom_out = true;
        break;

    case OIS::KC_W:
        pan_up = true;
        break;

    case OIS::KC_A:
        pan_left = true;
        break;

    case OIS::KC_S:
        pan_down = true;
        break;

    case OIS::KC_D:
        pan_right = true;
        break;
    case OIS::KC_E:
        ascii_effect = !ascii_effect;
        asciiCompositor->setEnabled(ascii_effect);
        break;
    case OIS::KC_SPACE:
        update_loop->getSingletonPtr()->resume_from_player();
        break;

    case OIS::KC_ESCAPE:
        gameMenu->openMainMenu();
        break;

    case OIS::KC_P:
        playerMenu->toggleVisible();
        break;

    case OIS::KC_1:
    case OIS::KC_2:
    case OIS::KC_3:
    case OIS::KC_4:
    case OIS::KC_5:
    case OIS::KC_6:
        playerActionGui->keyPressed(arg);
        break;
    }

    return true;
}

bool Neurocaster::keyReleased(const OIS::KeyEvent &arg)
{
    switch(arg.key)
    {
    case OIS::KC_Z:
        zoom_in = false;
        break;

    case OIS::KC_X:
        zoom_out = false;
        break;

    case OIS::KC_W:
        pan_up = false;
        break;

    case OIS::KC_A:
        pan_left = false;
        break;

    case OIS::KC_S:
        pan_down = false;
        break;

    case OIS::KC_D:
        pan_right = false;
        break;
    }

    return true;
}

void Neurocaster::mouseMoved(int x, int y, int relX, int relY)
{
    //Menu Mouse in Absolute Coordinates
    float mX = (float) x / (float) mWindow->getViewport(0)->getActualWidth();
    float mY = (float) y / (float) mWindow->getViewport(0)->getActualHeight();
    mouseX = mX;
    mouseY = mY;
    gameMenu->mouseMoved(mX, mY);


    if(playerMenu->isVisible())
        playerMenu->mouseMoved(mX, mY);
    else
        playerActionGui->mouseMoved(mX, mY);

    if(scene_manager->isInitialized())
    {
        Square mt = neuro::util::mouseToTile(x,y);
        getCurrentMap()->tile_hilight->manualSetPos(mt);
    }
}

bool Neurocaster::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(zoom_in)
    {
        current_zoom += 0.01;
        neuro::util::moveCamera(current_zoom,current_pan_x,current_pan_y);
    }
    else if(zoom_out)
    {
        current_zoom -= 0.01;
        neuro::util::moveCamera(current_zoom,current_pan_x,current_pan_y);
    }

    if(pan_left)
    {
        current_pan_x -= 0.01;
        neuro::util::moveCamera(current_zoom,current_pan_x,current_pan_y);
    }
    else if(pan_right)
    {
        current_pan_x += 0.01;
        neuro::util::moveCamera(current_zoom,current_pan_x,current_pan_y);
    }

    if(pan_up)
    {
        current_pan_y += 0.01;
        neuro::util::moveCamera(current_zoom,current_pan_x,current_pan_y);
    }
    else if(pan_down)
    {
        current_pan_y -= 0.01;
        neuro::util::moveCamera(current_zoom,current_pan_x,current_pan_y);
    }

    boost::unique_lock<boost::shared_mutex> lock(async_func_mutex);

    if(async_functions.size() > 0)
    {
        for(int i = 0; i < async_functions.size(); ++i)
        {
            async_functions.at(i)();
        }
    }

    async_functions.clear();

    return BaseApplication::frameRenderingQueued(evt);
}

bool Neurocaster::frameStarted(const Ogre::FrameEvent &evt)
{
    return Ogre::FrameListener::frameStarted(evt);
}

bool Neurocaster::frameEnded(const Ogre::FrameEvent &evt)
{
    return Ogre::FrameListener::frameEnded(evt);
}


void Neurocaster::destroyScene()
{

}

void Neurocaster::startNewGame()
{
    std::cout << "startNewGame()" << std::endl;
    startMenu = false;
    sig_newGame();
}

void Neurocaster::continueGame()
{

}

void Neurocaster::shutdownGame()
{
    mShutDown = true;
}

void Neurocaster::quitGame()
{
    std::cout << "QUITTING GAME!!!!!!!!!!!!!!!!!" << std::endl;

    update_loop->stop();
    BaseApplication::quitGame();
}

void Neurocaster::update()
{
    scene_manager->continuous_update();
}

void Neurocaster::update_turn(double turn)
{
    scene_manager->update_turn(turn);
}

void Neurocaster::new_round()
{
    scene_manager->getCurrentScene()->new_round();
}

void Neurocaster::register_async_func(boost::function<void ()> func)
{
    boost::unique_lock<boost::shared_mutex> lock(async_func_mutex);
    async_functions.push_back(func);
}

neuro::Map::Ptr Neurocaster::getCurrentMap()
{
    return boost::static_pointer_cast<neuro::Map>(scene_manager->getCurrentScene());
}

double Neurocaster::getAspectRatio() const
{
    return mWindow->getViewport(0)->getWidth() / mWindow->getViewport(0)->getHeight();
}

#ifndef __APPLE__
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
    // Create application object
    Neurocaster app;

    try {
        app.go();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        //        MessageBox( NULL, CT2CW(e.getFullDescription().c_str()), L"An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#else
        std::cerr << "An exception has occured: " <<
                     e.getFullDescription().c_str() << std::endl;
#endif
    } catch (std::exception& e) {
        std::cerr << "An exception has occured: " << e.what() << std::endl;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
#endif

template<> Neurocaster* Ogre::Singleton<Neurocaster>::msSingleton = 0;
Neurocaster* Neurocaster::getSingletonPtr(void)
{
    return msSingleton;
}
Neurocaster& Neurocaster::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
