// System includes
#include <iostream>
#include <ios>
#include <locale>

#ifndef WIN32
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

#include "boost/thread/shared_mutex.hpp"

// Ogre includes
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreFontManager.h"
#include "OGRE/OgreMaterial.h"

// Entropy includes
#include "Entropy.h"
#include "terminal.h"
#include "src/Physics/physics.h"
#include "src/Graphics/graphics.h"
#include "src/Graphics/OgreDebugDraw/DebugDrawer.h"
#include "src/Core/dotscene.h"
#include "Menu/inventoryitem.h"
#include "src/Script/script.h"
#include "src/gameplay/quest.h"
#include "src/gameplay/actors.h"
#include "src/gameplay/quest.h"
#include "update.h"
#include "terminal.h"
#include "src/Menu/gamemenu.h"
#include "src/Menu/entropylog.h"
#include "Menu/inventory.h"
#include "Menu/chatmenu.h"
#include "Menu/entropycursor.h"
#include "Menu/console.h"
#include "Menu/questmenu.h"
#include "VoxelTerrain/terraingenerator.h"
#include "Graphics/ShadowListener.h"
#include "Graphics/Deferred/deferred.h"
#include "gameplay/actorupdateactions.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreOverlayManager.h"
#include "OGRE/OgreMeshManager.h"
#include "src/gameplay/playerinputaction.h"
#include "Graphics/Deferred/materialgenerator.h"
#include "VoxelTerrain/terrainmultiverse.h"

class InventoryItem;

using namespace Ogre;

Entropy::Entropy() :
    startMenu(true)
{
#ifndef OIS
    utility::initializeTranslationMaps();
#endif
}

Entropy::~Entropy()
{
    delete updateThread;
    delete terminal;
}

void Entropy::createScene()
{
    //mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    //    mouseMovedSig.connect(boost::bind(&Entropy::mouseMovedSlot,this,_1,_2));
    cursor = new EntropyCursor();
    Ogre::FontPtr font = Ogre::FontManager::getSingleton().create("bluehigh", "General");
    font->setParameter("type", "truetype");
    font->setParameter("source", "bluehigh.ttf");
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
    asciiFont->setParameter("resolution", "90");
    asciiFont->load();
    Ogre::MaterialPtr asciiFontMaterial = asciiFont.getPointer()->getMaterial();

    gameMenu = new GameMenu();

    log = new EntropyLog();
    inventory = new Inventory();
    questMenu = new QuestMenu();
    chatMenu = new ChatMenu();
    mCameraMan->initCollisionObject();
    gameplay::GrapplingHook::create(); // Create the grappling hook action

    console = new Console;
    script::initialize(); // Initialize Lua, wrap our classes and load the script library
    physics::world.setDebugDrawer(&graphics::detail::bulletDebugDraw);
    physics::addDefaultCollisionActions();
    //HAVE TO UNDO THIS SO I CAN WORK ON THE VOXEL SITUATION
    //    shadowListener = new graphics::ShadowListener(mSceneMgr);
    graphics::deferred::initialize(mWindow->getViewport(0), mSceneMgr, mCamera);

    Ogre::ColourValue fadeColour(0.0, 0.0, 0.0);
    mWindow->getViewport(0)->setBackgroundColour(fadeColour);
    mSceneMgr->setFog(Ogre::FOG_EXP2, fadeColour, 0.003);

    gameMenu->startNewGame();
}



void Entropy::startNewGame()
{
    std::cout << "startNewGame()" << std::endl;
    startMenu = false;
    //#ifndef __APPLE__
    utility::initializeTranslationMaps();
    //#endif

    gameplay::actors::setActors(
                dotscene::parseDotScene(
                    "TestLevel.scene",
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                    mSceneMgr
                    )
                );

    hacking = false;
    wDown = false;
    sDown = false;
    aDown = false;
    dDown = false;
    wTimer = 0;
    aTimer = 0;
    sTimer = 0;
    dTimer = 0;
    moveSpeed = 0;
    terminal = new Terminal();
    updateThread = new Update(this);
    sig_newGame();
    terrainGenerator = boost::shared_ptr<voxel::TerrainGenerator>(new voxel::TerrainGenerator(MAPSIZE_X_NONSCALED,MAPSIZE_Y_NONSCALED,MAPSIZE_Z_NONSCALED,MAPSIZE_SCALE));
    //terrainGenerator->genSimplex(2,5,2,0.115,0.115,0.2);
    mCameraMan->toggleFlashLights();
}

void Entropy::continueGame()
{
    utility::initializeTranslationMaps();
    gameplay::actors::setActors(
                dotscene::parseDotScene("TestLevel.scene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSceneMgr)
                );

    hacking = false;
    wDown = false;
    sDown = false;
    aDown = false;
    dDown = false;
    wTimer = 0;
    aTimer = 0;
    sTimer = 0;
    dTimer = 0;
    moveSpeed = 0;
    terminal = new Terminal();
    updateThread = new Update(this);
}


void Entropy::destroyScene()
{
    gameplay::PlayerInputAction::clear();
    physics::removeDefaultCollisionActions();
    gameplay::actors::clearActors();
    graphics::setDebugDrawingEnabled(false, 0);
    gameplay::MetaQuest::reset();
    graphics::deferred::cleanup();
    //delete shadowListener;
}

void Entropy::update()
{
    if(hacking) {
        if(wDown) {
            wTimer+=1;
            wTimer = 0;
            terminal->moveHero(259);
        }
        if(aDown) {
            aTimer+=1;
            aTimer = 0;
            terminal->moveHero(260);
        }
        if(sDown) {
            sTimer+=1;
            sTimer = 0;
            terminal->moveHero(258);
        }
        if(dDown) {
            dTimer+=1;
            dTimer = 0;
            terminal->moveHero(261);
        }
        terminal->updateTerminal(1);
    } else {
        if(mCameraMan)
            mCameraMan->update();

    }

    //    voxel::TerrainMultiVerse::getSingletonPtr()->update();
}


bool Entropy::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    //    boost::upgrade_lock<boost::shared_mutex> lock(functionLock);
    //    boost::unique_lock<boost::shared_mutex> uniqueLock(functionLock);

    if(terrainGenerator.use_count()>0)
    {
        terrainGenerator->getWater()->updateWater();
    }

    gameplay::actors::updateTransform();

    int queueSize = 10;

    for(int i=0;i<1;i++)
    {
        if(!queuedFunctions.empty())
        {
            try
            {
                queuedFunctions.front()();
            }
            catch(boost::bad_function_call &fc)
            {
                std::cerr << "QueuedFunction, bad_function_call" << std::endl;
            }
            catch(std::exception &e)
            {
                std::cerr << "QueuedFunction exception: " << e.what() << std::endl;
            }
            catch(...)
            {
                std::cerr << "Unknown QueuedFunction exception." << std::endl;
            }
            queuedFunctions.pop_front();
        }
        else
        {
            break;
        }

    }

    //Deque version
    for(int i=0;i<1;i++)
    {

        if(!graphicsQueuedFunctions.empty())
        {
            try
            {
                graphicsQueuedFunctions.front().func(graphicsQueuedFunctions.at(i).lod,graphicsQueuedFunctions.at(i).id);
            } catch(...) {

            }
            graphicsQueuedFunctions.pop_front();
        }
        else
        {
            break;
        }

    }

    for(int i=0;i<20;i++)
    {

        if(!queuedDeleteOverlays.empty())
        {
            try
            {
                Terminal::getSingletonPtr()->_overlayMgr->destroyOverlayElement(queuedDeleteOverlays.front());
            } catch(...) {
                std::cerr << "Couldn't delete overlay element." << std::endl;
            }
            queuedDeleteOverlays.pop_front();
        }
        else
        {
            break;
        }

    }

    for(int i=0;i<1;i++)
    {
        if(!removedManualObjects.empty())
        {
            try
            {
                std::cout << "Ogre::MeshManager::getSingletonPtr()->remove(removedManualObjects.at(i));" << std::endl;
                Ogre::MeshManager::getSingletonPtr()->remove(removedManualObjects.front());
            }
            catch(std::exception& e )
            {
                std::cerr << "Error removing ManualObject: " << e.what() << std::endl;
            }
            removedManualObjects.pop_front();
        }
        else
        {
            break;
        }
    }

    for(int i=0;i<1;i++)
    {
        if(!manualEntitiesForDeletion.empty())
        {
            if(manualEntitiesForDeletion.front()!=0)
            {
                delete manualEntitiesForDeletion.front();
            }
            manualEntitiesForDeletion.pop_front();
        }
        else
        {
            break;
        }
    }

    return BaseApplication::frameRenderingQueued(evt);
}

int drawCount = 0;

bool Entropy::frameStarted(const FrameEvent &evt)
{

#ifndef OIS
    handleInput();
#endif
    if(graphics::getDebugDrawingEnabled())
    {
        /*
        if(drawCount > 120)
        {
            physics::world.drawDebug();
            drawCount = 0;
        }
        drawCount++;*/

        physics::world.drawDebug();
        gameplay::astar.debugDraw();
        graphics::beginDebugDraw();
    }

    // Ogre::OverlayElement* element = Ogre::OverlayManager::getSingletonPtr()->getOverlayElement("mousePanel");
    // std::cout << "Entropy::frameStarted element position: " << element->getLeft() << ", " << element->getTop() << std::endl;

    return Ogre::FrameListener::frameStarted(evt);
}

bool Entropy::frameEnded(const FrameEvent &evt)
{
    if(graphics::getDebugDrawingEnabled())
        graphics::finishDebugDraw();

    // Ogre::OverlayElement* element = Ogre::OverlayManager::getSingletonPtr()->getOverlayElement("mousePanel");
    // std::cout << "Entropy::frameEnded element position: " << element->getLeft() << ", " << element->getTop() << std::endl;
    return Ogre::FrameListener::frameEnded(evt);
}



#ifdef OIS
void Entropy::keyReleased(SDL_Event event)
{
    keyReleased(utility::keyMap.at(event.key.keysym.sym));
}

void Entropy::keyPressed(SDL_Event event)
{
    keyPressed(utility::keyMap.at(event.key.keysym.sym));
}

void Entropy::mousePressed(SDL_Event event)
{
    mousePressed(utility::mouseMap.at(event.button.button));
}

void Entropy::mouseRleased(SDL_Event event)
{
    mouseReleased(utility::mouseMap.at(event.button.button));
}

#else

bool Entropy::mouseMoved(const OIS::MouseEvent &arg)
{
    mouseMoved(arg.state.X.abs, arg.state.Y.abs, arg.state.X.rel, arg.state.Y.rel);
    return true;
}

bool Entropy::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    mousePressed(utility::mouseMap.at(id));
    return true;
}

bool Entropy::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    mouseReleased(utility::mouseMap.at(id));
    return true;
}

bool Entropy::keyPressed(const OIS::KeyEvent &arg)
{
    keyPressed(utility::keyMap.at(arg.key));
    return true;
}

bool Entropy::keyReleased(const OIS::KeyEvent &arg)
{
    keyReleased(utility::keyMap.at(arg.key));
    return true;
}

#endif

void Entropy::mouseMoved(int x, int y, int relX, int relY)
{
    if(gameMenu->getMenuState() == GameMenu::Gameplay
            && log->getStatus() == EntropyLog::NotReading
            && !(inventory->isOpen()))
    {

        mCameraMan->mouseMove(relX * -1, relY * -1);
    }

    else
    {
        //Menu Mouse in Absolute Coordinates
        float mX = (float) x / (float) mWindow->getViewport(0)->getActualWidth();
        float mY = (float) y / (float) mWindow->getViewport(0)->getActualHeight();
        mouseX = mX;
        mouseY = mY;
        gameMenu->mouseMoved(mX, mY);
        log->mouseMoved(mX, mY);

        if(inventory->isOpen())
        {
            inventory->mouseMoved(mX, mY);
        }
    }
}


/*
// THIS NEED SOME SERIOUS FORMATTING LOVE. PROBABLY BETTER AS A SWITCH CALLING SEVERAL FUNCTIONS.
void Entropy::keyPressed(SDL_Keycode key)
{
    if(chatMenu->isOpen() && key != SDLK_ESCAPE && key != SDLK_TAB) {
        //        if(key == SDLK_DELETE || key == SDLK_BACKSPACE) {
        //            chatMenu->deleteText();
        //        } else if(key == SDLK_RETURN){
        //            chatMenu->returnKey();
        //        } else {
        //            const char* c = SDL_GetKeyName(key);
        //            chatMenu->enterText(c[0]);
        //        }
        if(key == SDLK_LSHIFT || key == SDLK_RSHIFT) {
            chatMenu->shiftDown();
        } else {
            chatMenu->enterText(key);
        }
    } else if(console->isOpen() && key != SDLK_ESCAPE
              && key != SDLK_BACKQUOTE
              ) {
        //        if(key == SDLK_DELETE || key == SDLK_BACKSPACE) {
        //            console->deleteText();
        //        } else if(key == SDLK_RETURN){
        //            console->returnKey();
        //        } else {
        //            char c = std::use_facet< std::ctype<wchar_t> >(loc).narrow(event.key.keysym.unicode,'*');
        //            console->enterText(c);
        //        }
        if(key == SDLK_LSHIFT || key == SDLK_RSHIFT) {
            console->shiftDown();
        } else {

            console->enterText(key);
        }
    } else {
        if (key == SDLK_w)
        {
            if(!wDown) {
                wDown = true;
            }

        } else if (key == SDLK_a)
        {
            if(!aDown) {
                aDown = true;
            }
        } else if (key == SDLK_s)
        {
            if(!sDown) {
                sDown = true;
            }
        } else if (key == SDLK_d)
        {
            if(!dDown) {
                dDown = true;
            }
        } else if(key == SDLK_n) {
            if(!hacking) {
                if(gameMenu->getMenuState()==GameMenu::Gameplay && log->getStatus()==EntropyLog::NotReading&& !(inventory->isOpen()) && !(questMenu->isOpen())
                        && console->isClosed()
                        ) {
                    //Start Neurocaster
                    terminal->initTerminal("test"," ",0,0, 1280,1024, Ogre::ColourValue(1.0,1.0,1.0,1.0));
                    hacking = true;
                }
            } else {
                //Shutdown Neurocaster
                hacking = false;
                terminal->closeTerminal();
                hacking = false;
            }
        } else if(key == SDLK_m) {
            if(hacking) {
                terminal->setMapView(true);
            }
        } else if(key == SDLK_LEFT) {
            if(hacking) {
                terminal->button1();
            }
        } else if(key == SDLK_RIGHT) {
            if(hacking) {
                terminal->button2();
            }
        } else if(key == SDLK_UP) {
            if(hacking) {
                terminal->button3();
            }
        } else if(key == SDLK_DOWN) {
            if(hacking) {
                terminal->button4();
            }
        } else if(key == SDLK_ESCAPE) {
            if(log->getStatus()==EntropyLog::Reading) {
                log->closeLog();
            } else if(inventory->isOpen()) {
                inventory->closeInventory();
            } else if(hacking){
                //Shutdown Neurocaster
                hacking = false;
                terminal->closeTerminal();
            } else if(questMenu->isOpen()) {
                questMenu->close();
            } else if(chatMenu->isOpen()){
                chatMenu->close();
            } else if(console->isOpen()){
                console->close();
            } else {
                gameMenu->openMainMenu();
            }
        } else if(key == SDLK_1)
        {
            graphics::setDebugDrawingEnabled(!graphics::getDebugDrawingEnabled(), mSceneMgr);
        } else if(key == SDLK_l) {
            if(!(inventory->isOpen()) && gameMenu->getMenuState()==GameMenu::Gameplay&& !hacking && !(questMenu->isOpen())
                    && console->isClosed()
                    ) {
                log->changeLog();
            }
        } else if(key == SDLK_i) {
            if(gameMenu->getMenuState()==GameMenu::Gameplay && log->getStatus()==EntropyLog::NotReading && !hacking &&  !(questMenu->isOpen())
                    && console->isClosed()
                    ) {
                inventory->change();
            }
        } else if(key == SDLK_q) {
            if(gameMenu->getMenuState()==GameMenu::Gameplay && log->getStatus()==EntropyLog::NotReading && !hacking && !(inventory->isOpen())
                    && console->isClosed()
                    ) {
                questMenu->change();
            }
        } else if(key == SDLK_TAB) {
            if(gameMenu->getMenuState()==GameMenu::Gameplay && log->getStatus()==EntropyLog::NotReading && !hacking && !(inventory->isOpen())
                    && console->isClosed()
                    ) {
                chatMenu->change();
            }
        } else if( key == SDLK_BACKQUOTE) {
            if(gameMenu->getMenuState()==GameMenu::Gameplay && log->getStatus()==EntropyLog::NotReading && !hacking && !(inventory->isOpen())) {
                console->change();
            }
        }

        mCameraMan->injectKeyDown(key);
    }
}

void Entropy::keyReleased(SDL_Keycode key)
{
    if(key == SDLK_w)
    {
        wDown = false;
    }

    else if(key == SDLK_a)
    {
        aDown = false;
    }

    else if(key == SDLK_s)
    {
        sDown = false;
    }

    else if(key == SDLK_d)
    {
        dDown = false;
    }

    else if(key == SDLK_m)
    {
        if(hacking)
        {
            terminal->setMapView(false);
        }
    }

    else if(key == SDLK_LSHIFT || key == SDLK_RSHIFT)
    {
        chatMenu->shiftUp();
        console->shiftUp();
    }

    mCameraMan->injectKeyUp(key);
}

void Entropy::mousePressed(Uint8 button)
{
    // physics::createProjectile(mCamera->getRealPosition(), mCamera->getRealDirection(), 250, 1, 5);
    gameMenu->mousePressed();
    log->mouseClicked(mouseX,mouseY);

    if(button == SDL_BUTTON_LEFT)
    {
        cursor->use();
    }

    else if(button == SDL_BUTTON_RIGHT)
    {
        inventory->unEquipItem();
    }

    if(inventory->isOpen())
    {
        inventory->mouseClicked(mouseX,mouseY);
    }

    else
    {
        mCameraMan->injectMouseDown(button);
    }
}

void Entropy::mouseReleased(Uint8 button)
{
    cursor->unUse();
    mCameraMan->injectMouseUp(button);
}
*/

void Entropy::keyPressed(utility::EntKeyCode key)
{
    if(chatMenu->isOpen() && key != utility::KC_ESCAPE && key != utility::KC_TAB) {
        //        if(key == SDLK_DELETE || key == SDLK_BACKSPACE) {
        //            chatMenu->deleteText();
        //        } else if(key == SDLK_RETURN){
        //            chatMenu->returnKey();
        //        } else {
        //            const char* c = SDL_GetKeyName(key);
        //            chatMenu->enterText(c[0]);
        //        }
        if(key == utility::KC_LSHIFT || key == utility::KC_RSHIFT) {
            chatMenu->shiftDown();
        } else {
            chatMenu->enterText(key);
        }
    } else if(console->isOpen() && key != utility::KC_ESCAPE
              && key != utility::KC_GRAVE
              ) {
        //        if(key == SDLK_DELETE || key == SDLK_BACKSPACE) {
        //            console->deleteText();
        //        } else if(key == SDLK_RETURN){
        //            console->returnKey();
        //        } else {
        //            char c = std::use_facet< std::ctype<wchar_t> >(loc).narrow(event.key.keysym.unicode,'*');
        //            console->enterText(c);
        //        }
        if(key == utility::KC_LSHIFT || key == utility::KC_RSHIFT) {
            console->shiftDown();
        } else {

            console->enterText(key);
        }
    } else {
        if (key == utility::KC_K)
        {
            terrainGenerator->clear();

        }
        else if (key == utility::KC_W)
        {
            if(!wDown) {
                wDown = true;
            }

        } else if (key == utility::KC_A)
        {
            if(!aDown) {
                aDown = true;
            }
        } else if (key == utility::KC_S)
        {
            if(startMenu)
            {
                startNewGame();
            }
            else
            {
                if(!sDown) {
                    sDown = true;
                }
            }
        } else if (key == utility::KC_D)
        {
            if(!dDown) {
                dDown = true;
            }
        } else if(key == utility::KC_N) {
            if(!hacking) {
                if(gameMenu->getMenuState()==GameMenu::Gameplay && log->getStatus()==EntropyLog::NotReading&& !(inventory->isOpen()) && !(questMenu->isOpen())
                        && console->isClosed()
                        ) {
                    //Start Neurocaster
                    terminal->initTerminal("test"," ",0,0, 1280,1024, Ogre::ColourValue(1.0,1.0,1.0,1.0));
                    physics::world.setPaused(true);
                    hacking = true;
                }
            } else {
                //Shutdown Neurocaster
                hacking = false;
                terminal->closeTerminal();
                hacking = false;
                physics::world.setPaused(false);
            }
        } else if(key == utility::KC_M) {
            if(hacking) {
                terminal->setMapView(true);
            }
        } else if(key == utility::KC_V) {
            Ogre::Vector3 pos(mCameraMan->getPosition().x(),mCameraMan->getPosition().y(),mCameraMan->getPosition().z());
            terrainGenerator->removeChunkxel(pos);
        }else if(key == utility::KC_LEFT) {
            if(hacking) {
                terminal->button1();
            }
        } else if(key == utility::KC_RIGHT) {
            if(hacking) {
                terminal->button2();
            }
        } else if(key == utility::KC_UP) {
            if(hacking) {
                terminal->button3();
            }
        } else if(key == utility::KC_DOWN) {
            if(hacking) {
                terminal->button4();
            }
        } else if(key == utility::KC_ESCAPE) {
            if(log->getStatus()==EntropyLog::Reading) {
                log->closeLog();
            } else if(inventory->isOpen()) {
                inventory->closeInventory();
            } else if(hacking){
                //Shutdown Neurocaster
                hacking = false;
                terminal->closeTerminal();
            } else if(questMenu->isOpen()) {
                questMenu->close();
            } else if(chatMenu->isOpen()){
                chatMenu->close();
            } else if(console->isOpen()){
                console->close();
            } else {
                gameMenu->openMainMenu();
            }
        } else if(key == utility::KC_1)
        {
            graphics::setDebugDrawingEnabled(!graphics::getDebugDrawingEnabled(), mSceneMgr);
        } else if(key == utility::KC_L) {
            if(!(inventory->isOpen()) && gameMenu->getMenuState()==GameMenu::Gameplay&& !hacking && !(questMenu->isOpen())
                    && console->isClosed()
                    ) {
                log->changeLog();
            }
        } else if(key == utility::KC_I) {
            if(gameMenu->getMenuState()==GameMenu::Gameplay && log->getStatus()==EntropyLog::NotReading && !hacking &&  !(questMenu->isOpen())
                    && console->isClosed()
                    ) {
                inventory->change();
            }
        } else if(key == utility::KC_Q) {
            if(gameMenu->getMenuState()==GameMenu::Gameplay && log->getStatus()==EntropyLog::NotReading && !hacking && !(inventory->isOpen())
                    && console->isClosed()
                    ) {
                questMenu->change();
            }
        } else if(key == utility::KC_TAB) {
            if(gameMenu->getMenuState()==GameMenu::Gameplay && log->getStatus()==EntropyLog::NotReading && !hacking && !(inventory->isOpen())
                    && console->isClosed()
                    ) {
                chatMenu->change();
            }
        } else if( key == utility::KC_GRAVE) {
            if(gameMenu->getMenuState()==GameMenu::Gameplay && log->getStatus()==EntropyLog::NotReading && !hacking && !(inventory->isOpen())) {
                console->change();
            }
        }
        else if( key == utility::KC_F1)
        {
             terrainGenerator->genSimplex(2,5,2,0.115,0.115,0.2);
//            terrainGenerator->genSimplex(2,5,2,0.075,0.075,0.0);
        }

        mCameraMan->injectKeyDown(key);
    }
}

void Entropy::keyReleased(utility::EntKeyCode key)
{
    if(key == utility::KC_W)
    {
        wDown = false;
    }
    else if(key == utility::KC_A)
    {
        aDown = false;
    }
    else if(key == utility::KC_S)
    {
        sDown = false;
    }
    else if(key == utility::KC_D)
    {
        dDown = false;
    }
    else if(key == utility::KC_M)
    {
        if(hacking)
        {
            terminal->setMapView(false);
        }
    }
    else if(key == utility::KC_LSHIFT || key == utility::KC_RSHIFT)
    {
        chatMenu->shiftUp();
        console->shiftUp();
    }

    mCameraMan->injectKeyUp(key);
}

void Entropy::mousePressed(utility::EntKeyCode button)
{
    // physics::createProjectile(mCamera->getRealPosition(), mCamera->getRealDirection(), 250, 1, 5);
    gameMenu->mousePressed();
    log->mouseClicked(mouseX,mouseY);

    if(button == utility::MB_Left)
    {
        cursor->use();
    }
    else if(button == utility::MB_Right)
    {
        inventory->unEquipItem();
    }

    if(inventory->isOpen())
    {
        inventory->mouseClicked(mouseX,mouseY);
    }
    else
    {
        mCameraMan->injectMouseDown(button);
    }
}

void Entropy::mouseReleased(utility::EntKeyCode button)
{
    cursor->unUse();
    mCameraMan->injectMouseUp(button);
}


void Entropy::addQeueudFunction(boost::function<void (void)> func)
{
    //    boost::shared_lock<boost::shared_mutex> lock(functionLock);
    //    boost::unique_lock<boost::shared_mutex> uniqueLock(functionLock);
    queuedFunctions.push_back(func);
}

void Entropy::addQeueudFunction(boost::function<void (uint16,int)> func,uint16 lod, int id)
{
    //    boost::shared_lock<boost::shared_mutex> lock(functionLock);
    //    boost::unique_lock<boost::shared_mutex> uniqueLock(functionLock);
    GraphicsStruct gStruct = {func,lod,id};
    graphicsQueuedFunctions.push_back(gStruct);
}

void Entropy::addQeueudDeleteOverlays(OverlayElement* overlaysElement)
{
    queuedDeleteOverlays.push_back(overlaysElement);
}

void Entropy::addQueuedDeleteManualObject(std::string mo)
{
    removedManualObjects.push_back(mo);
}

void Entropy::addQueuedDeleteManualEntity(graphics::ManualEntity *me)
{
    manualEntitiesForDeletion.push_back(me);
}

void Entropy::quitGame()
{
    std::cout << "QUITTING GAME!!!!!!!!!!!!!!!!!" << std::endl;
    mShutDown = true;
}

void Entropy::setPlayerPosition(float x, float y, float z)
{
    std::cout<<"Entropy::setPlayerPosition("<<x<<","<<y<<","<<z<<")"<<std::endl;
    ((CollisionCamera*)Entropy::getSingletonPtr()->mCameraMan)->setCameraPosition(x,y,z);
    //    Entropy::getSingletonPtr()->mCameraMan->setPosition(btVector3(x,y,z));
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> Entropy* Ogre::Singleton<Entropy>::ms_Singleton = 0;
Entropy* Entropy::getSingletonPtr(void)
{
    return ms_Singleton;
}
Entropy& Entropy::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> Entropy* Ogre::Singleton<Entropy>::msSingleton = 0;
Entropy* Entropy::getSingletonPtr(void)
{
    return msSingleton;
}
Entropy& Entropy::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
