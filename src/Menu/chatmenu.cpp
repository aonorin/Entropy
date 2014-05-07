#include "chatmenu.h"

#include <OGRE/Ogre.h>
#include "OGRE/OgreTextAreaOverlayElement.h"

#include "Menu/gamemenu.h"
#include "src/Core/update.h"

ChatMenu::ChatMenu() :
    opened(false),
    shift(false)
{
    overlayMgr = Ogre::OverlayManager::getSingletonPtr();

    chatOverlay = overlayMgr->create("chatOverlay");
    chatOverlay->setZOrder(99);
    chatOverlay->setScale(1,1);
    chatOverlay->hide();

    Ogre::MaterialPtr chatMat = Ogre::MaterialManager::getSingleton().create("chatMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    chatMat->getTechnique(0)->getPass(0)->createTextureUnitState("InventoryBlackout.png");
    chatMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    chatMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    chatMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    chatMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr inputchatMat = Ogre::MaterialManager::getSingleton().create("inputchatMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    inputchatMat->getTechnique(0)->getPass(0)->createTextureUnitState("InventoryBlackout.png");
    inputchatMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    inputchatMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    inputchatMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    inputchatMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE);

    chatContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "chatContainer"));
    chatContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    chatContainer->setHorizontalAlignment(Ogre::GHA_LEFT);
    chatContainer->setVerticalAlignment(Ogre::GVA_TOP);
    chatContainer->setDimensions(0.0,0.4);
    chatContainer->setPosition(0.1,0.02);
    chatContainer->show();
    chatContainer->setMaterialName("chatMat");
    chatOverlay->add2D(chatContainer);

    inputContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "inputContainer"));
    inputContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    inputContainer->setHorizontalAlignment(Ogre::GHA_LEFT);
    inputContainer->setVerticalAlignment(Ogre::GVA_TOP);
    inputContainer->setDimensions(0.0,0.1);
    inputContainer->setPosition(0.1,0.4125);
    inputContainer->show();
    inputContainer->setMaterialName("chatMat");
    chatOverlay->add2D(inputContainer);

    chatBox = (Ogre::TextAreaOverlayElement*)overlayMgr->createOverlayElement("TextArea","chatBox");
    chatBox->setMetricsMode(Ogre::GMM_RELATIVE);
    chatBox->setDimensions(0.75,0.35);
    chatBox->setPosition(0.05, 0.05);
    chatBox->setFontName("bluehigh");
    chatBox->setCharHeight(0.03);
    chatBox->setAlignment(Ogre::TextAreaOverlayElement::Left);
    chatBox->setColour(Ogre::ColourValue(0.1,0.75,1,1.0));
    chatContainer->addChild(chatBox);
    chatBox->show();

    inputBox = (Ogre::TextAreaOverlayElement*)overlayMgr->createOverlayElement("TextArea","inputBox");
    inputBox->setMetricsMode(Ogre::GMM_RELATIVE);
    inputBox->setDimensions(0.8,0.1);
    inputBox->setPosition(0.05, 0.05);
    inputBox->setFontName("bluehigh");
    inputBox->setCharHeight(0.03);
    inputBox->setAlignment(Ogre::TextAreaOverlayElement::Left);
    inputBox->setColour(Ogre::ColourValue(1,1,1,1));
    inputContainer->addChild(inputBox);
    inputBox->show();

    chatString.push_back("Virgil: Hello.");
    finalChatString += "Virgil: Hello.";
    inputString = "";

    chatBox->setCaption(finalChatString);
    inputBox->setCaption(inputString);
    wrapNum = 100;
//    chat = new ChatScript();
}

void ChatMenu::shiftDown()
{
    shift = true;
}

void ChatMenu::shiftUp()
{
    shift = false;
}

void ChatMenu::enterText(EntKeyCode key)
{
    if(key == KC_RETURN || key == KC_NUMPADENTER) {
        returnKey();
    } else if(key == KC_DELETE || key == KC_BACK){
        deleteText();
    } else {
        inputString+=keyCodeToChar(key,shift);
        inputBox->setCaption(wordWrap(inputString,wrapNum));
    }
}

void ChatMenu::botSpeak(char *text)
{
    finalChatString = "Virgil: ";
    finalChatString += std::string(text);
    chatBox->setCaption(wordWrap(finalChatString,wrapNum));
}

void ChatMenu::deleteText()
{
    if(inputString.size()>0) {
        inputString.erase(inputString.end()-1);
        inputBox->setCaption(wordWrap(inputString,wrapNum));
    }
}

void ChatMenu::returnKey()
{
//    botSpeak(chat->chat(inputString));
    inputString = "";
    inputBox->setCaption(inputString);
}

void ChatMenu::change()
{
    if(opened) {
        close();
    } else {
        open();
    }
}

void ChatMenu::open()
{
    opened = true;
    inputString = "";
    inputBox->setCaption(inputString);
    chatOverlay->show();
    boost::function<void (void)> func = boost::bind(&ChatMenu::done,this);
    boost::function<void (float)> update = boost::bind(&ChatMenu::animate,this,_1);
    Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(200),func,update);
}

void ChatMenu::close()
{
    opened = false;
    boost::function<void (void)> func = boost::bind(&ChatMenu::done,this);
    boost::function<void (float)> update = boost::bind(&ChatMenu::animate,this,_1);
    Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(200),func,update);
}

bool ChatMenu::isOpen()
{
    return opened;
}

void ChatMenu::animate(float progress)
{
    float cProgress;
    if(opened) {
        cProgress = 1-progress;
    } else {
        cProgress = progress;
    }
    chatContainer->setDimensions(0.8*cProgress,0.4);
    inputContainer->setDimensions(0.8*cProgress,0.15);
}

void ChatMenu::done()
{
    if(!opened) {
        chatOverlay->hide();
    }
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> ChatMenu* Ogre::Singleton<ChatMenu>::ms_Singleton = 0;
ChatMenu* ChatMenu::getSingletonPtr(void)
{
    return ms_Singleton;
}
ChatMenu& ChatMenu::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> ChatMenu* Ogre::Singleton<ChatMenu>::msSingleton = 0;
ChatMenu* ChatMenu::getSingletonPtr(void)
{
    return msSingleton;
}
ChatMenu& ChatMenu::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
