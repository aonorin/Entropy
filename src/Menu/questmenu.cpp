#include "questmenu.h"
#include "Menu/gamemenu.h"
#include "Core/update.h"
#include <algorithm>
#include "OGRE/OgreTextAreaOverlayElement.h"
#include "src/Core/Entropy.h"
#include <OGRE/Ogre.h>

QuestMenu::QuestMenu() :
    opened(false)
{
    overlayMgr = Ogre::OverlayManager::getSingletonPtr();

    questOverlay = overlayMgr->create("questOverlay");
    questOverlay->setZOrder(100);
    questOverlay->setScale(1,1);
    questOverlay->hide();

    Ogre::MaterialPtr questBackgroundMat = Ogre::MaterialManager::getSingleton().create("questBackgroundMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    questBackgroundMat->getTechnique(0)->getPass(0)->createTextureUnitState("InventoryBlackout.png");
    questBackgroundMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    questBackgroundMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    questBackgroundMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    questBackgroundMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //Quest Material
    Ogre::MaterialPtr questMaterial = Ogre::MaterialManager::getSingleton().create("questMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    questMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("questIcon.png");
    questMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    questMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    questMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    questMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //hiQuest Material
    Ogre::MaterialPtr hiquestMaterial = Ogre::MaterialManager::getSingleton().create("hiquestMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    hiquestMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("questIcon.png");
    hiquestMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    hiquestMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    hiquestMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    hiquestMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_ONE);

    //QuestItem Material
    Ogre::MaterialPtr questItemMaterial = Ogre::MaterialManager::getSingleton().create("questItemMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    questItemMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("questIcon.png");
    questItemMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    questItemMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    questItemMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    questItemMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //hiQuestItem Material
    Ogre::MaterialPtr hiquestItemMaterial = Ogre::MaterialManager::getSingleton().create("hiquestItemMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    hiquestItemMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("questIcon.png");
    hiquestItemMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    hiquestItemMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    hiquestItemMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    hiquestItemMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_COLOUR, Ogre::SBF_ONE);

    questContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "questContainer"));
    questContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    questContainer->setHorizontalAlignment(Ogre::GHA_LEFT);
    questContainer->setVerticalAlignment(Ogre::GVA_TOP);
    questContainer->setDimensions(0.6,0.4);
    questContainer->setPosition(0.2,0.1);
    questContainer->show();

    questContainer->setMaterialName("questMaterial");
    questContainer->setMaterialName("hiquestMaterial");
    questContainer->setMaterialName("questItemMaterial");
    questContainer->setMaterialName("hiquestItemMaterial");

    questContainer->setMaterialName("questBackgroundMat");
    questOverlay->add2D(questContainer);

    questDescContainer = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "questDescContainer"));
    questDescContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    questDescContainer->setHorizontalAlignment(Ogre::GHA_LEFT);
    questDescContainer->setVerticalAlignment(Ogre::GVA_TOP);
    questDescContainer->setDimensions(0.6,0.4);
    questDescContainer->setPosition(0.2,0.125);
    questDescContainer->show();
    questOverlay->add2D(questDescContainer);
}

void QuestMenu::addQuestIcon(std::string questName, std::string description)
{
    QuestMenuEntry quest = {questName, description};
    queuedQuests.push_back(quest);

    std::cout << "Starting Quest" << questName << std::endl;
    boost::function<void (void)> func = boost::bind(&QuestMenu::addQueuedQuests,this);
    Entropy::getSingletonPtr()->addQeueudFunction(func);
}

void QuestMenu::addQuestItemIcon(std::string questItemName, std::string parent, std::string description)
{
    QuestMenuItem quest = {questItemName,parent,description};
    queuedQuestItems.push_back(quest);

    boost::function<void (void)> func = boost::bind(&QuestMenu::addQueuedQuestItems,this);
    Entropy::getSingletonPtr()->addQeueudFunction(func);
    func = boost::bind(&QuestMenu::organizeEntries,this);
    Entropy::getSingletonPtr()->addQeueudFunction(func);
    func = boost::bind(&std::vector<QuestMenuEntry>::clear,quests);
    Entropy::getSingletonPtr()->addQeueudFunction(func);
}

void QuestMenu::addQueuedQuests()
{
    boost::upgrade_lock<boost::shared_mutex> lock(questMutex);
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    for(int i=0;i<queuedQuests.size();i++) {
        QuestMenuEntry quest = queuedQuests.at(i);
        Ogre::OverlayElement* questElement = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel",quest.name);
        questElement->setMetricsMode(Ogre::GMM_RELATIVE);
        questElement->setDimensions(0.03,0.04);
        questElement->setPosition(0,0);
        questElement->setParameter("uv_coords","0 0 1 1");
        questElement->setMaterialName("questMaterial");
        questContainer->addChild(questElement);
        questElement->hide();

        Ogre::TextAreaOverlayElement* questDesc = (Ogre::TextAreaOverlayElement*)overlayMgr->createOverlayElement("TextArea",quest.name+"questDesc");
        questDesc->setMetricsMode(Ogre::GMM_RELATIVE);
        questDesc->setDimensions(0.1,0.9);
        questDesc->setPosition(0.05, 0.05);
        questDesc->setFontName("bluehigh");
        questDesc->setCharHeight(0.03);
        questDesc->setAlignment(Ogre::TextAreaOverlayElement::Left);
        questDesc->setColour(Ogre::ColourValue(1,1,1,0.9));
        questDesc->setCaption(quest.description);
        questDescContainer->addChild(questDesc);
        questDesc->hide();

        quests.push_back(queuedQuests.at(i));
    }
    queuedQuests.clear();
    organizeEntries();
}

void QuestMenu::addQueuedQuestItems()
{
    boost::upgrade_lock<boost::shared_mutex> lock(questMutex);
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    for(int i=0;i<queuedQuestItems.size();i++) {

        QuestMenuItem questItem = queuedQuestItems.at(i);
        Ogre::OverlayElement* questElement = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel",questItem.name+"/"+questItem.parent);
        questElement->setMetricsMode(Ogre::GMM_RELATIVE);
        questElement->setDimensions(0.015,0.02);
        questElement->setPosition(0,0);
        questElement->setParameter("uv_coords","0 0 1 1");
        questElement->setMaterialName("questMaterial");
        questContainer->addChild(questElement);
        questElement->hide();

        Ogre::TextAreaOverlayElement* questDesc = (Ogre::TextAreaOverlayElement*)overlayMgr->createOverlayElement("TextArea",questItem.name+"/"+questItem.parent+"questDesc");
        questDesc->setMetricsMode(Ogre::GMM_RELATIVE);
        questDesc->setDimensions(0.1,0.9);
        questDesc->setPosition(0.05, 0.0);
        questDesc->setFontName("bluehigh");
        questDesc->setCharHeight(0.02);
        questDesc->setAlignment(Ogre::TextAreaOverlayElement::Left);
        questDesc->setColour(Ogre::ColourValue(1,1,1,0.9));
        questDesc->setCaption(questItem.description);
        questDescContainer->addChild(questDesc);
        questDesc->hide();
        for(int i=0;i<quests.size();i++) {
            if(quests.at(i).name.compare(questItem.parent)==0) {
                quests.at(i).items.push_back(questItem);
            }
        }
    }
}

void QuestMenu::organizeEntries()
{
    //    boost::shared_lock<boost::shared_mutex> lock(questMutex);

    int entryNum = 0;
    for(int i=0;i<quests.size();i++) {
        overlayMgr->getOverlayElement(quests.at(i).name)->setPosition(0,0.015+0.05*entryNum);
        overlayMgr->getOverlayElement(quests.at(i).name)->show();
        overlayMgr->getOverlayElement(quests.at(i).name+"questDesc")->setPosition(0.03,(0.05*entryNum));
        overlayMgr->getOverlayElement(quests.at(i).name+"questDesc")->show();
        entryNum++;
        for(int i2=0;i2<quests.at(i).items.size();i2++) {
            overlayMgr->getOverlayElement(quests.at(i).items.at(i2).name+"/"+quests.at(i).items.at(i2).parent)->setPosition(0.03,0.015+0.05*entryNum);
            overlayMgr->getOverlayElement(quests.at(i).items.at(i2).name+"/"+quests.at(i).items.at(i2).parent)->show();
            overlayMgr->getOverlayElement(quests.at(i).items.at(i2).name+"/"+quests.at(i).items.at(i2).parent+"questDesc")->setPosition(0.05,(0.05*entryNum));
            overlayMgr->getOverlayElement(quests.at(i).items.at(i2).name+"/"+quests.at(i).items.at(i2).parent+"questDesc")->show();
            entryNum++;
        }
    }
}

void QuestMenu::removeQuestIcon(std::string questName)
{
    boost::upgrade_lock<boost::shared_mutex> lock(questMutex);
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    std::cout << "Removing Quest: " << questName << std::endl;
    //    questContainer->removeChild(questName);
    //    questDescContainer->removeChild(questName+"questDesc");
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(overlayMgr->getOverlayElement(questName));
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(overlayMgr->getOverlayElement(questName+"questDesc"));
    int q=1000;
    for(int i=0;i<quests.size();i++) {
        if(quests.at(i).name.compare(questName)==0) {
            q=i;
        }
    }
    if(q!=1000) {
        //        std::cout << "QuestItems size: " << quests.at(q).items.size() << std::endl;
//        for(int i2=0;i2<quests.at(q).items.size();i2++) {
//            std::cout << "Removing QuestItem: " << quests.at(q).items.at(i2).name << std::endl;
//            Entropy::getSingletonPtr()->addQeueudDeleteOverlays(overlayMgr->getOverlayElement(quests.at(q).items.at(i2).name+"/"+questName));
//            Entropy::getSingletonPtr()->addQeueudDeleteOverlays(overlayMgr->getOverlayElement(quests.at(q).items.at(i2).name+"/"+questName+"questDesc"));
//        }
        quests.erase(quests.begin()+q);
    }
    organizeEntries();
}

void QuestMenu::removeQuestItemIcon(std::string questItemName, std::string parent)
{
    boost::upgrade_lock<boost::shared_mutex> lock(questMutex);
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    std::cout << "Removing QuestItem: " << questItemName << std::endl;
    //    questContainer->removeChild(questItemName+"/"+parent);
    //    questDescContainer->removeChild(questItemName+"/"+parent+"questDesc");
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(overlayMgr->getOverlayElement(questItemName+"/"+parent));
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(overlayMgr->getOverlayElement(questItemName+"/"+parent+"questDesc"));
    int q=1000;
    for(int i=0;i<quests.size();i++) {
        if(quests.at(i).name.compare(questItemName)==0) {
            q=i;
        }
    }
    int q2=1000;
    if(q!=1000) {
        for(int i=0;i<quests.at(q).items.size();i++) {
            if(quests.at(q).items.at(i).name.compare(questItemName)==0) {
                q2 = i;
            }
        }
    }
    if(q2!=1000) {
        quests.at(q).items.erase(quests.at(q).items.begin()+q2);
    }
    organizeEntries();
}

void QuestMenu::change()
{
    if(opened) {
        close();
    } else {
        open();
    }
}

void QuestMenu::open()
{
    opened = true;
    questOverlay->show();
    //    Update::getSingletonPtr()->pause();
}

void QuestMenu::close()
{
    opened = false;
    questOverlay->hide();
    //    Update::getSingletonPtr()->unpause();
}

bool QuestMenu::isOpen()
{
    return opened;
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> QuestMenu* Ogre::Singleton<QuestMenu>::ms_Singleton = 0;
QuestMenu* QuestMenu::getSingletonPtr(void)
{
    return ms_Singleton;
}
QuestMenu& QuestMenu::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> QuestMenu* Ogre::Singleton<QuestMenu>::msSingleton = 0;
QuestMenu* QuestMenu::getSingletonPtr(void)
{
    return msSingleton;
}
QuestMenu& QuestMenu::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
