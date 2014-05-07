#include "terminal.h"

#include <OGRE/Ogre.h>

#include "neuro/map.h"
#include "boost/lexical_cast.hpp"
#include "src/Core/Entropy.h"

#include "neuro/map.h"
#include "neuro/room.h"
#include "neuro/Global.h"
#include "neuro/hero.h"

#include "OGRE/OgreRectangle2D.h"

using namespace Ogre;

Terminal::Terminal()
{
    /*
    scale = 0.8;
    _overlayMgr = Ogre::OverlayManager::getSingletonPtr();

    _overlay = _overlayMgr->create("overlay1");
    _overlay2 = _overlayMgr->create("overlay2");
    _overlay2->setZOrder(105);
    _overlay3 = _overlayMgr->create("overlay3");
    _overlay3->setZOrder(120);
    //    _overlay4 = _overlayMgr->create("overlay4");
    //    _overlay4->setZOrder(140);
    relativeOverlay = _overlayMgr->create("relativeOverlay");
    relativeOverlay->setZOrder(140);
    isScrollRoom = false;
    mapView = false;
    //Panel 1
    _panel = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel", "container1"));
    _panel->setDimensions(1, 1);
    _panel->setPosition(0, 0);
    _overlay->add2D(_panel);
    _panel2 = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel", "container2"));
    _panel2->setDimensions(1, 1);
    _panel2->setPosition(0, 0);
    _overlay2->add2D(_panel2);
    _panel3 = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel", "container3"));
    _panel3->setDimensions(1, 1);
    _panel3->setPosition(0, 0);
    _overlay3->add2D(_panel3);
    //    _panel4 = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel", "container4"));
    //    _panel4->setDimensions(1, 1);
    //    _panel4->setPosition(0, 0);
    //    _panel4->setMetricsMode(Ogre::GMM_PIXELS);
    //    _overlay4->setMetricsMode(Ogre::GMM_RELATIVE);
    //    _overlay4->setScale(1.0,1.0);
    //    _overlay4->add2D(_panel4);

    relativePanel = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel", "relativePanel"));
    relativePanel->setMetricsMode(Ogre::GMM_RELATIVE);
    relativePanel->setDimensions(1, 1);
    relativePanel->setPosition(0, 0);
    relativePanel->show();
    relativeOverlay->setScale(1.0,1.0);
    //    relativeOverlay->add2D(_panel4);
    relativeOverlay->add2D(relativePanel);
    relativeOverlay->show();

    //    _overlay->show();
    _overlay->hide();
    _overlay2->hide();
    _overlay3->hide();

    //    _overlay->setZOrder();
    srand ( time(NULL) );
    pixelSize = 110;
    _overlay->setScale(scale,scale);
    _overlay2->setScale(scale,scale);
    _overlay3->setScale(scale,scale);
    hacking = false;
    */
    Ogre::CompositorManager &compMan = Ogre::CompositorManager::getSingleton();
    asciiCompositor = compMan.addCompositor(Entropy::getSingletonPtr()->mWindow->getViewport(0),"AsciiComp");



    //NEW NON-OVERLAY VERSION
    background = createTexturedRect("terminal_background","Obsidian_COLOR.jpg",-1.0, 1.0, 1.0, -1.0);

    // Render the background before everything else
//    background->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);
}

Terminal::~Terminal()
{
    int tcount=0;
    if(hacking) {
        closeTerminal();
    }
    _overlayMgr->destroyOverlayElement(_panel);
    _overlayMgr->destroyOverlayElement(_panel2);
    _overlayMgr->destroyOverlayElement(_panel3);
    //    _overlayMgr->destroyAll();
    //    delete _overlay;
    //    delete _overlay2;
    //    delete _overlay3;
    //    delete _overlayMgr;
}

void Terminal::initTerminal(const std::string& ID,
                            const std::string& text,
                            Ogre::Real x, Ogre::Real y,
                            Ogre::Real width, Ogre::Real height,
                            const Ogre::ColourValue& color)
{
//    asciiCompositor->setEnabled(true);
    background->setVisible(true);
    /*
    Ogre::CompositorManager &compMan = Ogre::CompositorManager::getSingleton();
//    asciiCompositor = compMan.addCompositor(Entropy::getSingletonPtr()->mWindow->getViewport(0),"AsciiComp");
//    Ogre::RenderTarget *pRenderTexture =asciiCompositor->getRenderTarget("sceneTex");
    String TextureName = asciiCompositor->getTextureInstanceName("sceneTex",0);
    TexturePtr Texture = TextureManager::getSingleton().getByName(TextureName);
    RenderTexture *pRenderTexture = Texture->getBuffer()->getRenderTarget();
    Viewport* pViewport = pRenderTexture->getViewport(0);
    pViewport->setOverlaysEnabled(true);
    hacking = true;
    _overlay->show();
    _overlay2->show();
    _overlay3->show();
    _overlay3->show();
    _panel->show();
    _panel2->show();
    _panel3->show();
    //    _panel4->show();

    cols = neuro::MAPSIZEX;
    lines = neuro::MAPSIZEY;
    //    roomsX = neuro::nrand(2,3);
    //    roomsY = neuro::nrand(2,3);
    roomsX = 3;
    roomsY = 3;
    //    map = new neuro::Map(roomsX,roomsY,cols,lines);
    map = boost::shared_ptr<neuro::Map>(new neuro::Map(roomsX,roomsY,cols,lines));

    //====================
    //ROOMS
    //====================
    Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create("RoomMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    TextureUnitState* tex = mat->getTechnique(0)->getPass(0)->createTextureUnitState("Panels_refmask.png");
    //    TextureUnitState* tex2 =mat->getTechnique(0)->getPass(0)->createTextureUnitState("Panels_Diffuse.png");
    mat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    mat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    mat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    //    mat.getPointer()->getTechnique(0)->getPass(0)->sce

    Ogre::MaterialPtr unmat = Ogre::MaterialManager::getSingleton().create("UnRoomMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    TextureUnitState* tex3 = unmat->getTechnique(0)->getPass(0)->createTextureUnitState("Panels_refmask.png");
    //    TextureUnitState* tex4 = unmat->getTechnique(0)->getPass(0)->createTextureUnitState("Panels_Diffuse.png");
    unmat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    unmat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    unmat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    unmat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_DEST_COLOUR, Ogre::SBF_ONE_MINUS_SOURCE_COLOUR);

    Ogre::MaterialPtr ma2t = Ogre::MaterialManager::getSingleton().create("RoomBorder", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    ma2t->getTechnique(0)->getPass(0)->createTextureUnitState("Translucent_Glass_Tinted.jpg");
    ma2t->getTechnique(0)->getPass(0)->createTextureUnitState("Chrome.jpg");
    //    ma2t->getTechnique(0)->getPass(0)->createTextureUnitState("Panels_Normal_Tangent.png");
    ma2t->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    ma2t->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    ma2t->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    //    ma2t.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_DEST_COLOUR, Ogre::SBF_ONE_MINUS_SOURCE_COLOUR);


    int roomID = 0;
    //Create Rooms
    for(int x=0;x<roomsX;x++) {
        for(int y=0;y<roomsY;y++) {
            //            roomID++;
            //            map->getRoom(x,y)->setID(roomID);
            roomID = map->getRoom(x,y)->getID();
            //            std::cout << "ROOM ID: " <<roomID << std::endl;
            if(!map->getRoom(x,y)->isGone()) {
                int rx = map->getRoom(x,y)->getX()*pixelSize + pixelSize;
                int ry = map->getRoom(x,y)->getY()*pixelSize + pixelSize;
                int rw = map->getRoom(x,y)->getWidth()*pixelSize - pixelSize*2;
                int rh = map->getRoom(x,y)->getHeight()*pixelSize - pixelSize*2;

                //Occupied Room
                Ogre::OverlayContainer* room = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("BorderPanel", "room"+boost::lexical_cast<std::string>(roomID)));
                room->setMetricsMode(Ogre::GMM_PIXELS);
                room->setPosition(rx, ry);
                room->setWidth(rw);
                room->setHeight(rh);
                room->setParameter("transparent","false");
                room->setParameter("uv_coords","0 0 1 1");
                std::string xString = boost::lexical_cast<std::string>((rw+pixelSize*2)/pixelSize/4);
                std::string yString = boost::lexical_cast<std::string>((rh+pixelSize*2)/pixelSize/4);
                room->setParameter("tiling","0 "+xString+" "+yString);
                //                room->setParameter("tiling","0 0.25,0.25");
                room->setParameter("border_size","4 4 4 4");
                room->setParameter("border_topleft_uv","0 0 1 1");
                room->setParameter("border_bottomleft_uv","0 0 1 1");
                room->setParameter("border_bottomright_uv","0 0 1 1");
                room->setParameter("border_topright_uv","0 0 1 1");
                room->setParameter("border_bottom_uv","0 0 1 1");
                room->setParameter("border_left_uv","0 0 1 1");
                room->setParameter("border_right_uv","0 0 1 1");
                room->setParameter("border_top_uv","0 0 1 1");
                room->setMaterialName("RoomMat");
                room->setParameter("border_material","RoomBorder");

                //Explored Room
                Ogre::OverlayContainer* unroom = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("BorderPanel", "unroom"+boost::lexical_cast<std::string>(roomID)));
                unroom->setMetricsMode(Ogre::GMM_PIXELS);
                unroom->setPosition(rx, ry);
                unroom->setWidth(rw);
                unroom->setHeight(rh);
                unroom->setParameter("transparent","false");
                unroom->setParameter("uv_coords","0 0 1 1");
                unroom->setParameter("tiling","0 "+xString+" "+yString);
                unroom->setParameter("border_size","4 4 4 4");
                unroom->setParameter("border_topleft_uv","0 0 1 1");
                unroom->setParameter("border_bottomleft_uv","0 0 1 1");
                unroom->setParameter("border_bottomright_uv","0 0 1 1");
                unroom->setParameter("border_topright_uv","0 0 1 1");
                unroom->setParameter("border_bottom_uv","0 0 1 1");
                unroom->setParameter("border_left_uv","0 0 1 1");
                unroom->setParameter("border_right_uv","0 0 1 1");
                unroom->setParameter("border_top_uv","0 0 1 1");
                unroom->setMaterialName("UnRoomMat");
                unroom->setParameter("border_material","RoomBorder");
                if(map->getRoom(x,y)->getStatus()==neuro::OCCUPIED) {
                    room->show();
                    unroom->hide();
                } else {
                    unroom->show();
                    room->hide();
                }
                _panel->addChild(unroom);
                _panel->addChild(room);
            }
        }
    }

    //====================
    //HALLWAYS
    //====================
    Ogre::MaterialPtr mat3 = Ogre::MaterialManager::getSingleton().create("HallwayMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    mat3->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/Nanowires.jpg");
    mat3->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    mat3->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    mat3->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    //    boost::multi_array<neuro::Square, 2> terrainMap = map->getTerrainMap();
    hallwayID=0;
    for(int x=0;x<cols;x++) {
        for(int y=0;y<lines;y++) {
            if(map->getSquare(x,y)->getChar()==neuro::HALLWAY) {
                hallwayID++;
                int rx = map->getSquare(x,y)->getX()*pixelSize;
                int ry = map->getSquare(x,y)->getY()*pixelSize;
                Ogre::OverlayContainer* hallway = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel","hallway"+boost::lexical_cast<std::string>(hallwayID)));
                map->setSquareID(x,y,hallwayID);
                hallway->setMetricsMode(Ogre::GMM_PIXELS);
                hallway->setPosition(rx, ry);
                hallway->setWidth(pixelSize);
                hallway->setHeight(pixelSize);
                hallway->show();
                hallway->setParameter("uv_coords","0 0 0.5 0.5");
                hallway->setMaterialName("HallwayMat");
                _panel->addChild(hallway);
            }
        }
    }

    //====================
    //HERO
    //====================
    Ogre::MaterialPtr heroMat = Ogre::MaterialManager::getSingleton().create("heroMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    heroMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/Vamp.png");
    heroMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    heroMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    heroMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    heroMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr invheroMat = Ogre::MaterialManager::getSingleton().create("InvisibleHero", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    invheroMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/InvisibleHero.png");
    invheroMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    invheroMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    invheroMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    invheroMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
    //    heroMat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.25, 0.0);

    heroBox = _overlayMgr->createOverlayElement("Panel", "hero");
    heroBox->setMetricsMode(Ogre::GMM_PIXELS);
    heroBox->setDimensions(pixelSize,pixelSize);
    heroBox->setWidth(pixelSize*0.5);
    heroBox->setHeight(pixelSize*0.5);

    heroBox->setParameter("uv_coords","0.75 0.125 0.84 0.245");
    heroBox->setMaterialName("heroMat");

    _panel3->addChild(heroBox);
    //    _panel->addChild(textBox);
    _panel->setMetricsMode(Ogre::GMM_PIXELS);
    neuro::Vec2f pos = map->getHeroPos();
    _overlay->setScroll(1.0,-1.0);
    _overlay2->setScroll(1.0,-1.0);
    _overlay3->setScroll(1.0,-1.0);
    //    _overlay4->setScroll(1.0,-1.0);
    _panel->setPosition(-pixelSize*pos.x,-pixelSize*pos.y);
    _panel2->setMetricsMode(Ogre::GMM_PIXELS);
    _panel2->setPosition(-pixelSize*pos.x,-pixelSize*pos.y);
    _panel3->setMetricsMode(Ogre::GMM_PIXELS);
    _panel3->setPosition(-pixelSize*pos.x,-pixelSize*pos.y);


    //====================
    //GUI BARS
    //====================
    Ogre::MaterialPtr healthMat = Ogre::MaterialManager::getSingleton().create("healthMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    healthMat->getTechnique(0)->getPass(0)->createTextureUnitState("HeatLookup.tga");
    healthMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    healthMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    healthMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    healthBar = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel","healthBar"));
    healthBar->setMetricsMode(Ogre::GMM_RELATIVE);
    healthBar->setDimensions(pixelSize,pixelSize);
    healthBar->setPosition(-0.7,-0.65);
    healthBar->setWidth(0.45);
    healthBar->setHeight(0.0125);
    healthBar->setColour(ColourValue(1,0,0,0.75));
    healthBar->setParameter("uv_coords","1 1 1 1");
    healthBar->setMaterialName("healthMat");
    _overlay->add2D(healthBar);

    suspicionBar = static_cast<Ogre::OverlayContainer*>(_overlayMgr->createOverlayElement("Panel","suspicionBar"));
    suspicionBar->setMetricsMode(Ogre::GMM_RELATIVE);
    suspicionBar->setDimensions(pixelSize,pixelSize);
    suspicionBar->setPosition(-0.7,-0.625);
    suspicionBar->setWidth(0.025);
    suspicionBar->setHeight(0.0125);
    suspicionBar->setParameter("uv_coords","0.8 0.8 0.8 0.8");
    suspicionBar->setMaterialName("healthMat");
    _overlay->add2D(suspicionBar);
    */


}

void Terminal::removeTextBox(const std::string& ID)
{
    _panel->removeChild(ID);
    _overlayMgr->destroyOverlayElement(ID);
}

void Terminal::closeTerminal()
{
    background->setVisible(false);
    asciiCompositor->setEnabled(false);
    /*
    hacking = false;
    for(int x=0;x<roomsX;x++) {
        for(int y=0;y<roomsY;y++) {
            if(map->getRoom(x,y).use_count()>0) {
                int roomID = map->getRoom(x,y)->getID();
                if(!(map->getRoom(x,y)->isGone())) {
                    try{
                        Entropy::getSingletonPtr()->addQeueudDeleteOverlays(_overlayMgr->getOverlayElement("room"+boost::lexical_cast<std::string>(roomID)));
                    } catch(...) {

                    }
                    try{
                        Entropy::getSingletonPtr()->addQeueudDeleteOverlays(_overlayMgr->getOverlayElement("unroom"+boost::lexical_cast<std::string>(roomID)));
                    } catch(...) {

                    }
                }
            }
        }
    }
    for(int i=0;i<=hallwayID;i++) {
        try{
            Entropy::getSingletonPtr()->addQeueudDeleteOverlays(_overlayMgr->getOverlayElement("hallway"+boost::lexical_cast<std::string>(i)));
        } catch(...) {

        }
    }

    //    delete map;
    map->deleteResources();
    map.reset();
    //    map = boost::static_pointer_cast<neuro::Map>(boost::shared_ptr<void>() );
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(healthBar);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(suspicionBar);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(heroBox);
    _panel->hide();
    _panel2->hide();
    _panel3->hide();
    //    _panel4->hide();
    _overlay->hide();
    _overlay2->hide();
    _overlay3->hide();
    //    _overlay4->hide();
    */
}

void Terminal::updateTerminal(int time)
{
    /*
    if(map->getGameState()==neuro::InProgress) {
        map->getHero()->damageHealth(0.000018);
        map->updateAI();
        neuro::Vec2f pos = map->getHeroPos();
        heroBox->setPosition((pos.x*pixelSize),(pos.y*pixelSize));
        map->getHero()->updateProjectiles();
        switch(map->getHero()->getFacing()) {
        case neuro::UP:
            heroBox->setParameter("uv_coords","0.75 0 0.84 0.12");
            break;
        case neuro::DOWN:
            heroBox->setParameter("uv_coords","0.75 0.375 0.84 0.5");
            break;
        case neuro::LEFT:
            heroBox->setParameter("uv_coords","0.75 0.125 0.84 0.245");
            break;
        case neuro::RIGHT:
            heroBox->setParameter("uv_coords","0.75 0.25 0.84 0.370");
            break;
        }

        //    heatlthBar.
        roomScrollSwitch = false;
        //Check Rooms for reveal
        for(int x=0;x<roomsX;x++) {
            for(int y=0;y<roomsY;y++) {
                int roomID = map->getRoom(x,y)->getID();
                if(!map->getRoom(x,y)->isGone()) {
                    Ogre::OverlayElement* room = _overlayMgr->getOverlayElement("room"+boost::lexical_cast<std::string>(roomID));
                    Ogre::OverlayElement* unroom = _overlayMgr->getOverlayElement("unroom"+boost::lexical_cast<std::string>(roomID));
                    if(map->getRoom(x,y)->getStatus()==neuro::OCCUPIED) {
                        if(!(map->getRoom(x,y)->getEnd())) {
                            room->show();
                        }
                        unroom->hide();
                        occupiedRoom = room;
                        scrollLeft=true;
                        scrollRight=true;
                        scrollUp=true;
                        scrollDown=true;
                        roomScrollSwitch = true;
                    } else if(map->getRoom(x,y)->getStatus()==neuro::EXPLORED) {
                        room->hide();
                        unroom->show();
                    } else {
                        //                    room->hide();
                        //                    unroom->hide();
                    }
                }
            }
        }

        suspicionBar->setWidth(0.025 + (map->getHero()->getSuspicion()*0.4275));

        //Scroll Map
        if(roomScrollSwitch) {
            isScrollRoom = true;
        } else {
            isScrollRoom = false;
        }

        if(isScrollRoom) {
            scrollRoom();
        } else {
            scrollPlayer();
        }
        map->checkGameStatus();
    }
    */
}

void Terminal::scrollPlayer()
{
    if(!mapView) {
        if(heroBox->getLeft() < (_panel->getLeft()*-1)-(_overlayMgr->getViewportWidth()*0.025)) {
            scrollLeft = true;
        }
        if(heroBox->getLeft() > (_panel->getLeft()*-1)+(_overlayMgr->getViewportWidth()*0.025)) {
            scrollRight = true;
        }
        if(heroBox->getTop() < (_panel->getTop()*-1)-(_overlayMgr->getViewportHeight()*0.025)) {
            scrollUp = true;
        }
        if(heroBox->getTop() > (_panel->getTop()*-1)+(_overlayMgr->getViewportHeight()*0.025)) {
            scrollDown = true;
        }
        if(scrollLeft) {
            if(heroBox->getLeft() < (_panel->getLeft()*-1)) {
                _panel->setPosition(_panel->getLeft()+6,_panel->getTop());
                _panel2->setPosition(_panel2->getLeft()+6,_panel2->getTop());
                _panel3->setPosition(_panel3->getLeft()+6,_panel3->getTop());

            } else {
                scrollLeft = false;
            }
        }
        if(scrollRight) {
            if(heroBox->getLeft() > (_panel->getLeft()*-1)+(_overlayMgr->getViewportWidth()*0.025)) {
                _panel->setPosition(_panel->getLeft()-6,_panel->getTop());
                _panel2->setPosition(_panel2->getLeft()-6,_panel2->getTop());
                _panel3->setPosition(_panel3->getLeft()-6,_panel3->getTop());

            } else {
                scrollRight = false;
            }
        }
        if(scrollUp) {
            if(heroBox->getTop() < (_panel->getTop()*-1)-(_overlayMgr->getViewportHeight()*0.025)) {
                _panel->setPosition(_panel->getLeft(),_panel->getTop()+6);
                _panel2->setPosition(_panel2->getLeft(),_panel2->getTop()+6);
                _panel3->setPosition(_panel3->getLeft(),_panel3->getTop()+6);

            } else {
                scrollUp = false;
            }
        }
        if(scrollDown) {
            if(heroBox->getTop() > (_panel->getTop()*-1)+(_overlayMgr->getViewportHeight()*0.025)) {
                _panel->setPosition(_panel->getLeft(),_panel->getTop()-6);
                _panel2->setPosition(_panel2->getLeft(),_panel2->getTop()-6);
                _panel3->setPosition(_panel3->getLeft(),_panel3->getTop()-6);
            } else {
                scrollDown = false;
            }
        }
    }
}

void Terminal::scrollRoom()
{
    if(!mapView) {
        if(scrollLeft) {
            if(occupiedRoom->getLeft()+(occupiedRoom->getWidth()*0.5) < (_panel->getLeft()*-1)) {
                _panel->setPosition(_panel->getLeft()+6,_panel->getTop());
                _panel2->setPosition(_panel2->getLeft()+6,_panel2->getTop());
                _panel3->setPosition(_panel3->getLeft()+6,_panel3->getTop());
            } else {
                scrollLeft = false;
            }
        }
        if(scrollRight) {
            if(occupiedRoom->getLeft()+(occupiedRoom->getWidth()*0.5) > (_panel->getLeft()*-1)) {
                _panel->setPosition(_panel->getLeft()-6,_panel->getTop());
                _panel2->setPosition(_panel2->getLeft()-6,_panel2->getTop());
                _panel3->setPosition(_panel3->getLeft()-6,_panel3->getTop());
            } else {
                scrollRight = false;
            }
        }
        if(scrollUp) {
            if(occupiedRoom->getTop()+(occupiedRoom->getHeight()*0.5) < (_panel->getTop()*-1)) {
                _panel->setPosition(_panel->getLeft(),_panel->getTop()+6);
                _panel2->setPosition(_panel2->getLeft(),_panel2->getTop()+6);
                _panel3->setPosition(_panel3->getLeft(),_panel3->getTop()+6);
            } else {
                scrollUp = false;
            }
        }
        if(scrollDown) {
            if(occupiedRoom->getTop()+(occupiedRoom->getHeight()*0.5) > (_panel->getTop()*-1)) {
                _panel->setPosition(_panel->getLeft(),_panel->getTop()-6);
                _panel2->setPosition(_panel2->getLeft(),_panel2->getTop()-6);
                _panel3->setPosition(_panel3->getLeft(),_panel3->getTop()-6);
            } else {
                scrollDown = false;
            }
        }
    }
}

const std::string& Terminal::getText(const std::string& ID)
{
    Ogre::OverlayElement* textBox = _overlayMgr->getOverlayElement(ID);
    return textBox->getCaption();
}

void Terminal::moveHero(int ch)
{
    map->moveHero(ch);
}

void Terminal::setMapView(bool mapView)
{
    this->mapView = mapView;
    //Press M to bring up "MapView" overview of whole map
    if(mapView) {
        //Layer 1
        _panel->setPosition(0,0);
        _overlay->setScroll(-0.65,0.75);
        _overlay->setScale(0.2,0.2);

        //Layer 2
        _panel2->setPosition(0,0);
        _overlay2->setScroll(-0.65,0.75);
        _overlay2->setScale(0.2,0.2);

        //Layer 3
        _panel3->setPosition(0,0);
        _overlay3->setScroll(-0.65,0.75);
        _overlay3->setScale(0.2,0.2);
    } else {
        neuro::Vec2f pos = map->getHeroPos();
        //Layer 1
        _overlay->setScroll(1.0,-1.0);
        _overlay->setScale(scale,scale);

        //Layer 2
        _overlay2->setScroll(1.0,-1.0);
        _overlay2->setScale(scale,scale);

        //Layer 3
        _overlay3->setScroll(1.0,-1.0);
        _overlay3->setScale(scale,scale);
        if(true) {
            _panel->setPosition(-pixelSize*pos.x,-pixelSize*pos.y);
            _panel2->setPosition(-pixelSize*pos.x,-pixelSize*pos.y);
            _panel3->setPosition(-pixelSize*pos.x,-pixelSize*pos.y);
        } else {
            _panel->setPosition(occupiedRoom->getLeft()+(occupiedRoom->getWidth()*0.5),occupiedRoom->getTop()+(occupiedRoom->getHeight()*0.5));
            _panel2->setPosition(occupiedRoom->getLeft()+(occupiedRoom->getWidth()*0.5),occupiedRoom->getTop()+(occupiedRoom->getHeight()*0.5));
            _panel3->setPosition(occupiedRoom->getLeft()+(occupiedRoom->getWidth()*0.5),occupiedRoom->getTop()+(occupiedRoom->getHeight()*0.5));
        }
    }
}

void Terminal::button1()
{
    map->initProgram1();
}

void Terminal::button2()
{
    map->initProgram2();
}

void Terminal::button3()
{
    map->initProgram3();
}

void Terminal::button4()
{
    map->initProgram4();
}

void Terminal::preViewportUpdate(const RenderTargetViewportEvent &evt)
{
    _overlay->show();
    _overlay2->show();
    _overlay3->show();
    _overlay3->show();
    _panel->show();
    _panel2->show();
    _panel3->show();
}

void Terminal::postViewportUpdate(const RenderTargetViewportEvent &evt)
{
    _overlay->hide();
    _overlay2->hide();
    _overlay3->hide();
    _overlay3->hide();
    _panel->hide();
    _panel2->hide();
    _panel3->hide();
}

void Terminal::preRenderTargetUpdate(const RenderTargetEvent &evt)
{
//    _overlay->show();
//    _overlay2->show();
//    _overlay3->show();
//    _overlay3->show();
//    _panel->show();
//    _panel2->show();
//    _panel3->show();
}

void Terminal::postRenderTargetUpdate(const RenderTargetEvent &evt)
{
    _overlay->hide();
    _overlay2->hide();
    _overlay3->hide();
    _overlay3->hide();
    _panel->hide();
    _panel2->hide();
    _panel3->hide();
}

Ogre::SceneNode* Terminal::createTexturedRect(std::string object_name, std::string texture_name, float left, float top, float right, float bottom)
{
    MaterialPtr material = MaterialManager::getSingleton().create(object_name,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->getTechnique(0)->getPass(0)->createTextureUnitState(texture_name);
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(true);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

//    Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
    ManualObject* manual = Entropy::getSingletonPtr()->mSceneMgr->createManualObject(object_name);
    manual->setUseIdentityProjection(true);
    manual->setUseIdentityView(true);

    manual->begin(object_name, RenderOperation::OT_TRIANGLE_STRIP);

    manual->position(left, bottom, 0.0);
    manual->position(left, top, 0.0);
    manual->position(right, bottom, 0.0);
    manual->position(right,  top, 0.0);

    manual->index(0);
    manual->index(1);
    manual->index(2);
    manual->index(3);

    manual->end();

//    rect->setCorners(left,top,right,bottom);
//    rect->setMaterial(object_name);

    manual->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);

    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    manual->setBoundingBox(aabInf);

    Ogre::SceneNode* rect_node = Entropy::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(object_name);
    rect_node->attachObject(manual);


//    rect->setVisible(false);



//    rect_node->setPosition(0,0,0);

    return rect_node;
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> Terminal* Ogre::Singleton<Terminal>::ms_Singleton = 0;
Terminal* Terminal::getSingletonPtr(void)
{
    return ms_Singleton;
}
Terminal& Terminal::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> Terminal* Ogre::Singleton<Terminal>::msSingleton = 0;
Terminal* Terminal::getSingletonPtr(void)
{
    return msSingleton;
}
Terminal& Terminal::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif

