#ifndef INVENTORY_H
#define INVENTORY_H

// Ogre includes
#include <OGRE/OgreSingleton.h>

// Entropy includes
#include "Script/scripttypes.h"

namespace Ogre {
    class OverlayManager;
    class Overlay;
    class OverlayContainer;
}

class InventoryItem;

class Inventory : public Ogre::Singleton<Inventory>
{
public:
    Inventory();
    ~Inventory();

    void change();
    void openInventory();
    void closeInventory();

    void addItem(std::string itemName, std::string iconName, std::string description);
    void removeItem(std::string itemName);
    void combineItems(std::string itemName1, std::string itemName2);
    void addUseAction(std::string itemName, std::string targetObjectType, script::lua_functor_t action);
    void removeUseAction(std::string itemName, std::string targetObjectType);
    void useAction(std::string itemName, std::string targetObjectType);

    static void staticAddItem(std::string itemName, std::string iconName, std::string description);
    static void staticRemoveItem(std::string itemName);
    static void staticCombineItems(std::string itemName1, std::string itemName2);
    static void staticAddUseAction(std::string itemName, std::string targetObjectType, script::lua_functor_t action);
    static void staticRemoveUseAction(std::string itemName, std::string targetObjectType);
    static void staticUseAction(std::string itemName, std::string targetObjectType);

    bool isOpen();
    bool isEquipped();
    InventoryItem* getEquipped();
    void equipItem(std::string name);
    void unEquipItem();

    void mouseMoved(float x,float y);
    void mouseClicked(float x, float y);

    static Inventory& getSingleton(void);
    static Inventory* getSingletonPtr(void);

    typedef std::list<InventoryItem*>::iterator item_index_t;

private:
    std::string equipName;
    bool equipped;

    InventoryItem* getItem(std::string itemName);
    bool open;
    Ogre::OverlayManager*   overlayMgr;
    Ogre::Overlay*   invOverlay;
    Ogre::OverlayContainer* invContainer;
    Ogre::OverlayContainer* descContainer;

    std::list<InventoryItem*> items;
};

#endif // INVENTORY_H
