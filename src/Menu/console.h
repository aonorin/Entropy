#ifndef CONSOLE_H
#define CONSOLE_H


#include <OGRE/OgreSingleton.h>
#include "src/Menu/menufuncs.h"
#include "src/Menu/consolefile.h"

namespace Ogre {
    class Overlay;
    class OverlayContainer;
    class OverlayManager;
    class TextAreaOverlayElement;
}

class Console : public Ogre::Singleton<Console>
{
public:
    Console();
    ~Console();

    void change();
    void open();
    void close();
    bool isOpen();
    bool isClosed();
    void enterText(EntKeyCode key);
    void deleteText();
    void returnKey();
    void animateConsole(float progress);
    void done();
    void shiftDown();
    void shiftUp();
    void print(const char* chars);
    void print(const std::string string);

    void mouseMoved(float x,float y);
    void mouseClicked(float x, float y);

    static Console& getSingleton(void);
    static Console* getSingletonPtr(void);

private:
    int savedNum;
    int charWrap;
    void processOutputString();
    int pureXPos;
    std::vector<std::string> yTestString;
    std::vector<std::string> executedStrings;
    std::deque<std::string> savedStrings;
    float cursorYPos;
    float cursorXPos;
    float fontHeight;
    float fontWidth;
    bool shift;
    int wrapNum;
    int numLines;
    bool opened;
    Ogre::OverlayManager*   overlayMgr;
    Ogre::Overlay*   consoleOverlay;
    Ogre::Overlay*   consoleCursorOverlay;
    Ogre::OverlayContainer* outputContainer;
    Ogre::OverlayContainer* inputContainer;
    Ogre::OverlayContainer* consoleContainer;
    Ogre::OverlayContainer* consoleCursorContainer;

    Ogre::TextAreaOverlayElement* consoleText;
    Ogre::TextAreaOverlayElement* inputBox;

    Ogre::OverlayElement* consoleCursor;

    std::deque<std::string> consoleString;
    std::string finalChatString;
    std::string executeString;
    std::string inputString;
    std::string outputString;

    ConsoleFilePtr consoleFile;
};

#endif // CONSOLE_H
