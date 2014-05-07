#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

// System includes
#include <string>
#ifndef WIN32
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

// Entropy includes
#include "Script/scripttypes.h"

//forward declerations
namespace Ogre{
    class TextAreaOverlayElement;
    class OverlayManager;
    class OverlayContainer;
    class OverlayElement;
}

class InventoryItem {
public:
    InventoryItem(std::string name, std::string iconName, std::string description);
    ~InventoryItem();
    std::string getName();
    void setPos(int pos);
    void equip();
    void unequip();
    bool isEquipped();
    void hilight();
    void unhilight();
    bool isInsideElement(float x, float y);
    // action is defined as a string of lua code to interpret. Actions are unique to objects, no duplicate entries
    void addUseAction(std::string targetObjectType, script::lua_functor_t action);
    void removeUseAction(std::string targetObjectType);
    void useAction(std::string targetObjectType);
    bool containsAction(std::string targetObjectType);

private:
    bool equipped;
    int rows;
    int columns;
    Ogre::OverlayManager*   overlayMgr;
    Ogre::OverlayContainer* invContainer;
    Ogre::OverlayElement* invElement;
    Ogre::TextAreaOverlayElement* invDesc;
    int id;
    int pos;
    std::string name;
    std::string iconName;
    std::string description;
    std::tr1::unordered_map<std::string, script::lua_functor_t> useActionMap;
};

#endif // INVENTORYITEM_H
