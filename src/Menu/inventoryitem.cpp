// Ogre includes
#include "OGRE/OgreTextAreaOverlayElement.h"
#include <OGRE/Ogre.h>

// Entropy includes
#include "inventoryitem.h"
#include "Core/Entropy.h"
#include "src/Script/script.h"

InventoryItem::InventoryItem(std::string name, std::string iconName, std::string description) :
    columns(6),
    rows(8),
    name(name),
    iconName(iconName),
    description(description)
{
    /*
    switch(id) {
    case Flashlight:
        name = "Flashlight";
        iconName = "Flashlight.png";
        description = "A modern flashlight with long lasting batteries.";
        break;
    case StrangeRock:
        name = "Strange Rock";
        iconName = "StrangeRock.png";
        description = "A strange rock which seems to be slowly pulsating, and always slightly warm.";
        break;
    case VirgilComm:
        name = "Virgil Comm Device";
        iconName = "VirgilComm.png";
        description = "A communications relay that allows me to communicate with Virgil.";
        break;
    case SonicNavigator:
        name = "Sonic Pathfinder";
        iconName = "SonicPathfinder.png";
        description = "A device that emits frequency modulated ultra-sonic sound waves to produce unidirectional echos\nfor spatial recognition without the aid of sight.";
        break;
    }
    */

    //InventoryItem Material
    Ogre::MaterialPtr invMaterial = Ogre::MaterialManager::getSingleton().create(name+"invMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    invMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(iconName);
    invMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    invMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    invMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    invMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //InventoryItem Material
    Ogre::MaterialPtr hiinvMaterial = Ogre::MaterialManager::getSingleton().create(name+"hiinvMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    hiinvMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(iconName);
    hiinvMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    hiinvMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    hiinvMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    hiinvMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_ONE);

    overlayMgr = Ogre::OverlayManager::getSingletonPtr();
    invContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->getOverlayElement("invContainer"));

    invElement = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel",name);
    invElement->setMetricsMode(Ogre::GMM_RELATIVE);
    invElement->setDimensions(0.06,0.08);
    invElement->setPosition(0,0);
    invElement->setParameter("uv_coords","0 0 1 1");
    invElement->setMaterialName(name+"invMaterial");
    invContainer->addChild(invElement);
    invElement->show();

    invDesc = (Ogre::TextAreaOverlayElement*)overlayMgr->createOverlayElement("TextArea",name+"invDesc");
    invDesc->setMetricsMode(Ogre::GMM_RELATIVE);
    invDesc->setDimensions(0.6,0.3);
    invDesc->setPosition(0.05, 0.05);
    invDesc->setFontName("bluehigh");
    invDesc->setCharHeight(0.03);
    invDesc->setAlignment(Ogre::TextAreaOverlayElement::Left);
    invDesc->setColour(Ogre::ColourValue(1,1,1,0.9));
    invDesc->setCaption(description);
    static_cast<Ogre::OverlayContainer*>(overlayMgr->getOverlayElement("descContainer"))->addChild(invDesc);
    invDesc->hide();
}

InventoryItem::~InventoryItem()
{
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(invElement);
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(invDesc);
}

void InventoryItem::setPos(int pos)
{
    this->pos = pos;
    int px = pos%rows;
    int py = pos/columns;
    float x = ((float)px/(float)rows)*0.8+0.05;
    float y = ((float)py/(float)columns)*0.8+0.05;
//    std::cout << "Inventory item: " << id << " at pos: " <<pos << " with coords: " <<x << "," <<y<<std::endl;
    invElement->setPosition(x,y);
}

//===================================
//Getters/Setters
//===================================
bool InventoryItem::isInsideElement(float x, float y)
{
    float adjX = x -0.1;
    float adjY = y -0.05;
    //Check Prev Arrow
    if(
            adjX>=invElement->getLeft()&&
            adjX<=invElement->getLeft()+invElement->getWidth()&&
            adjY>=invElement->getTop()&&
            adjY<=invElement->getTop()+invElement->getHeight()
            ) {
        hilight();
        invDesc->show();
        return true;
    } else {
        unhilight();
        invDesc->hide();
        return false;
    }
}

void InventoryItem::addUseAction(std::string targetObjectType, script::lua_functor_t action)
{
    if(!containsAction(targetObjectType))
    {
        useActionMap[targetObjectType] = action;
    }

    else
    {
        std::cerr << "InventoryItem: " << this->name << "duplicate action object type " << targetObjectType
                  << ". Discarding addUseAction() request." << std::endl;
    }
}

void InventoryItem::removeUseAction(std::string targetObjectType)
{
    useActionMap.erase(targetObjectType);
}

void InventoryItem::useAction(std::string targetObjectType)
{
    if(containsAction(targetObjectType))
    {
        useActionMap[targetObjectType]->call();
    }
}

bool InventoryItem::containsAction(std::string targetObjectType)
{
//    std::cout <<"OBJECT TYPE: " << targetObjectType << std::endl;
    std::tr1::unordered_map<std::string, script::lua_functor_t>::iterator find = useActionMap.find(targetObjectType);
    if(find != useActionMap.end())
    {
        return true;
    }

    else
    {
        return false;
    }
}

void InventoryItem::hilight()
{
    invElement->setMaterialName(name+"hiinvMaterial");
}

void InventoryItem::unhilight()
{
    invElement->setMaterialName(name+"invMaterial");
}

bool InventoryItem::isEquipped()
{
    return equipped;
}

void InventoryItem::unequip()
{
    equipped = false;
}

void InventoryItem::equip()
{
    equipped = true;
}

std::string InventoryItem::getName()
{
    return name;
}
