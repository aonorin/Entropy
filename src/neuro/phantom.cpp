#include "phantom.h"
#include "src/Core/terminal.h"
#include <boost/lexical_cast.hpp>
#include "src/Core/update.h"
#include "neuro/port.h"
#include "boost/pointer_cast.hpp"
#include "src/Utility/WeakFunc.h"
#include "src/Core/Entropy.h"
#include "neuro/astar.h"
#include "neuro/hero.h"
#include "neuro/backtracer.h"

#include "neuro/map.h"
#include "neuro/program.h"
#include "neuro/node.h"
#include "neuro/room.h"
#include "OGRE/OgreOverlayElement.h"
#include "OGRE/OgreOverlayManager.h"
#include "OGRE/OgreOverlayContainer.h"
#include "OGRE/OgreMaterialManager.h"

namespace neuro {

Phantom::Phantom(Vec2f pos,int id, int status, int metaStatus, Map *map)
{
    //    program = 0;
    this->pos = pos;
    nodePos = Vec2i(0,0);
    nodePos.x = pos.x;
    nodePos.y = pos.y-1;
    this->iconName = iconName;
    this->health = 1.0;
    this->enemy = enemy;
    this->id = id;
    this->status = status;
    this->metaStatus = metaStatus;
    this->map = map;
    iconName = "PhantomIcon";
    speed = 0.015;
    scanPos.x = pos.x;
    scanPos.y = pos.y+1;
    scanTimer = 0;
    scanNum = 0;
    scanRate = 6;
    guiSetup = false;
    targetPos = Vec2i(0,0);
    boost::function<void (void)> func = boost::bind(&Phantom::setupPhantomGUI,this);
    Entropy::getSingletonPtr()->addQeueudFunction(func);
}

void Phantom::setupPhantomGUI()
{
    setupGUI();

    Ogre::MaterialPtr scanMat = Ogre::MaterialManager::getSingleton().create("ScanIcon", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    scanMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/ScanIcon.png");
    scanMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    scanMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    scanMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    scanMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_DEST_COLOUR);

    scanIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "ScanIcon"+boost::lexical_cast<std::string>(this->id )));
    scanIcon->setMetricsMode(Ogre::GMM_PIXELS);
    scanIcon->setPosition(scanPos.x*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25, scanPos.y*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25);
    scanIcon->setWidth(Terminal::getSingletonPtr()->pixelSize*0.5);
    scanIcon->setHeight(Terminal::getSingletonPtr()->pixelSize*0.5);
    scanIcon->setParameter("uv_coords","0 0 1 1");
    scanIcon->setMaterialName("ScanIcon");
    //    Terminal::getSingletonPtra()->_panel2->addChild(scanIcon);

    //    targetIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "TargetIcon"+boost::lexical_cast<std::string>(this->id )));
    //    targetIcon->setMetricsMode(Ogre::GMM_PIXELS);
    //    targetIcon->setPosition(targetPos.x*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25, targetPos.y*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25);
    //    targetIcon->setWidth(Terminal::getSingletonPtr()->pixelSize*0.5);
    //    targetIcon->setHeight(Terminal::getSingletonPtr()->pixelSize*0.5);
    //    targetIcon->setParameter("uv_coords","0 0 1 1");
    //    targetIcon->setMaterialName("ScanIcon");
    //    Terminal::getSingletonPtr()->_panel2->addChild(targetIcon);
    guiSetup = true;
}

Phantom::~Phantom()
{
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(scanIcon);
}

void Phantom::attack()
{

}

void Phantom::updateProjectiles()
{

}

void Phantom::update()
{
    if(guiSetup) {
        //        if(prevPoint.x==map->getAStar()->getPos(targetID).x&&prevPoint.y==map->getAStar()->getPos(targetID).x) {
        if(map->getAStar()->closedOnTarget(this->id,targetID)) {
            if(targetID!=map->getHero()->id) {
                status = AtDestination;
            }
        }
        if(metaStatus == Patrolling) {
            speed = 0.0125;
            if(status == HackingPort) {
                if( boost::static_pointer_cast<Port>(map->getSquare(program->getPos().x,program->getPos().y))->containsFlag(Port::EnemyOwned)) {
                    status = Walking;
                    program.reset();
                    move();
                }
            } else if(status == HackingNode) {
                if(! boost::static_pointer_cast<Node>(map->getSquare(program->getPos().x,program->getPos().y))->containsFlag(Node::UserOwned)) {
                    status = Walking;
                    program.reset();
                    move();
                }
            } else {
                chooseTarget();
                if(status == Walking) {
                    targetPos = map->getAStar()->getPos(targetID);
                    //                    targetIcon->setPosition(targetPos.x*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25, targetPos.y*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25);
                    //                    scanIcon->setPosition(nextPoint.x*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25, nextPoint.y*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25);
                    if(abs(nextPoint.x-pos.x)>1&&abs(nextPoint.y-pos.y)>1){
                        status = Idle;
                        chooseTarget();
                        updateR = 1.1;
                    } else {
                        scan();
                        move();
                    }
                }
            }
        } else if(metaStatus == Alarmed) {
            speed = 0.03;
            targetID = map->getHero()->id;
            if(status == Walking) {
                scan();
                move();
                if(abs(nextPoint.x-pos.x)>1&&abs(nextPoint.y-pos.y)>1){
                    //                    status = Idle;
                    //                    chooseTarget();
                    updateR = 1.1;
                }
            } else if(status == HackingPort) {
                if( boost::static_pointer_cast<Port>(map->getSquare(program->getPos().x,program->getPos().y))->containsFlag(Port::EnemyOwned)) {
                    status = Walking;
                    program.reset();
                    move();
                }
            } else if(status == HackingNode) {
                if(! boost::static_pointer_cast<Node>(map->getSquare(program->getPos().x,program->getPos().y))->containsFlag(Node::UserOwned)) {
                    status = Walking;
                    program.reset();
                    move();
                }
            }
        }
    }
}

void Phantom::chooseTarget()
{
    if(status == Idle || status == AtDestination) {
        bool chosen = false;
        int rx,ry;
        while(!chosen) {
            rx = nrand(0,map->getGridSizeX()-1)%map->getGridSizeX();
            ry = nrand(0,map->getGridSizeY()-1)%map->getGridSizeY();
            if(!map->honeyPotActive) {
                if(!map->getRoom(rx,ry)->isGone()) {
                    if(
                            map->getSquare(map->getRoom(rx,ry)->getMidX(),map->getRoom(rx,ry)->getMidY())->getChar()==HALLWAY ||
                            map->getSquare(map->getRoom(rx,ry)->getMidX(),map->getRoom(rx,ry)->getMidY())->getChar()==FLOOR ||
                            map->getSquare(map->getRoom(rx,ry)->getMidX(),map->getRoom(rx,ry)->getMidY())->getChar()==ENEMYNODE ||
                            map->getSquare(map->getRoom(rx,ry)->getMidX(),map->getRoom(rx,ry)->getMidY())->getChar()==NODE
                            ) {
                        chosen = true;
                        targetID = map->getRoom(rx,ry)->getID();
                        status = Walking;
                        break;
                    }
                }
            } else {
                if(!map->getRoom(rx,ry)->isGone()) {
                    if(map->getRoom(rx,ry)->getNode()->containsFlag(Node::HoneyPot)) {
                        chosen = true;
                        targetID = map->getRoom(rx,ry)->getID();
                        status = Walking;
                        break;
                    }
                }
            }
        }
    }
}

void Phantom::scan()
{
    /*
    4   5   6   7   8
        1   2   3
            0
      */
    scanTimer++;
    if(scanTimer > scanRate) {
        scanNum++;
        if(scanNum>3) {
            scanNum = 0;
        }
        scanTimer = 0;
    }

    switch(scanNum) {
    case 0:
        scanPos.x = pos.x;
        scanPos.y = pos.y;
        if(facing==UP)scanPos.y+=1;
        if(facing==DOWN)scanPos.y-=1;
        if(facing==LEFT)scanPos.x-=1;
        if(facing==RIGHT)scanPos.x+=1;
        break;
    case 1:
        scanPos.x = pos.x;
        scanPos.y = pos.y;
        if(facing==UP) {scanPos.x-=1;scanPos.y+=2;}
        if(facing==DOWN) {scanPos.x+=1;scanPos.y-=2;}
        if(facing==LEFT) {scanPos.x-=2;scanPos.y-=1;}
        if(facing==RIGHT) {scanPos.x+=2;scanPos.y+=1;}
        break;
    case 2:
        scanPos.x = pos.x;
        scanPos.y = pos.y;
        if(facing==UP) {scanPos.y+=2;}
        if(facing==DOWN) {scanPos.y-=2;}
        if(facing==LEFT) {scanPos.x-=2;}
        if(facing==RIGHT) {scanPos.x+=2;}
        break;
    case 3:
        scanPos.x = pos.x;
        scanPos.y = pos.y;
        if(facing==UP) {scanPos.x+=1;scanPos.y+=2;}
        if(facing==DOWN) {scanPos.x-=1;scanPos.y-=2;}
        if(facing==LEFT) {scanPos.x-=2;scanPos.y+=1;}
        if(facing==RIGHT) {scanPos.x+=2;scanPos.y-=1;}
        break;
    case 4:
        scanPos.x = pos.x;
        scanPos.y = pos.y;
        if(facing==UP) {scanPos.x-=2;scanPos.y+=3;}
        if(facing==DOWN) {scanPos.x+=2;scanPos.y-=3;}
        if(facing==LEFT) {scanPos.x-=3;scanPos.y-=2;}
        if(facing==RIGHT) {scanPos.x+=3;scanPos.y+=2;}
        break;
    case 5:
        scanPos.x = pos.x;
        scanPos.y = pos.y;
        if(facing==UP) {scanPos.x-=1;scanPos.y+=3;}
        if(facing==DOWN) {scanPos.x+=1;scanPos.y-=3;}
        if(facing==LEFT) {scanPos.x-=3;scanPos.y-=1;}
        if(facing==RIGHT) {scanPos.x+=3;scanPos.y+=1;}
        break;
    case 6:
        scanPos.x = pos.x;
        scanPos.y = pos.y;
        if(facing==UP) {scanPos.y+=3;}
        if(facing==DOWN) {scanPos.y-=3;}
        if(facing==LEFT) {scanPos.x-=3;}
        if(facing==RIGHT) {scanPos.x+=3;}
        break;
    case 7:
        scanPos.x = pos.x;
        scanPos.y = pos.y;
        if(facing==UP) {scanPos.x+=2;scanPos.y+=3;}
        if(facing==DOWN) {scanPos.x-=2;scanPos.y-=3;}
        if(facing==LEFT) {scanPos.x-=3;scanPos.y+=2;}
        if(facing==RIGHT) {scanPos.x+=3;scanPos.y-=2;}
        break;
    case 8:
        scanPos.x = pos.x;
        scanPos.y = pos.y;
        if(facing==UP) {scanPos.x+=1;scanPos.y+=3;}
        if(facing==DOWN) {scanPos.x-=1;scanPos.y-=3;}
        if(facing==LEFT) {scanPos.x-=3;scanPos.y+=1;}
        if(facing==RIGHT) {scanPos.x+=3;scanPos.y-=1;}
        break;
    }
    if(facing==DOWN)scanPos.y+=1;
    if(facing==LEFT)scanPos.x+=1;

    //    scanIcon->setPosition(scanPos.x*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25, scanPos.y*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25);

    if(map->getSquare(scanPos.x,scanPos.y)->getChar()==NODE) {
        boost::shared_ptr<Room> parent = boost::static_pointer_cast<Node>((map->getSquare(scanPos.x,scanPos.y)))->getParent();
        if(!(parent->getPort1()->containsFlag(Port::Trojaned)&&parent->getPort2()->containsFlag(Port::Trojaned))) {
            status = HackingNode;
            initProgram(Program::EnemyInfect);
        }
    } else if(map->getHero()->getX() == scanPos.x && map->getHero()->getY()==scanPos.y) {
        if(!map->getHero()->isInvisible()) {
            map->getHero()->addSuspicion(0.25);
            metaStatus = Alarmed;
            status = Walking;
            scanRate = 3;
            targetID = map->getHero()->id;
            map->getHero()->damageHealth(0.01);
            //            std::cout << "HERO DETECTED" << std::endl;
        } else {
            metaStatus = Patrolling;
            //            std::cout << "HERO INVISIBLE" << std::endl;
            if(targetID==map->getHero()->id) {
                chooseTarget();
            }
        }
    } else if(map->getSquare(scanPos.x,scanPos.y)->getChar()==PORT) {
        boost::shared_ptr<Port> port = boost::static_pointer_cast<Port>(map->getSquare(scanPos.x,scanPos.y));
        if(port->containsFlag(Port::Alarmed)) {
            map->getHero()->addSuspicion(-0.001);
        }
        if(port->containsFlag(Port::BlackICED)) {
            map->deletePhantom(this->id);
        } else if(port->containsFlag(Port::Daemoned)) {
            port->explodeDaemon();
            map->deletePhantom(this->id);
        } else if(!port->containsFlag(Port::Trojaned)) {
            status = HackingPort;
            initProgram(Program::EnemyCrack);
        }
    }
}

void Phantom::initProgram(int programType){
    program =  boost::shared_ptr<Program>(new Program(programType,scanPos,Program::getProgramID()));

    boost::function<void (int)> func = boost::bind(boost::weak_fn(&Phantom::endProgram,shared_from_this()),_1);
    boost::function<void (float)> update = boost::bind(boost::weak_fn(&Program::update,program),_1);
    int eid = Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(program->getTime()),func,update,programType);
    program->setTimedEventID(eid);
}

void Phantom::endProgram(int programType)
{
    try{
        if(program!=0) {
            boost::shared_ptr<Port> port;
            boost::shared_ptr<Node> node;
            if(programType==Program::EnemyCrack) {
                port = boost::static_pointer_cast<Port>(map->getSquare(program->getPos().x,program->getPos().y));
            } else if(programType==Program::EnemyInfect) {
                node = boost::static_pointer_cast<Node>(map->getSquare(program->getPos().x,program->getPos().y));
            }
            switch(programType) {
            case Program::EnemyCrack:
                port->portToEnemyPort();
                status = Walking;
                break;
            case Program::EnemyInfect:
                status = Walking;
                node->enemyInfect();
                if(node->getParent()->getEnd()) {
                    map->getBackTracer()->reactivate();
                }
                break;
            }
            //            delete program;
            program.reset();
            if(targetID==map->getHero()->id) {
                metaStatus = Alarmed;
                status = Walking;
            } else {
                metaStatus = Patrolling;
            }
        }
    } catch(...) {
        //        std::cout << "End Program error" << std::endl;
    }
}

void Phantom::setTargetID(int targetID)
{
    this->targetID = targetID;
    status = Walking;
}

void Phantom::setNodePos(Vec2i nodePos)
{
    this->nodePos = nodePos;
}

Vec2i Phantom::getNodePos()
{
    return nodePos;
}

}
