#ifndef CURSOR_H
#define CURSOR_H

#include <OGRE/OgreSingleton.h>

namespace Ogre {
    class OverlayManager;
    class Overlay;
    class OverlayContainer;
}

class EntropyCursor: public Ogre::Singleton<EntropyCursor>
{
public:
    EntropyCursor();

    static EntropyCursor& getSingleton(void);
    static EntropyCursor* getSingletonPtr(void);

    void use();
    void unUse();
    void setCursor(int cursorType);
    void setCursor(std::string itemName);
    void unequip();
    void queuedSetCursor();
    void queuedSetItemCursor();
private:
    bool isUsing;
    int cursorState;
    std::string itemName;
    Ogre::OverlayManager*   overlayMgr;
    Ogre::Overlay*   cursorOverlay;
    Ogre::OverlayContainer* cursor;
};

#endif // CURSOR_H
