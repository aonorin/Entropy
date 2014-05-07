#include "entropycursor.h"
#include "boost/function.hpp"
#include "src/Core/Entropy.h"
#include "src/Menu/inventory.h"
#include <OGRE/Ogre.h>
#include "Physics/physicstypes.h"
#include "Core/collisioncamera.h"

EntropyCursor::EntropyCursor() :
    cursorState(physics::NoCursor),
    isUsing(false)
{
    overlayMgr = Ogre::OverlayManager::getSingletonPtr();

    cursorOverlay = overlayMgr->create("cursorOverlay");
    cursorOverlay->setZOrder(105);
    cursorOverlay->setScale(1,1);
    cursorOverlay->hide();

    Ogre::MaterialPtr grabIconMat = Ogre::MaterialManager::getSingleton().create("grabIconMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    grabIconMat->getTechnique(0)->getPass(0)->createTextureUnitState("GrabIcon.png");
    grabIconMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    grabIconMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    grabIconMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    grabIconMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr hackIconMat = Ogre::MaterialManager::getSingleton().create("hackIconMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    hackIconMat->getTechnique(0)->getPass(0)->createTextureUnitState("HackIcon.png");
    hackIconMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    hackIconMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    hackIconMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    hackIconMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr activateIconMat = Ogre::MaterialManager::getSingleton().create("activateIconMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    activateIconMat->getTechnique(0)->getPass(0)->createTextureUnitState("ActivateIcon.png");
    activateIconMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    activateIconMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    activateIconMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    activateIconMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr lookIconMat = Ogre::MaterialManager::getSingleton().create("lookIconMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    lookIconMat->getTechnique(0)->getPass(0)->createTextureUnitState("LookIcon.png");
    lookIconMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    lookIconMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    lookIconMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    lookIconMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    cursor = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "cursor"));
    cursor->setMetricsMode(Ogre::GMM_RELATIVE);
    cursor->setHorizontalAlignment(Ogre::GHA_LEFT);
    cursor->setVerticalAlignment(Ogre::GVA_TOP);
    cursor->setDimensions(3*0.015,4*0.015);
    cursor->setPosition(0.5,0.5);
    cursor->show();
    cursor->setMaterialName("hackIconMat");
    cursor->setMaterialName("activateIconMat");
    cursor->setMaterialName("grabIconMat");
    cursor->setMaterialName("lookIconMat");
    cursorOverlay->add2D(cursor);
}

void EntropyCursor::unequip()
{
    cursorState = physics::NoCursor;
    boost::function<void (void)> func = boost::bind(&EntropyCursor::queuedSetCursor,this);
    Entropy::getSingletonPtr()->addQeueudFunction(func);
}

void EntropyCursor::setCursor(int cursorType)
{

    if(cursorState != cursorType) {
        cursorState = cursorType;
        if(!Inventory::getSingletonPtr()->isEquipped()) {
            boost::function<void (void)> func = boost::bind(&EntropyCursor::queuedSetCursor,this);
            Entropy::getSingletonPtr()->addQeueudFunction(func);
        } else {
            boost::function<void (void)> func = boost::bind(&EntropyCursor::queuedSetItemCursor,this);
            Entropy::getSingletonPtr()->addQeueudFunction(func);
        }
    }
}

void EntropyCursor::use()
{
    if(Inventory::getSingletonPtr()->isEquipped()) {
        if(CollisionCamera::getSingletonPtr()->getPickedObject())
        {
            if(!isUsing) {
                isUsing = true;
                Inventory::getSingletonPtr()->useAction(itemName,CollisionCamera::getSingletonPtr()->getPickedObject()->getObjectType());
                isUsing = false;
            }
        }
    } else {

    }
}

void EntropyCursor::unUse()
{
    isUsing = false;
}

void EntropyCursor::setCursor(std::string itemName)
{
    this->itemName = itemName;
    boost::function<void (void)> func = boost::bind(&EntropyCursor::queuedSetItemCursor,this);
    Entropy::getSingletonPtr()->addQeueudFunction(func);
}

void EntropyCursor::queuedSetItemCursor()
{
    if(cursorState==physics::NoCursor) {
        cursorOverlay->show();
        cursor->setMaterialName(itemName+"invMaterial");
    } else {
        cursorOverlay->show();
        cursor->setMaterialName(itemName+"hiinvMaterial");
    }
}

void EntropyCursor::queuedSetCursor()
{

    switch(cursorState) {
    case physics::NoCursor:
        cursorOverlay->hide();
        break;
    case physics::LookCursor:
        cursorOverlay->show();
        cursor->setMaterialName("lookIconMat");
        break;
    case physics::HackCursor:
        cursorOverlay->show();
        cursor->setMaterialName("hackIconMat");
        break;
    case physics::PickUpCursor:
        cursorOverlay->show();
        cursor->setMaterialName("grabIconMat");
        break;
    case physics::ActivateCursor:
        cursorOverlay->show();
        cursor->setMaterialName("activateIconMat");
        break;
    }
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> EntropyCursor* Ogre::Singleton<EntropyCursor>::ms_Singleton = 0;
EntropyCursor* EntropyCursor::getSingletonPtr(void)
{
    return ms_Singleton;
}
EntropyCursor& EntropyCursor::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> EntropyCursor* Ogre::Singleton<EntropyCursor>::msSingleton = 0;
EntropyCursor* EntropyCursor::getSingletonPtr(void)
{
    return msSingleton;
}
EntropyCursor& EntropyCursor::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
