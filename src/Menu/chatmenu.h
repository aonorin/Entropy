#ifndef CHATMENU_H
#define CHATMENU_H

#include <OGRE/OgreSingleton.h>
#include "Menu/chatscript.h"
#include "Menu/menufuncs.h"

namespace Ogre {
    class OverlayManager;
    class Overlay;
    class OverlayContainer;
    class TextAreaOverlayElement;
}

class ChatScript;

class ChatMenu : public Ogre::Singleton<ChatMenu>
{
public:
    ChatMenu();
    ~ChatMenu();

    void change();
    void open();
    void close();
    bool isOpen();
    void enterText(EntKeyCode key);
    void deleteText();
    void returnKey();
    void botSpeak(char* text);
    void shiftDown();
    void shiftUp();
    void animate(float progress);
    void done();

    void mouseMoved(float x,float y);
    void mouseClicked(float x, float y);

    static ChatMenu& getSingleton(void);
    static ChatMenu* getSingletonPtr(void);

private:
    bool shift;
    int wrapNum;
    bool opened;
    Ogre::OverlayManager*   overlayMgr;
    Ogre::Overlay*   chatOverlay;
    Ogre::OverlayContainer* chatContainer;
    Ogre::OverlayContainer* inputContainer;

    Ogre::TextAreaOverlayElement* chatBox;
    Ogre::TextAreaOverlayElement* inputBox;

    std::deque<std::string> chatString;
    std::string finalChatString;
    std::string inputString;

    ChatScript* chat;
};
#endif // CHATMENU_H
