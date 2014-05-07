#include "neuroai.h"
#include "src/Core/terminal.h"
#include "boost/lexical_cast.hpp"
#include "src/Core/Entropy.h"
#include "OGRE/Ogre.h"

namespace neuro {

NeuroAI::NeuroAI()
{

}

NeuroAI::NeuroAI(Vec2f pos, std::string iconName) :
    pos(pos),
    iconName(iconName),
    health(1.0),
    enemy(true)
{
    this->id = aid++;
}

NeuroAI::~NeuroAI()
{
//    Terminal::getSingletonPtr()->_panel2->removeChild(iconName+boost::lexical_cast<std::string>(this->id ));
    //    Terminal::getSingletonPtr()->_overlayMgr->destroyOverlayElement(aiIcon);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(aiIcon);
}

void NeuroAI::setupGUI()
{
    Ogre::MaterialPtr aiMat = Ogre::MaterialManager::getSingleton().create(iconName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    aiMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/"+iconName+".png");
    aiMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    aiMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    aiMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    aiMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    aiIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", iconName+boost::lexical_cast<std::string>(this->id )));
    aiIcon->setMetricsMode(Ogre::GMM_PIXELS);
    aiIcon->setPosition(pos.x*Terminal::getSingletonPtr()->pixelSize*0.5,pos.y*Terminal::getSingletonPtr()->pixelSize*0.5);
    aiIcon->setWidth(Terminal::getSingletonPtr()->pixelSize);
    aiIcon->setHeight(Terminal::getSingletonPtr()->pixelSize);
    aiIcon->setParameter("uv_coords","0 0 1 1");
    aiIcon->setMaterialName(iconName);
    Terminal::getSingletonPtr()->_panel3->addChild(aiIcon);
    updateR = nrand(0,990)*0.001;
}

bool NeuroAI::checkForUpdate()
{
    if(status == Walking||status == Idle||status == AtDestination) {
        updateR+=speed;
        if(updateR>=1.0) {
            updateR = 0.0f;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void NeuroAI::move()
{
    if(nextPoint.x==0&&nextPoint.y==0) {
        status = AtDestination;
    } else {
        if (pos.y > nextPoint.y) {
            pos.y = pos.y - speed;
            if(prevPoint.y > nextPoint.y)
                facing = DOWN;
        }
        if (pos.y < nextPoint.y) {
            pos.y = pos.y + speed;
            if(prevPoint.y < nextPoint.y)
                facing = UP;
        }
        if (pos.x > nextPoint.x) {
            pos.x = pos.x - speed;
            if(prevPoint.x > nextPoint.x)
                facing = LEFT;
        }
        if (pos.x < nextPoint.x) {
            pos.x = pos.x + speed;
            if(prevPoint.x < nextPoint.x)
                facing = RIGHT;
        }
    }
    aiIcon->setPosition(pos.x*Terminal::getSingletonPtr()->pixelSize,pos.y*Terminal::getSingletonPtr()->pixelSize);
}

void NeuroAI::setNextPoint(Vec2i nextPoint)
{
    prevPoint = this->nextPoint;
    this->nextPoint = nextPoint;
}

Vec2f NeuroAI::getPos()
{
    return pos;
}

Vec2i NeuroAI::getIPos()
{
    Vec2i iPos;
    iPos.x = pos.x;
    iPos.y = pos.y;
    return iPos;
}

void NeuroAI::makeFriendly()
{
    enemy = false;
}

void NeuroAI::attack()
{

}

int NeuroAI::getID()
{
    return this->id;
}

int NeuroAI::getTargetID()
{
    return this->targetID;
}

void NeuroAI::setTargetID(int tid)
{
    this->targetID = tid;
}

void NeuroAI::updateProjectiles()
{

}

void NeuroAI::update()
{

}

void NeuroAI::setStatus(int status)
{
    this->status = status;
}

int NeuroAI::getStatus()
{
    return status;
}

void NeuroAI::setMetaStatus(int metaStatus)
{
    this->metaStatus = metaStatus;
}

int NeuroAI::getMetaStatus()
{
    return metaStatus;
}

}
