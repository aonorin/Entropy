#include "questentry.h"
#include "Core/Entropy.h"
#include <OGRE/Ogre.h>
#include "OGRE/OgreTextAreaOverlayElement.h"

QuestEntry::QuestEntry(int id) :
    id(id),
    rows(8)
{
    switch(id) {
    case FindResearcher:
        name = "FindResearcher";
        description = "Find the missing researcher who contacted you.";
        break;
    case RadioBase:
        name = "RadioBase";
        description = "Find and use a radio device to communicate with base.";
        break;
    case ObtainCompound:
        name = "ObtainCompound";
        description = "Construct the mysterious compound.";
        break;
    }

    //InventoryItem Material
    Ogre::MaterialPtr questMaterial = Ogre::MaterialManager::getSingleton().create(name+"questMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    questMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("questIcon.png");
    questMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    questMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    questMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    questMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //InventoryItem Material
    Ogre::MaterialPtr hiquestMaterial = Ogre::MaterialManager::getSingleton().create(name+"hiquestMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    hiquestMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("questIcon.png");
    hiquestMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    hiquestMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    hiquestMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    hiquestMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_ONE);

    overlayMgr = Ogre::OverlayManager::getSingletonPtr();
    questContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->getOverlayElement("questContainer"));

    questElement = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel",name);
    questElement->setMetricsMode(Ogre::GMM_RELATIVE);
    questElement->setDimensions(0.06,0.08);
    questElement->setPosition(0,0);
    questElement->setParameter("uv_coords","0 0 1 1");
    questElement->setMaterialName(name+"questMaterial");
    questContainer->addChild(questElement);
    questElement->show();

    questDesc = (Ogre::TextAreaOverlayElement*)overlayMgr->createOverlayElement("TextArea",name+"questDesc");
    questDesc->setMetricsMode(Ogre::GMM_RELATIVE);
    questDesc->setDimensions(0.1,0.9);
    questDesc->setPosition(0.05, 0.05);
    questDesc->setFontName("bluehigh");
    questDesc->setCharHeight(0.03);
    questDesc->setAlignment(Ogre::TextAreaOverlayElement::Left);
    questDesc->setColour(Ogre::ColourValue(1,1,1,0.9));
    questDesc->setCaption(description);
    static_cast<Ogre::OverlayContainer*>(overlayMgr->getOverlayElement("questDescContainer"))->addChild(questDesc);
    questDesc->show();
}

QuestEntry::~QuestEntry()
{
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(questElement);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(questDesc);
}

//===================================
//Getters/Setters
//===================================

void QuestEntry::setPos(int pos)
{
    this->pos = pos;
//    std::cout << "Inventory item: " << id << " at pos: " <<pos << " with coords: " <<x << "," <<y<<std::endl;
    questElement->setPosition(0.0,0.1*pos);
    questDesc->setPosition(0.075,0.1*pos);
}

bool QuestEntry::isInsideElement(float x, float y)
{
    float adjX = x -0.1;
    float adjY = y -0.05;
    //Check Prev Arrow
    if(
            adjX>=questDesc->getLeft()&&
            adjX<=questDesc->getLeft()+questDesc->getWidth()&&
            adjY>=questDesc->getTop()&&
            adjY<=questDesc->getTop()+questDesc->getHeight()
            ) {
        hilight();
        return true;
    } else {
        unhilight();
        return false;
    }
}

void QuestEntry::hilight()
{
    questElement->setMaterialName(name+"hiquestMaterial");
    questDesc->setColour(Ogre::ColourValue(0,0.8,1,1));
}

void QuestEntry::unhilight()
{
    questElement->setMaterialName(name+"questMaterial");
    questDesc->setMaterialName(name+"invMaterial");
    questDesc->setColour(Ogre::ColourValue(1,1,1,0.9));
}

bool QuestEntry::isActive()
{
    return activated;
}

void QuestEntry::deactivate()
{
    activated = false;
}

void QuestEntry::activate()
{
    activated = true;
}

int QuestEntry::getID()
{
    return this->id;
}

std::string QuestEntry::getName()
{
    return name;
}

