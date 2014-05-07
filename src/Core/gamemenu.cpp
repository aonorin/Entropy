#include "gamemenu.h"
#include "src/Core/Entropy.h"
#include <boost/lexical_cast.hpp>
#include "libraries/tinyxml/tinyxml.h"
#include <iostream>
#include <fstream>

GameMenu::GameMenu() :
    mode(Opening),
    idCounter(0),
    fullscreen(false),
    fsaa(0),
    resolution(0)
{
    //Get Renderer Options
    Ogre::ConfigOptionMap& CurrentRendererOptions = Entropy::getSingletonPtr()->mRoot->getRenderSystem()->getConfigOptions();
    Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin();
    while( configItr != CurrentRendererOptions.end() )
    {
        if( (configItr)->first == "Rendering Device" )
        {
            // Store Available Rendering Devices
            foundRenderDevices = ((configItr)->second.possibleValues);
        }
        if( (configItr)->first == "Video Mode" )
        {
            // Store Available Resolutions
            foundResolutions = ((configItr)->second.possibleValues);
        }
        configItr++;
    }

    overlayMgr = Ogre::OverlayManager::getSingletonPtr();

    menuOverlay = overlayMgr->create("gameMenuOverlay");
    menuOverlay->setZOrder(290);
    menuOverlay->setScale(1,1);
    menuOverlay->show();

    backgroundOverlay = overlayMgr->create("backgroundOverlay");
    backgroundOverlay->setZOrder(280);
    backgroundOverlay->setScale(1,1);
    backgroundOverlay->show();

    mouseOverlay = overlayMgr->create("MouseOverlay");
    mouseOverlay->setZOrder(300);
    mouseOverlay->setScale(1,1);
    mouseOverlay->show();

    //Background Material
    Ogre::MaterialPtr backgroundMaterial = Ogre::MaterialManager::getSingleton().create("backgroundMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    backgroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("glossy_button.png");
    backgroundMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    backgroundMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    backgroundMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    backgroundMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //Hilighted Background Material
    Ogre::MaterialPtr hibackgroundMaterial = Ogre::MaterialManager::getSingleton().create("hibackgroundMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    hibackgroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("glossy_button.png");
    hibackgroundMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    hibackgroundMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    hibackgroundMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    hibackgroundMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_ONE_MINUS_SOURCE_COLOUR, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //Arrow Material
    Ogre::MaterialPtr arrowMaterial = Ogre::MaterialManager::getSingleton().create("arrowMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    arrowMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("arrow_button.png");
    arrowMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    arrowMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    arrowMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    backgroundMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //HiLighted Arrow Material
    Ogre::MaterialPtr hiarrowMaterial = Ogre::MaterialManager::getSingleton().create("hiarrowMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    hiarrowMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("arrow_button.png");
    hiarrowMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    hiarrowMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    hiarrowMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    hiarrowMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //Mouse Material
    Ogre::MaterialPtr mouseMaterial = Ogre::MaterialManager::getSingleton().create("mouseMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    mouseMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("white_cursor.png");
    mouseMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    mouseMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    mouseMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    mouseMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    setupMouse();
    setupOpeningMenu();
    setupMainMenu();
    setupLoadMenu();
    setupSaveMenu();
    setupSettingsMenu();

    getContainer("openingMenu")->show();
    getContainer("openingMenuBackground")->show();
}

void GameMenu::getCurrentSettings()
{
    //Get Current Resolution
    std::string resString =
            boost::lexical_cast<std::string>(Entropy::getSingletonPtr()->mWindow->getViewport(0)->getActualWidth()) +
            " x " +
            boost::lexical_cast<std::string>(Entropy::getSingletonPtr()->mWindow->getViewport(0)->getActualHeight());
    for(int i=0;i<foundResolutions.size();i++) {
        if(resString.compare(foundResolutions.at(i))==0) {
            resolution = i;
        }
    }
    //Get Current FSAA
    fsaa = Entropy::getSingletonPtr()->mWindow->getFSAA();
    fullscreen = Entropy::getSingletonPtr()->mWindow->isFullScreen();
}

void GameMenu::setupMouse()
{
    mouseContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "mouseContainer"));
    mouseContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    mouseContainer->setHorizontalAlignment(GHA_LEFT);
    mouseContainer->setVerticalAlignment(GVA_TOP);
    mouseContainer->setDimensions(1,1);
    mouseContainer->setPosition(0,0);

    //Background
    mousePanel = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel","mousePanel");
    mousePanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mousePanel->setDimensions(0.02,0.03);
    mousePanel->setPosition(0,0);
    mousePanel->setParameter("uv_coords","0 0 1 1");
    mousePanel->setMaterialName("mouseMaterial");
    mouseContainer->addChild(mousePanel);
    mousePanel->show();

    mouseOverlay->add2D(mouseContainer);
    mouseContainer->show();
}

void GameMenu::setupOpeningMenu()
{
    addMenu("openingMenu");
    addButton("startNew","openingMenu",0,"New Game");
    addButton("continueGame","openingMenu",1,"Continue");
}

void GameMenu::setupMainMenu()
{
    addMenu("mainMenu");
    addButton("loadButton","mainMenu",0,"Load");
    addButton("saveButton","mainMenu",1,"Save");
    addButton("settingsButton","mainMenu",2,"Settings");
    addButton("exitMain","mainMenu",3,"Exit");
    addButton("quitButton","mainMenu",4,"Quit");
}

void GameMenu::setupLoadMenu()
{  
    addMenu("loadMenu");
}

void GameMenu::setupSaveMenu()
{
    addMenu("saveMenu");
}

void GameMenu::setupSettingsMenu()
{
    addMenu("settingsMenu");
    addButton("fullScreen","settingsMenu",0,"Fullscreen: Off");
    addButton("resolution","settingsMenu",1,"1280x960");
    addButton("fsaa","settingsMenu",2,"FSAA: 0");
    addButton("apply","settingsMenu",3,"Apply");
    addButton("exit","settingsMenu",4,"Exit");

    getElement("resolution")->setCaption(foundResolutions.at(resolution));
    getElement("fsaa")->setCaption("FSAA: "+boost::lexical_cast<std::string>(fsaa));
    if(fullscreen) {
        getElement("fullScreen")->setCaption("Fullscreen: On");
    } else {
        getElement("fullScreen")->setCaption("Fullscreen: Off");
    }
}

void GameMenu::addMenu(std::string menuName)
{
    Ogre::OverlayContainer* menu = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel",menuName));
    menu->setMetricsMode(Ogre::GMM_RELATIVE);
    menu->setHorizontalAlignment(GHA_LEFT);
    menu->setVerticalAlignment(GVA_TOP);
    menu->setDimensions(1,1);
    menu->setPosition(0,0);

    Ogre::OverlayContainer* menuBackground = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel",menuName+"Background"));
    menuBackground->setMetricsMode(Ogre::GMM_RELATIVE);
    menuBackground->setHorizontalAlignment(GHA_LEFT);
    menuBackground->setVerticalAlignment(GVA_TOP);
    menuBackground->setDimensions(1,1);
    menuBackground->setPosition(0,0);

    backgroundOverlay->add2D(menuBackground);
    menuBackground->hide();
    menuOverlay->add2D(menu);
    menu->hide();
}

void GameMenu::addButton(std::string buttonName, std::string parentName, int order, std::string caption)
{
    addTextElement(buttonName,parentName,order,caption);
    addGUIElement(buttonName+"Background",parentName+"Background",order);
}

void GameMenu::addTextElement(std::string elementName,std::string parentName, int order,std::string caption)
{
    TextAreaOverlayElement* element = (TextAreaOverlayElement*)overlayMgr->createOverlayElement("TextArea",elementName);
    element->setMetricsMode(Ogre::GMM_RELATIVE);
    element->setDimensions(0.4,0.2);
    element->setPosition(0.5, 0.4+(0.1*order));
    element->setFontName("bluehigh");
    element->setCharHeight(0.03);
    element->setAlignment(TextAreaOverlayElement::Center);
    element->setColour(Ogre::ColourValue(1,1,1,0.9));
    element->setCaption(caption);
    static_cast<Ogre::OverlayContainer*>(overlayMgr->getOverlayElement(parentName))->addChild(element);
    element->show();
}

void GameMenu::addGUIElement(std::string elementName,std::string parentName, int order)
{
    OverlayElement* element = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel",elementName);
    element->setMetricsMode(Ogre::GMM_RELATIVE);
    element->setDimensions(0.15,0.08);
    element->setPosition(0.5-0.075, 0.4-0.03+(0.1*order));
    element->setParameter("uv_coords","0 0 1 1");
    element->setMaterialName("backgroundMaterial");
    static_cast<Ogre::OverlayContainer*>(overlayMgr->getOverlayElement(parentName))->addChild(element);
    element->show();
}

OverlayContainer* GameMenu::getContainer(std::string containerName)
{
    return static_cast<Ogre::OverlayContainer*>(overlayMgr->getOverlayElement(containerName));
}

OverlayElement* GameMenu::getElement(std::string elementName)
{
    return overlayMgr->getOverlayElement(elementName);
}

void GameMenu::mouseMoved(float x, float y)
{
    mousePanel->setPosition(x,y);
    switch(mode) {
    case Opening:
        if(insideElement("startNew")) {
            hilightElement("startNew");
        } else {
            unhilightElement("startNew");
        }
        if(insideElement("continueGame")) {
            hilightElement("continueGame");
        } else {
            unhilightElement("continueGame");
        }
        break;
    case Gameplay:
        break;
    case Main:
        if(insideElement("loadButton")) {
            hilightElement("loadButton");
        } else {
            unhilightElement("loadButton");
        }
        if(insideElement("saveButton")) {
            hilightElement("saveButton");
        } else {
            unhilightElement("saveButton");
        }
        if(insideElement("settingsButton")) {
            hilightElement("settingsButton");
        } else {
            unhilightElement("settingsButton");
        }
        if(insideElement("quitButton")) {
            hilightElement("quitButton");
        } else {
            unhilightElement("quitButton");
        }
        if(insideElement("exitMain")) {
            hilightElement("exitMain");
        } else {
            unhilightElement("exitMain");
        }
        break;
    case Load:
        break;
    case Save:
        break;
    case Settings:
        if(insideElement("fullScreen")) {
            hilightElement("fullScreen");
        } else {
            unhilightElement("fullScreen");
        }
        if(insideElement("resolution")) {
            hilightElement("resolution");
        } else {
            unhilightElement("resolution");
        }
        if(insideElement("fsaa")) {
            hilightElement("fsaa");
        } else {
            unhilightElement("fsaa");
        }
        if(insideElement("apply")) {
            hilightElement("apply");
        } else {
            unhilightElement("apply");
        }
        if(insideElement("exit")) {
            hilightElement("exit");
        } else {
            unhilightElement("exit");
        }
        break;
    }
}

void GameMenu::hilightElement(std::string elementName)
{
    getElement(elementName+"Background")->setMaterialName("hibackgroundMaterial");
    getElement(elementName)->setColour(Ogre::ColourValue(0.0,0.0,0.0,0.9));
}

void GameMenu::unhilightElement(std::string elementName)
{
    getElement(elementName+"Background")->setMaterialName("backgroundMaterial");
    getElement(elementName)->setColour(Ogre::ColourValue(1.0,1.0,1.0,0.9));
}

bool GameMenu::insideElement(std::string elementName)
{
    OverlayElement* element = getElement(elementName+"Background");

    if(
            mousePanel->getLeft() >= element->getLeft() &&
            mousePanel->getLeft() <= element->getLeft()+element->getWidth() &&
            mousePanel->getTop() >= element->getTop() &&
            mousePanel->getTop() <= element->getTop()+element->getHeight()
            ) {
        return true;
    } else {
        return false;
    }
}

void GameMenu::mousePressed()
{
    switch(mode) {
    case Opening:
        if(insideElement("startNew")) {
            startNewGame();
        }
        if(insideElement("continueGame")) {
            continueGameM();
        }
        break;
    case Gameplay:
        break;
    case Main:
        if(insideElement("settingsButton")) {
            openSettingsMenu();
        }
        if(insideElement("quitButton")) {
            quitGame();
        }
        if(insideElement("exitMain")) {
            openMainMenu();
        }
        break;
    case Load:
        break;
    case Save:
        break;
    case Settings:
        if(insideElement("apply")) {
            applySettings();
        } else if(insideElement("exit")) {
            openMainMenu();
        } else if(insideElement("fullScreen")) {
            changeFullScreen();
        } else if(insideElement("fsaa")) {
            changeFSAA();
        } else if(insideElement("resolution")) {
            changeResolution();
        }
        break;
    }
}

void GameMenu::applySettings()
{
    Entropy::getSingletonPtr()->mRoot->getRenderSystem()->setConfigOption("Video Mode",foundResolutions.at(resolution));
    Entropy::getSingletonPtr()->mRoot->getRenderSystem()->setConfigOption("FSAA",boost::lexical_cast<std::string>(fsaa));
    if(fullscreen) {
        Entropy::getSingletonPtr()->mRoot->getRenderSystem()->setConfigOption("Full Screen","Yes");
    } else  {
        Entropy::getSingletonPtr()->mRoot->getRenderSystem()->setConfigOption("Full Screen","No");
    }
    //Separate string out into width and height
    int middle = foundResolutions.at(resolution).find(" x ");
    std::string stringWidth(foundResolutions.at(resolution));
    std::string stringHeight(foundResolutions.at(resolution));
    stringWidth.erase(stringWidth.begin()+middle,stringWidth.end());
    stringHeight.erase(stringHeight.begin(),stringHeight.begin()+middle+3);
    std::cout << "Resolution: " << stringWidth <<","<<stringHeight<<std::endl;
    int width = boost::lexical_cast<int>(stringWidth);
    int height =boost::lexical_cast<int>(stringHeight);
    std::ofstream config("ogre.cfg");
    config << "Render System="<< Entropy::getSingletonPtr()->mRoot->getRenderSystem()->getName()<<std::endl;
    config << std::endl;
    config << "["<<Entropy::getSingletonPtr()->mRoot->getRenderSystem()->getName()<<"]"<<std::endl;
    config << "Colour Depth=" << 32 << std::endl;
    config << "FSAA=" << fsaa << std::endl;
    if(fullscreen) {
        config << "Full Screen=Yes" << std::endl;
    } else {
        config << "Full Screen=No" << 32 << std::endl;
    }
    config << "Video Mode=" << foundResolutions.at(resolution) << std::endl;
#ifdef __APPLE__ & __MACH__
    config << "macAPI=cocoa" << std::endl;
#else

#endif
//    Entropy::getSingletonPtr()->mWindow->setFullscreen(fullscreen,width,height);
//    config <<
//    Entropy::getSingletonPtr()->mWindow->getViewport(0)->setDimensions(0,0,width,height);
    /*
    Entropy::getSingletonPtr()->mWindow->setFullscreen(
                fullscreen,
                width,
                height
                );
    Entropy::getSingletonPtr()->mWindow->getViewport(0)->update();
    */
//    Entropy::getSingletonPtr()->mRoot->getRenderSystem()->reinitialise();
//    Entropy::getSingletonPtr()->mRoot->getRenderSystem()->setConfigOption();
    //    Entropy::getSingletonPtr()->mWindow->
}

void GameMenu::changeResolution()
{
    resolution = (resolution+1)%foundResolutions.size();
    getElement("resolution")->setCaption(foundResolutions.at(resolution));
}

void GameMenu::changeFSAA()
{
    switch(fsaa) {
    case 0:
        fsaa = 2;
        break;
    case 2:
        fsaa = 4;
        break;
    case 4:
        fsaa = 6;
        break;
    case 6:
        fsaa = 8;
        break;
    case 8:
        fsaa = 0;
        break;
    }
    getElement("fsaa")->setCaption("FSAA: "+boost::lexical_cast<std::string>(fsaa));
}

void GameMenu::changeFullScreen()
{
    if(!fullscreen) {
        fullscreen = true;
        getElement("fullScreen")->setCaption("Fullscreen On");
    } else {
        fullscreen = false;
        getElement("fullScreen")->setCaption("Fullscreen Off");
    }
}

void GameMenu::openSettingsMenu()
{
    if(mode==Main) {
        mode = Settings;
        getCurrentSettings();
        getElement("resolution")->setCaption(foundResolutions.at(resolution));
        getElement("fsaa")->setCaption("FSAA: "+boost::lexical_cast<std::string>(fsaa));
        if(fullscreen) {
            getElement("fullScreen")->setCaption("Fullscreen: On");
        } else {
            getElement("fullScreen")->setCaption("Fullscreen: Off");
        }
        getElement("mainMenu")->hide();
        getElement("mainMenuBackground")->hide();
        getElement("settingsMenu")->show();
        getElement("settingsMenuBackground")->show();
    }
}

void GameMenu::openMainMenu()
{
    if(mode == Gameplay) {
        mode = Main;
        getElement("mainMenu")->show();
        getElement("mainMenuBackground")->show();
        mouseContainer->show();
    } else if(mode == Main) {
        mode = Gameplay;
        getElement("mainMenu")->hide();
        getElement("mainMenuBackground")->hide();
        mouseContainer->hide();
    } else if(mode==Settings) {
        mode = Main;
        getElement("mainMenu")->show();
        getElement("mainMenuBackground")->show();
        mouseContainer->show();
        getElement("settingsMenu")->hide();
        getElement("settingsMenuBackground")->hide();
    }
}

void GameMenu::quitGame()
{
    Entropy::getSingletonPtr()->quitGame();
}

void GameMenu::startNewGame()
{
    mode = Gameplay;
    getElement("openingMenu")->hide();
    getElement("openingMenuBackground")->hide();
    mouseContainer->hide();
    Entropy::getSingletonPtr()->startNewGame();
}

void GameMenu::continueGameM()
{
    mode = Gameplay;
    getElement("openingMenu")->hide();
    getElement("openingMenuBackground")->hide();
    mouseContainer->hide();
    Entropy::getSingletonPtr()->continueGame();
}

int GameMenu::getMenuState()
{
    return mode;
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> GameMenu* Ogre::Singleton<GameMenu>::ms_Singleton = 0;
GameMenu* GameMenu::getSingletonPtr(void)
{
    return ms_Singleton;
}
GameMenu& GameMenu::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> GameMenu* Ogre::Singleton<GameMenu>::msSingleton = 0;
GameMenu* GameMenu::getSingletonPtr(void)
{
    return msSingleton;
}
GameMenu& GameMenu::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
