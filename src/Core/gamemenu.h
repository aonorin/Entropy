#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <Ogre.h>
#include <OgreSingleton.h>
#include "OgreTextAreaOverlayElement.h"

using namespace Ogre;

class GameMenu : public Ogre::Singleton<GameMenu>
{
public:
    GameMenu();
    ~GameMenu();

    enum MenuMode{Gameplay,Opening,Main,Load,Save,Settings};

    Ogre::OverlayManager*    overlayMgr;
    Ogre::Overlay*           menuOverlay;
    Ogre::Overlay*           backgroundOverlay;
    Ogre::Overlay*           mouseOverlay;

    Ogre::OverlayContainer* mouseContainer;
    Ogre::OverlayElement* mousePanel;

    static GameMenu& getSingleton(void);
    static GameMenu* getSingletonPtr(void);

    void mouseMoved(float x, float y);
    void mousePressed();
    int getMenuState();

    void openMainMenu();
    void openSettingsMenu();

private:
    void startNewGame();
    void continueGameM();
    void loadGame();
    void saveGame();
    void quitGame();
    void changeFullScreen();
    void changeFSAA();
    void changeResolution();
    void applySettings();

    bool fullscreen;
    int fsaa;
    int resolution;
    StringVector foundRenderDevices;
    StringVector foundResolutions;
    void getCurrentSettings();

    int idCounter;
    OverlayElement* getElement(std::string elementName);
    OverlayContainer* getContainer(std::string containerName);
    void hilightElement(std::string elementName);
    void unhilightElement(std::string elementName);

    bool insideElement(std::string elementName);
    void addButton(std::string buttonName,std::string parentName,int order,std::string caption);
    void addTextElement(std::string elementName,std::string parentName,int order,std::string caption);
    void addGUIElement(std::string elementName,std::string parentName,int order);
    void addMenu(std::string menuName);
    MenuMode mode;
    void setupMouse();
    void setupOpeningMenu();
    void setupMainMenu();
    void setupLoadMenu();
    void setupSaveMenu();
    void setupSettingsMenu();
};

#endif // GAMEMENU_H
