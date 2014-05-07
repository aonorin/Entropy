#include "console.h"
#include "src/Core/update.h"
#include "src/Script/script.h"
#include "src/Menu/consolefilemanager.h"
#include "src/Core/Entropy.h"
#include <OGRE/Ogre.h>
#include "OGRE/OgreTextAreaOverlayElement.h"

namespace SLB { class EntropyScript; }

void printLua(SLB::EntropyScript *s, const char *str, size_t length)
{
    //    std::cout << str << std::endl;
    Console::getSingletonPtr()->print(str);
}


Console::Console() :
    opened(false),
    shift(false)
{
    std::cout << "pre new ConsoleFileManager();" << std::endl;
    new ConsoleFileManager();
    if(Entropy::getSingletonPtr()->doesFileExist("SavedConsoleCode.txt")) {
        std::cout << "DOES EXIST" << std::endl;
        //        consoleFile = ConsoleFileManager::getSingletonPtr()->load("SavedConsoleCode.txt", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        //        std::vector<std::string> consoleLines = consoleFile->getLines();
        consoleFile = (ConsoleFilePtr) new ConsoleFile("SavedConsoleCode.txt");
        consoleFile->load();
        std::vector<std::string> consoleLines = consoleFile->getLines();
        for(int i=0;i<consoleLines.size();i++) {
            if(consoleLines.at(i).size() > 0)
                savedStrings.push_back(consoleLines.at(i));
        }
    } else {
        std::cout << "DOESN'T EXIST" << std::endl;
        consoleFile = (ConsoleFilePtr) new ConsoleFile("SavedConsoleCode.txt");
    }

    savedNum = 0;
    pureXPos = 0;
    overlayMgr = Ogre::OverlayManager::getSingletonPtr();

    consoleOverlay = overlayMgr->create("consoleOverlay");
    consoleOverlay->setZOrder(299);
    consoleOverlay->setScale(1,1);
    consoleOverlay->hide();

    consoleCursorOverlay = overlayMgr->create("consoleCursorOverlay");
    consoleCursorOverlay->setZOrder(300);
    consoleCursorOverlay->setScale(1,1);
    consoleCursorOverlay->hide();

    Ogre::MaterialPtr consoleMat = Ogre::MaterialManager::getSingleton().create("consoleMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    consoleMat->getTechnique(0)->getPass(0)->createTextureUnitState("InventoryBlackout.png");
    consoleMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    consoleMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    consoleMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    consoleMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr consoleCursorMat = Ogre::MaterialManager::getSingleton().create("consoleCursorMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    //    consoleCursorMat->getTechnique(0)->getPass(0)->createTextureUnitState("InventoryBlackout.png");
    consoleCursorMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    consoleCursorMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    consoleCursorMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    consoleCursorMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_ONE_MINUS_DEST_COLOUR, Ogre::SBF_ZERO);

    int width = Entropy::getSingletonPtr()->mWindow->getWidth();
    int height = Entropy::getSingletonPtr()->mWindow->getHeight();
    fontWidth = 13.98;
    float tmpWrap = ((float)width)/fontWidth;
    charWrap = ((int)tmpWrap)-2;
    fontHeight = 24;
    numLines = 13;

    consoleContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "consoleContainer"));
    consoleContainer->setMetricsMode(Ogre::GMM_PIXELS);
    consoleContainer->setHorizontalAlignment(Ogre::GHA_LEFT);
    consoleContainer->setVerticalAlignment(Ogre::GVA_TOP);
    consoleContainer->setDimensions(width,numLines*fontHeight);
    consoleContainer->setPosition(0,-1*numLines*fontHeight);
    consoleContainer->show();
    consoleContainer->setMaterialName("consoleMat");
    consoleOverlay->add2D(consoleContainer);

    consoleCursorContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "consoleCursorContainer"));
    consoleCursorContainer->setMetricsMode(Ogre::GMM_PIXELS);
    consoleCursorContainer->setHorizontalAlignment(Ogre::GHA_LEFT);
    consoleCursorContainer->setVerticalAlignment(Ogre::GVA_TOP);
    consoleCursorContainer->setDimensions(width,numLines*fontHeight);
    consoleCursorContainer->setPosition(0.0,-1*numLines*fontHeight);
    consoleCursorContainer->show();
    //    consoleCursorContainer->setMaterialName("consoleCursorMat");
    consoleCursorOverlay->add2D(consoleCursorContainer);

    consoleText = (Ogre::TextAreaOverlayElement*)overlayMgr->createOverlayElement("TextArea","consoleText");
    consoleText->setMetricsMode(Ogre::GMM_PIXELS);
    consoleText->setDimensions(width,height);
    consoleText->setPosition(0,0);
    consoleText->setFontName("ocra");
    consoleText->setCharHeight(24);
    consoleText->setAlignment(Ogre::TextAreaOverlayElement::Left);
    consoleText->setColour(Ogre::ColourValue(1.0,1.0,1,1.0));
    consoleContainer->addChild(consoleText);
    consoleText->show();

    consoleCursor = overlayMgr->createOverlayElement("Panel", "consoleCursor");
    consoleCursor->setMetricsMode(Ogre::GMM_PIXELS);
    consoleCursor->setHorizontalAlignment(Ogre::GHA_LEFT);
    consoleCursor->setVerticalAlignment(Ogre::GVA_TOP);
    //    consoleCursor->setDimensions(0.027*0.5,0.0225);
    consoleCursor->setDimensions(fontWidth,fontWidth*1.5);
    cursorXPos = 3;
    cursorYPos = 0;
    consoleCursor->setPosition(cursorXPos*fontWidth,12*fontHeight);
    consoleCursor->show();
    consoleCursor->setMaterialName("consoleCursorMat");
    consoleCursorContainer->addChild(consoleCursor);
    consoleCursor->show();

    consoleString.push_back(consBreak);
    consoleText->setCaption("> ");
    //    inputString = consBreak+"$ ";
    wrapNum = 128;
    for(int i=0;i<12;i++) {
        outputString.append("\n");
    }
    outputString += ">: ";
    script::setPrintCallback(printLua);
}

Console::~Console()
{
    consoleFile->clearLines();
    for(int i=0;i<savedStrings.size();i++) {
        consoleFile->addLine(savedStrings.at(i));
    }
    consoleFile->save();
}

void Console::shiftDown()
{
    shift = true;
}

void Console::shiftUp()
{
    shift = false;
}

void Console::enterText(EntKeyCode key)
{
    if(key == KC_RETURN || key == KC_NUMPADENTER) {
        returnKey();
    } else if(key == KC_DELETE || key == KC_BACK){
        deleteText();
    } else if(key == KC_LEFT){
        if(pureXPos>0) {
            pureXPos-=1;
            cursorXPos-=1;
            consoleCursor->setPosition(cursorXPos*fontWidth,(12+cursorYPos)*fontHeight);
        }
    } else if(key == KC_RIGHT){
        if(pureXPos<executeString.size()) {
            pureXPos+=1;
            cursorXPos+=1;
            consoleCursor->setPosition(cursorXPos*fontWidth,(12+cursorYPos)*fontHeight);
        }
    } else if(key == KC_UP) {
        //        std::cout << "savedStrings.size: " << savedStrings.size() << std::endl;
        //        std::cout << "savedNum" << savedNum << std::endl;
        if(savedStrings.size()>0) {
            if(savedNum<savedStrings.size()-1) {
                executeString = savedStrings.at(savedNum);
                pureXPos = executeString.size();
                cursorXPos = 3+executeString.size();
                inputString = executeString;
                processOutputString();
                savedNum +=1;
            } else {
                executeString = savedStrings.at(savedNum);
                pureXPos = executeString.size();
                cursorXPos = 3+executeString.size();
                processOutputString();
            }
        }
    } else if(key == KC_DOWN){
        if(savedNum>0) {
            savedNum-=1;
            executeString = savedStrings.at(savedNum);
            pureXPos = executeString.size();
            cursorXPos = 3 + executeString.size();
            inputString = executeString;
            processOutputString();
        } else {
            savedNum = 0;
            executeString.clear();
            pureXPos = 0;
            cursorXPos = 3;
            inputString.clear();
            processOutputString();
        }
    } else {
        //        executeString+=keyCodeToChar(event,shift);
        executeString.insert(pureXPos,std::string(1,keyCodeToChar(key,shift)));
        pureXPos+=1;
        inputString+=keyCodeToChar(key,shift);
        cursorXPos+=1;
        if(cursorXPos>charWrap) {
            cursorXPos = 1;
        }
        processOutputString();
    }
}

void Console::processOutputString()
{
    consoleCursor->setPosition(cursorXPos*fontWidth,(12+cursorYPos)*fontHeight);
    int lines = executeString.size()/charWrap;
    for(int i=0;i<executedStrings.size();i++){
        lines+=(executedStrings.at(i).size()/charWrap)+1;
    }
    outputString.clear();
    for(int i=0;i<executedStrings.size();i++){
        if(executedStrings.at(i).size()>=charWrap) {
            outputString += characterWrap(executedStrings.at(i),charWrap);
        } else {
            outputString += executedStrings.at(i)+"\n";
        }
    }
    outputString += characterWrap(">: "+executeString,charWrap);
    if(lines<12) {
        for(int i=lines;i<12;i++) {
            outputString.insert(0,"\n");
        }
    } else {
        std::vector<std::string> returnstrs;
        boost::split(returnstrs,outputString, boost::is_any_of("\n"));
        outputString.clear();
        for(int i=lines-12;i<=lines;i++) {
            outputString+=returnstrs.at(i)+"\n";
        }
    }
    consoleText->setCaption(outputString);
}

void Console::deleteText()
{
    if(inputString.size()>0) {
        //        executeString.erase(executeString.end()-1);
        executeString.erase(pureXPos-1,1);
        pureXPos-=1;
        cursorXPos-=1;
        if(cursorXPos<3) {
            cursorXPos = charWrap+3;
        }
        processOutputString();
    }
}

void Console::returnKey()
{
    savedNum = 0;
    script::interpret(executeString.c_str());
    executedStrings.push_back(">: "+executeString);
    savedStrings.push_front(executeString);
    executeString.clear();
    cursorXPos = 3;
    cursorYPos = 0;
    pureXPos=0;
    processOutputString();
}

void Console::print(const char *chars)
{
    std::string string(chars);
    if(string.size()>1) {
        executedStrings.push_back(string);
        processOutputString();
    }
}

void Console::change()
{
    if(opened) {
        close();
    } else {
        open();
    }
}

void Console::open()
{
    opened = true;
    cursorXPos = 3;
    cursorYPos = 0;
    pureXPos=0;
    consoleCursor->setPosition(cursorXPos*fontWidth,12*fontHeight);
    executeString.clear();
    inputString.clear();
    //outputString.clear();
    //executedStrings.clear();
    //for(int i=0;i<12;i++) {
    //    outputString.append("\n");
    //}
    //outputString += ">: ";
    savedNum = 0;
    executeString.clear();
    pureXPos = 0;
    cursorXPos = 3;
    processOutputString();
    consoleText->setCaption(outputString);
    consoleOverlay->show();
    consoleCursorOverlay->show();
    boost::function<void (void)> func = boost::bind(&Console::done,this);
    boost::function<void (float)> update = boost::bind(&Console::animateConsole,this,_1);
    Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(200),func,update);
}

void Console::close()
{
    opened = false;
    boost::function<void (void)> func = boost::bind(&Console::done,this);
    boost::function<void (float)> update = boost::bind(&Console::animateConsole,this,_1);
    Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(200),func,update);
    consoleFile->clearLines();
    for(int i=0;i<savedStrings.size();i++) {
        consoleFile->addLine(savedStrings.at(i));
    }
    consoleFile->save();
}

bool Console::isOpen()
{
    return opened;
}

bool Console::isClosed()
{
    return !opened;
}

void Console::animateConsole(float progress)
{
    float cProgress;
    if(opened) {
        cProgress = 1-progress;
    } else {
        cProgress = progress;
    }
    consoleContainer->setPosition(
                0,
                (-1*numLines*fontHeight)+(numLines*fontHeight*cProgress)
                //                (-0.4+(0.4*cProgress))*Entropy::getSingletonPtr()->mWindow->getWidth()
                );
    consoleCursorContainer->setPosition(
                0,
                (-1*numLines*fontHeight)+(numLines*fontHeight*cProgress)
                //                (-0.4+(0.4*cProgress))*Entropy::getSingletonPtr()->mWindow->getWidth()
                );
}

void Console::done()
{
    if(!opened) {
        consoleOverlay->hide();
        consoleCursorOverlay->hide();
    }
}


#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> Console* Ogre::Singleton<Console>::ms_Singleton = 0;
Console* Console::getSingletonPtr(void)
{
    return ms_Singleton;
}
Console& Console::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> Console* Ogre::Singleton<Console>::msSingleton = 0;
Console* Console::getSingletonPtr(void)
{
    return msSingleton;
}
Console& Console::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
