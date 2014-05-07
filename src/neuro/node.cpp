#include "node.h"
#include "src/Core/terminal.h"
#include <boost/lexical_cast.hpp>
#include "src/Core/Entropy.h"
#include <OGRE/Ogre.h>
#include "neuro/room.h"
#include "neuro/program.h"

namespace neuro {
Node::Node(Vec2i pos, int nodeFlags) :
    nodeFlags(nodeFlags)
{
    this->x = pos.x;
    this->y = pos.y;
    this->pos = pos;
    this->c = ENEMYNODE;
    setupGUI();
}

Node::~Node()
{

}

void Node::deleteResources()
{
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(nodeIcon);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(lockIcon);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(infectionIcon);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(crashIcon);
    program.reset();
}

void Node::setupGUI()
{
    //Captued Icons
    Ogre::MaterialPtr ioMat = Ogre::MaterialManager::getSingleton().create("IoNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    ioMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/IOIcon.png");
    ioMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    ioMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    ioMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    ioMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr scannerMat = Ogre::MaterialManager::getSingleton().create("ScannerNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    scannerMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/ScannerIcon.png");
    scannerMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    scannerMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    scannerMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    scannerMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr dataMat = Ogre::MaterialManager::getSingleton().create("DataNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    dataMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/DataIcon.png");
    dataMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    dataMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    dataMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    dataMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr controlMat = Ogre::MaterialManager::getSingleton().create("ControlNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    controlMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/ControlIcon.png");
    controlMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    controlMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    controlMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    controlMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr spawnerMat = Ogre::MaterialManager::getSingleton().create("SpawnerNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    spawnerMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/SpawnerIcon.jpg");
    spawnerMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    spawnerMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    spawnerMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    spawnerMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr wetwareMat = Ogre::MaterialManager::getSingleton().create("WetwareNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    wetwareMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/WetwareIcon.png");
    wetwareMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    wetwareMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    wetwareMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    wetwareMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //Enemy Icons

    Ogre::MaterialPtr escannerMat = Ogre::MaterialManager::getSingleton().create("EScannerNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    escannerMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/EScannerIcon.png");
    escannerMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    escannerMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    escannerMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    escannerMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr edataMat = Ogre::MaterialManager::getSingleton().create("EDataNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    edataMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/EDataIcon.png");
    edataMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    edataMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    edataMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    edataMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr econtrolMat = Ogre::MaterialManager::getSingleton().create("EControlNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    econtrolMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/EControlIcon.png");
    econtrolMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    econtrolMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    econtrolMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    econtrolMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr espawnerMat = Ogre::MaterialManager::getSingleton().create("ESpawnerNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    espawnerMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/ESpawnerIcon.jpg");
    espawnerMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    espawnerMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    espawnerMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    espawnerMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr ewetwareMat = Ogre::MaterialManager::getSingleton().create("EWetwareNode", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    ewetwareMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/EWetwareIcon.png");
    ewetwareMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    ewetwareMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    ewetwareMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    ewetwareMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //Lock Icon
    Ogre::MaterialPtr lockMat = Ogre::MaterialManager::getSingleton().create("LockIcon", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    lockMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/LockIcon.png");
    lockMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    lockMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    lockMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    lockMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr infectMat = Ogre::MaterialManager::getSingleton().create("InfectionIcon", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    infectMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/InfectionIcon.png");
    infectMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    infectMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    infectMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    infectMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr crashMat = Ogre::MaterialManager::getSingleton().create("CrashIcon", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    crashMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/CrashIcon.png");
    crashMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    crashMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    crashMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    crashMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr honeyMat = Ogre::MaterialManager::getSingleton().create("HoneyPotIcon", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    honeyMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/HoneyPotIcon.png");
    honeyMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    honeyMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    honeyMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    honeyMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    this->id = squareIDs++;
    nodeIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "node"+boost::lexical_cast<std::string>(this->id )));
    nodeIcon->setMetricsMode(Ogre::GMM_PIXELS);
    nodeIcon->setPosition(x*Terminal::getSingletonPtr()->pixelSize, y*Terminal::getSingletonPtr()->pixelSize);
    nodeIcon->setWidth(Terminal::getSingletonPtr()->pixelSize);
    nodeIcon->setHeight(Terminal::getSingletonPtr()->pixelSize);
    nodeIcon->setParameter("uv_coords","0 0 1 1");

    nodeIcon->setMaterialName("HoneyPotIcon");
    nodeIcon->setMaterialName("IoNode");
    nodeIcon->setMaterialName("ScannerNode");
    nodeIcon->setMaterialName("DataNode");
    nodeIcon->setMaterialName("ControlNode");
    nodeIcon->setMaterialName("SpawnerNode");
    nodeIcon->setMaterialName("WetwareNode");
    nodeIcon->setMaterialName("EScannerNode");
    nodeIcon->setMaterialName("EDataNode");
    nodeIcon->setMaterialName("EControlNode");
    nodeIcon->setMaterialName("ESpawnerNode");
    nodeIcon->setMaterialName("EWetwareNode");

    if(nodeFlags & IO) {
        nodeIcon->setMaterialName("IoNode");
    } else if(nodeFlags & Scanner){
        nodeIcon->setMaterialName("EScannerNode");
    } else if(nodeFlags & Data){
        nodeIcon->setMaterialName("EDataNode");
    } else if(nodeFlags & Control){
        nodeIcon->setMaterialName("EControlNode");
    } else if(nodeFlags & Spawner){
        nodeIcon->setMaterialName("ESpawnerNode");
    } else if(nodeFlags & Wetware){
        nodeIcon->setMaterialName("EWetwareNode");
    }
    Terminal::getSingletonPtr()->_panel2->addChild(nodeIcon);
    if(nodeFlags & IO) {
        nodeIcon->show();
    } else {
        nodeIcon->hide();
    }

    lockIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "lock"+boost::lexical_cast<std::string>(this->id )));
    lockIcon->setMetricsMode(Ogre::GMM_PIXELS);
    lockIcon->setPosition(x*Terminal::getSingletonPtr()->pixelSize, y*Terminal::getSingletonPtr()->pixelSize+(Terminal::getSingletonPtr()->pixelSize*0.6));
    lockIcon->setWidth(Terminal::getSingletonPtr()->pixelSize*0.4);
    lockIcon->setHeight(Terminal::getSingletonPtr()->pixelSize*0.4);
    lockIcon->setParameter("uv_coords","0 0 1 1");
    lockIcon->setMaterialName("LockIcon");
    lockIcon->hide();
    Terminal::getSingletonPtr()->_panel3->addChild(lockIcon);

    infectionIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "infectionIcon"+boost::lexical_cast<std::string>(this->id )));
    infectionIcon->setMetricsMode(Ogre::GMM_PIXELS);
    infectionIcon->setPosition(x*Terminal::getSingletonPtr()->pixelSize+(Terminal::getSingletonPtr()->pixelSize*0.6), y*Terminal::getSingletonPtr()->pixelSize+(Terminal::getSingletonPtr()->pixelSize*0.6));
    infectionIcon->setWidth(Terminal::getSingletonPtr()->pixelSize*0.4);
    infectionIcon->setHeight(Terminal::getSingletonPtr()->pixelSize*0.4);
    infectionIcon->setParameter("uv_coords","0 0 1 1");
    infectionIcon->setMaterialName("InfectionIcon");
    infectionIcon->hide();
    Terminal::getSingletonPtr()->_panel3->addChild(infectionIcon);

    crashIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "crashIcon"+boost::lexical_cast<std::string>(this->id )));
    crashIcon->setMetricsMode(Ogre::GMM_PIXELS);
    crashIcon->setPosition(x*Terminal::getSingletonPtr()->pixelSize, y*Terminal::getSingletonPtr()->pixelSize);
    crashIcon->setWidth(Terminal::getSingletonPtr()->pixelSize);
    crashIcon->setHeight(Terminal::getSingletonPtr()->pixelSize);
    crashIcon->setParameter("uv_coords","0 0 1 1");
    crashIcon->setMaterialName("CrashIcon");
    crashIcon->hide();
    Terminal::getSingletonPtr()->_panel2->addChild(crashIcon);
}

void Node::updateGUI()
{

}

void Node::reveal()
{
    if(!containsFlag(Crashed)) {
        nodeIcon->show();
        if(containsFlag(Encrypted)) {
            lockIcon->show();
        }
    }
}

void Node::captured()
{
    if(nodeFlags & IO) {
        nodeIcon->setMaterialName("IoNode");
    } else if(nodeFlags & Scanner){
        nodeIcon->setMaterialName("ScannerNode");
    } else if(nodeFlags & Data){
        nodeIcon->setMaterialName("DataNode");
    } else if(nodeFlags & Control){
        nodeIcon->setMaterialName("ControlNode");
    } else if(nodeFlags & Spawner){
        nodeIcon->setMaterialName("SpawnerNode");
    } else if(nodeFlags & Wetware){
        nodeIcon->setMaterialName("WetwareNode");
    }
    if(containsFlag(Infected)) {
        infectionIcon->show();
    }
    stopRespawner();
    lockIcon->hide();
    removePortFlag(Encrypted);
    addPortFlag(UserOwned);
//    if(program!=0) {
//        delete program;
//        program = 0;
//    }
}

void Node::enemyInfect()
{
    if(nodeFlags & IO) {
        nodeIcon->setMaterialName("IoNode");
    } else if(nodeFlags & Scanner){
        nodeIcon->setMaterialName("EScannerNode");
    } else if(nodeFlags & Data){
        nodeIcon->setMaterialName("EDataNode");
    } else if(nodeFlags & Control){
        nodeIcon->setMaterialName("EControlNode");
    } else if(nodeFlags & Spawner){
        nodeIcon->setMaterialName("ESpawnerNode");
    } else if(nodeFlags & Wetware){
        nodeIcon->setMaterialName("EWetwareNode");
    }
    if(containsFlag(Infected)) {
        try{
            infectionIcon->hide();
        } catch(...) {

        }
    }

    addPortFlag(Encrypted);
//    addPortFlag(Infected);
    //    if(containsFlag(Encrypted)) {
            try{
                lockIcon->show();
            } catch(...) {

            }
    //    }

//    removePortFlag(Encrypted);
    removePortFlag(Infected);
    removePortFlag(UserOwned);
    removePortFlag(HoneyPot);
    c = ENEMYNODE;
}

void Node::encrypt()
{
    addPortFlag(Encrypted);
    lockIcon->show();
}

void Node::crash()
{
    this->c = BLANK;
    lockIcon->hide();
    nodeIcon->hide();
    infectionIcon->hide();
    lockIcon->hide();
    addPortFlag(Crashed);
    crashIcon->show();
}

void Node::honeyPot()
{
    nodeIcon->setMaterialName("HoneyPotIcon");
    addPortFlag(HoneyPot);
}

void Node::unHoneyPot()
{
    if(nodeFlags & IO) {
        nodeIcon->setMaterialName("IoNode");
    } else if(nodeFlags & Scanner){
        nodeIcon->setMaterialName("EScannerNode");
    } else if(nodeFlags & Data){
        nodeIcon->setMaterialName("EDataNode");
    } else if(nodeFlags & Control){
        nodeIcon->setMaterialName("EControlNode");
    } else if(nodeFlags & Spawner){
        nodeIcon->setMaterialName("ESpawnerNode");
    } else if(nodeFlags & Wetware){
        nodeIcon->setMaterialName("EWetwareNode");
    }
    removePortFlag(HoneyPot);
}

//===================
//Bit Field Stuff
//===================
void Node::removePortFlag(NodeFlags flag)
{
    nodeFlags &= ~flag;
    updateGUI();
}

void Node::addPortFlag(NodeFlags flag)
{
    nodeFlags |= flag;
    updateGUI();
}

void Node::setPortFlags(int nodeFlags)
{
    this->nodeFlags = nodeFlags;
    updateGUI();
}

bool Node::containsFlag(NodeFlags flag)
{
    if(nodeFlags & flag) {
        return true;
    } else {
        return false;
    }
}

void Node::setParent(boost::shared_ptr<Room> parent)
{
    this->parent = parent;
}

boost::shared_ptr<Room> Node::getParent()
{
    return parent;
}

void Node::startRespawner(boost::shared_ptr<Program> program)
{
    if(this->program.use_count()==0) {
        this->program = program;
    }
}

void Node::stopRespawner()
{
    if(program.use_count()!=0) {
//        delete program;
//        program = 0;
        program->deleteResources();
        program.reset();
    }
}

bool Node::isRespawnActive()
{
    if(program.use_count()==0){
        return false;
    } else {
        return true;
    }
}

void Node::updateRespawner(float progress)
{
    if(program.use_count()!=0) {
        program->update(progress);
    }
}

void Node::changeType(NodeFlags flag)
{
    removePortFlag(IO);
    removePortFlag(Scanner);
    removePortFlag(Data);
    removePortFlag(Spawner);
    removePortFlag(Wetware);
    removePortFlag(Control);
    addPortFlag(flag);
}

}
