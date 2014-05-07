#include "port.h"
#include "src/Core/terminal.h"
#include <boost/lexical_cast.hpp>
#include "src/Core/Entropy.h"
#include "OGRE/Ogre.h"
#include "room.h"

namespace neuro {

Port::Port() :
    portFlags(Closed | EnemyOwned)
{
    setup = false;
    setupGUI();
}

Port::Port(Vec2i pos, int portFlags) :
    portFlags(portFlags)
{
    setup = false;
    this->x = pos.x;
    this->y = pos.y;
    c = ENEMYPORT;
    setupGUI();
}

Port::~Port()
{
    setup = false;
}

void Port::deleteResources()
{
    setup = false;
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(portIcon);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(daemonIcon);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(alarmIcon);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(trojanIcon);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(blackICEIcon);
}

//===================
//GUI STUFF
//===================

void Port::setupGUI()
{
    Ogre::MaterialPtr ePortMat = Ogre::MaterialManager::getSingleton().create("EnemyPort", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    ePortMat->getTechnique(0)->getPass(0)->createTextureUnitState("terrain.png");
    ePortMat->getTechnique(0)->getPass(0)->createTextureUnitState("nm_dn.png");
    ePortMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    ePortMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    ePortMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    Ogre::MaterialPtr dportMat = Ogre::MaterialManager::getSingleton().create("DPort", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    dportMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/virus_icon.png");
    dportMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    dportMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    dportMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    dportMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr edportMat = Ogre::MaterialManager::getSingleton().create("EDPort", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    edportMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/Evirus_icon.png");
    edportMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    edportMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    edportMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    edportMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);


    Ogre::MaterialPtr portMat = Ogre::MaterialManager::getSingleton().create("Port", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    portMat->getTechnique(0)->getPass(0)->createTextureUnitState("terrain_detail.jpg");
    portMat->getTechnique(0)->getPass(0)->createTextureUnitState("nm_up.png");
    portMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    portMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    portMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    Ogre::MaterialPtr alarmMat = Ogre::MaterialManager::getSingleton().create("AlarmIcon", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    alarmMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/AlarmIcon.png");
    alarmMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    alarmMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    alarmMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    alarmMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr triojanMat = Ogre::MaterialManager::getSingleton().create("TrojanIcon", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    triojanMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/TrojanIcon.png");
    triojanMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    triojanMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    triojanMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    triojanMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr blackIceMat = Ogre::MaterialManager::getSingleton().create("BlackICEIcon", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    blackIceMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/BlackIceIcon.png");
    blackIceMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    blackIceMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    blackIceMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    blackIceMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    this->id = squareIDs++;
    portIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "port"+boost::lexical_cast<std::string>(this->id )));
    portIcon->setMetricsMode(Ogre::GMM_PIXELS);
    portIcon->setPosition(x*Terminal::getSingletonPtr()->pixelSize, y*Terminal::getSingletonPtr()->pixelSize);
    portIcon->setWidth(Terminal::getSingletonPtr()->pixelSize);
    portIcon->setHeight(Terminal::getSingletonPtr()->pixelSize);
    portIcon->setParameter("transparent","false");
    portIcon->setParameter("uv_coords","0 0 1 1");
    portIcon->setMaterialName("DPort");
    portIcon->setMaterialName("EDPort");
    if(portFlags & EnemyOwned) {
        portIcon->setMaterialName("EnemyPort");
    } else {
        portIcon->setMaterialName("Port");
    }
    Terminal::getSingletonPtr()->_panel->addChild(portIcon);

    daemonIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "daemon"+boost::lexical_cast<std::string>(this->id )));
    daemonIcon->setMetricsMode(Ogre::GMM_PIXELS);
    daemonIcon->setPosition(x*Terminal::getSingletonPtr()->pixelSize, y*Terminal::getSingletonPtr()->pixelSize);
    daemonIcon->setWidth(Terminal::getSingletonPtr()->pixelSize);
    daemonIcon->setHeight(Terminal::getSingletonPtr()->pixelSize);
    daemonIcon->setParameter("transparent","false");
    daemonIcon->setParameter("uv_coords","0 0 1 1");
    daemonIcon->setMaterialName("DPort");
    daemonIcon->setMaterialName("EDPort");
    Terminal::getSingletonPtr()->_panel2->addChild(daemonIcon);
    daemonIcon->hide();

    alarmIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "alarm"+boost::lexical_cast<std::string>(this->id )));
    alarmIcon->setMetricsMode(Ogre::GMM_PIXELS);
    alarmIcon->setPosition(x*Terminal::getSingletonPtr()->pixelSize, y*Terminal::getSingletonPtr()->pixelSize+(Terminal::getSingletonPtr()->pixelSize*0.666));
    alarmIcon->setWidth(Terminal::getSingletonPtr()->pixelSize*0.333);
    alarmIcon->setHeight(Terminal::getSingletonPtr()->pixelSize*0.333);
    alarmIcon->setParameter("transparent","false");
    alarmIcon->setParameter("uv_coords","0 0 1 1");
    alarmIcon->setMaterialName("AlarmIcon");
    Terminal::getSingletonPtr()->_panel3->addChild(alarmIcon);
    alarmIcon->hide();

    trojanIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "trojan"+boost::lexical_cast<std::string>(this->id )));
    trojanIcon->setMetricsMode(Ogre::GMM_PIXELS);
    trojanIcon->setPosition(x*Terminal::getSingletonPtr()->pixelSize +(Terminal::getSingletonPtr()->pixelSize*0.666) , y*Terminal::getSingletonPtr()->pixelSize+(Terminal::getSingletonPtr()->pixelSize*0.666) );
    trojanIcon->setWidth(Terminal::getSingletonPtr()->pixelSize*0.333);
    trojanIcon->setHeight(Terminal::getSingletonPtr()->pixelSize*0.333);
    trojanIcon->setParameter("transparent","false");
    trojanIcon->setParameter("uv_coords","0 0 1 1");
    trojanIcon->setMaterialName("TrojanIcon");
    Terminal::getSingletonPtr()->_panel3->addChild(trojanIcon);
    trojanIcon->hide();

    blackICEIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "blackICEIcon"+boost::lexical_cast<std::string>(this->id )));
    blackICEIcon->setMetricsMode(Ogre::GMM_PIXELS);
    blackICEIcon->setPosition(x*Terminal::getSingletonPtr()->pixelSize, y*Terminal::getSingletonPtr()->pixelSize);
    blackICEIcon->setWidth(Terminal::getSingletonPtr()->pixelSize);
    blackICEIcon->setHeight(Terminal::getSingletonPtr()->pixelSize);
    blackICEIcon->setParameter("transparent","false");
    blackICEIcon->setParameter("uv_coords","0 0 1 1");
    blackICEIcon->setMaterialName("BlackICEIcon");
    Terminal::getSingletonPtr()->_panel2->addChild(blackICEIcon);
    blackICEIcon->hide();
    setup = true;
}

void Port::updateGUI()
{
    if(setup) {
        if(portFlags & EnemyOwned) {
            portIcon->setMaterialName("EnemyPort");
        } else {
            portIcon->setMaterialName("Port");
        }
    }
}


//===================
//Bit Field Stuff
//===================
void Port::removePortFlag(PortFlags flag)
{

    portFlags &= ~flag;
    if(setup) {
        updateGUI();
    }
}

void Port::addPortFlag(PortFlags flag)
{
    portFlags |= flag;
    if(setup) {
        updateGUI();
    }
}

void Port::setPortFlags(int portFlags)
{
    this->portFlags = portFlags;
    if(portFlags & EnemyOwned) {
        c = ENEMYPORT;
    } else {
        c = PORT;
    }
    if(setup) {
        updateGUI();
    }
}

bool Port::containsFlag(PortFlags flag)
{
    if(portFlags & flag) {
        return true;
    } else {
        return false;
    }
}

void Port::portToEnemyPort()
{
    c = ENEMYPORT;
    removePortFlag(UserOwned);
    removePortFlag(Daemoned);
    removePortFlag(Alarmed);
    removePortFlag(Trojaned);
    removePortFlag(BlackICED);
    addPortFlag(EnemyOwned);
    if(setup) {
        updateGUI();
        daemonIcon->hide();
        alarmIcon->hide();
        trojanIcon->hide();
    }
}

void Port::enemyPortToPort()
{
    c = PORT;
    removePortFlag(EnemyOwned);
    addPortFlag(UserOwned);
    if(setup) {
        updateGUI();
        daemonIcon->hide();
        alarmIcon->hide();
    }
}

void Port::activatePrograms()
{
    if(setup) {
        if(portFlags & EnemyOwned) {
            if(containsFlag(Daemoned)) {
                daemonIcon->setMaterialName("EDPort");
                daemonIcon->show();
            }
            if(containsFlag(Alarmed)) {
                alarmIcon->show();
            }
        } else {
            if(containsFlag(Alarmed)) {
                alarmIcon->show();
            }
            if(containsFlag(Trojaned)) {
                trojanIcon->show();
            }
            if(containsFlag(BlackICED)) {
                daemonIcon->hide();
                blackICEIcon->show();
                trojanIcon->hide();
                alarmIcon->hide();
            } else if(containsFlag(Daemoned)) {
                daemonIcon->setMaterialName("DPort");
                daemonIcon->show();
            }
        }
    }
}

void Port::deactivateAlarm()
{
    alarmIcon->hide();
}

int Port::getPortFlags()
{
    return portFlags;
}

void Port::setParent(boost::shared_ptr<Room> parent)
{
    this->parent = parent;
}

boost::shared_ptr<Room> Port::getParent()
{
    return parent;
}

void Port::explodeDaemon()
{
    removePortFlag(Daemoned);
    if(setup) {
        daemonIcon->hide();
    }
}

}
