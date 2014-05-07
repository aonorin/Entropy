#include "inventory.h"
#include "gamemenu.h"
#include "Core/update.h"
#include "src/Menu/entropycursor.h"
#include <OGRE/Ogre.h>
#include "src/Menu/inventoryitem.h"

Inventory::Inventory() :
    open(false),
    equipped(false)
{
    overlayMgr = Ogre::OverlayManager::getSingletonPtr();

    invOverlay = overlayMgr->create("invOverlay");
    invOverlay->setZOrder(99);
    invOverlay->setScale(1,1);
    invOverlay->hide();

    Ogre::MaterialPtr invBackgroundMat = Ogre::MaterialManager::getSingleton().create("invBackgroundMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    invBackgroundMat->getTechnique(0)->getPass(0)->createTextureUnitState("InventoryBackground.png");
    invBackgroundMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    invBackgroundMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    invBackgroundMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    invBackgroundMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    invContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "invContainer"));
    invContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    invContainer->setHorizontalAlignment(Ogre::GHA_LEFT);
    invContainer->setVerticalAlignment(Ogre::GVA_TOP);
    invContainer->setDimensions(0.8,0.6);
    invContainer->setPosition(0.1,0.025);
    invContainer->show();
    invContainer->setMaterialName("invBackgroundMat");
    invOverlay->add2D(invContainer);

    descContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "descContainer"));
    descContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    descContainer->setHorizontalAlignment(Ogre::GHA_LEFT);
    descContainer->setVerticalAlignment(Ogre::GVA_TOP);
    descContainer->setDimensions(0.8,0.35);
    descContainer->setPosition(0.1,0.625);
    descContainer->show();
    descContainer->setMaterialName("invBackgroundMat");
    invOverlay->add2D(descContainer);
}

void Inventory::change()
{
    if(open) {
        closeInventory();
    } else {
        openInventory();
    }
}

void Inventory::openInventory()
{
    open = true;
    invOverlay->show();
    GameMenu::getSingletonPtr()->blackoutOverlay->show();
    GameMenu::getSingletonPtr()->mouseContainer->show();
//    Update::getSingletonPtr()->pause();
}

void Inventory::closeInventory()
{
    open = false;
    invOverlay->hide();
    GameMenu::getSingletonPtr()->blackoutOverlay->hide();
    GameMenu::getSingletonPtr()->mouseContainer->hide();
//    Update::getSingletonPtr()->unpause();
}


void Inventory::addItem(std::string itemName, std::string iconName, std::string description)
{
    InventoryItem* item = new InventoryItem(itemName,iconName,description);
    items.push_back(item);
    std::list<InventoryItem*>::iterator iter = items.begin();
    int i=0;
    while(iter != items.end()) {
        (*iter)->setPos(i);
        i++;
        ++iter;
    }
}

void Inventory::removeItem(std::string itemName)
{
    std::list<InventoryItem*>::iterator iter = items.begin();
    while(iter != items.end()) {
        if((*iter)->getName().compare(itemName)==0) {
            break;
        }
        ++iter;
    }
    if(iter!=items.end()) {
        delete (*iter);
        items.erase(iter);
    }
    iter = items.begin();
    int i=0;
    while(iter != items.end()) {
        (*iter)->setPos(i);
        i++;
        ++iter;
    }
}

void Inventory::combineItems(std::string itemName1, std::string itemName2)
{

}

void Inventory::addUseAction(std::string itemName, std::string targetObjectType, script::lua_functor_t action)
{
    InventoryItem* item = getItem(itemName);
    if(item)
    {
        item->addUseAction(targetObjectType, action);
    }
}

void Inventory::removeUseAction(std::string itemName, std::string targetObjectType)
{
    InventoryItem* item = getItem(itemName);
    if(item)
    {
        item->removeUseAction(targetObjectType);
    }
}

void Inventory::useAction(std::string itemName, std::string targetObjectType)
{
    InventoryItem* item = getItem(itemName);
    if(item)
    {
        item->useAction(targetObjectType);
    }
}

InventoryItem* Inventory::getItem(std::string itemName)
{
    std::list<InventoryItem*>::iterator iter = items.begin();
    while(iter != items.end())
    {
        if((*iter)->getName().compare(itemName) == 0)
        {
            return *iter;
        }
    }

    return 0;
}

void Inventory::staticAddItem(std::string itemName, std::string iconName, std::string description)
{
    Inventory::getSingletonPtr()->addItem(itemName, iconName, description);
}

void Inventory::staticRemoveItem(std::string itemName)
{
    Inventory::getSingletonPtr()->removeItem(itemName);
}

void Inventory::staticCombineItems(std::string itemName1, std::string itemName2)
{
    Inventory::getSingletonPtr()->combineItems(itemName1, itemName2);
}

void Inventory::staticAddUseAction(std::string itemName, std::string targetObjectType, script::lua_functor_t action)
{
    Inventory::getSingletonPtr()->addUseAction(itemName, targetObjectType, action);
}

void Inventory::staticRemoveUseAction(std::string itemName, std::string targetObjectType)
{
    Inventory::getSingletonPtr()->removeUseAction(itemName, targetObjectType);
}

void Inventory::staticUseAction(std::string itemName, std::string targetObjectType)
{
    Inventory::getSingletonPtr()->useAction(itemName, targetObjectType);
}

void Inventory::mouseMoved(float x, float y)
{
    for(std::list<InventoryItem*>::iterator iter = items.begin();iter!=items.end();++iter) {
        (*iter)->isInsideElement(x,y);
    }
}

void Inventory::mouseClicked(float x, float y)
{
    for(std::list<InventoryItem*>::iterator iter = items.begin();iter!=items.end();++iter) {
        if((*iter)->isInsideElement(x,y)) {
            (*iter)->equip();
            equipName = (*iter)->getName();
            std::cout << "EQUIPPING: " << equipName << std::endl;
            equipped = true;
            EntropyCursor::getSingletonPtr()->setCursor(equipName);
            closeInventory();
        }
    }
}

void Inventory::unEquipItem()
{
    equipName = " ";
    equipped = false;
    EntropyCursor::getSingletonPtr()->unequip();
}

bool Inventory::isOpen()
{
    return open;
}

bool Inventory::isEquipped()
{
    return equipped;
}

InventoryItem* Inventory::getEquipped()
{
    InventoryItem* equippedItem = 0;
    std::list<InventoryItem*>::iterator iter = items.begin();
    while(iter != items.end()) {
        if((*iter)->getName().compare(equipName)==0) {
            equippedItem = (*iter);
        }
        ++iter;
    }
    return equippedItem;
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> Inventory* Ogre::Singleton<Inventory>::ms_Singleton = 0;
Inventory* Inventory::getSingletonPtr(void)
{
    return ms_Singleton;
}
Inventory& Inventory::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> Inventory* Ogre::Singleton<Inventory>::msSingleton = 0;
Inventory* Inventory::getSingletonPtr(void)
{
    return msSingleton;
}
Inventory& Inventory::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
