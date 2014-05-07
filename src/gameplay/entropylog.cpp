#include "entropylog.h"
#include "src/Core/gamemenu.h"
#include "logfile.h"

EntropyLog::EntropyLog():
    status(NotReading),
    currentPage(0),
    numPages(0)
{
    lfm = new LogFileManager();
    //Background Material
    Ogre::MaterialPtr logBackgroundMat = Ogre::MaterialManager::getSingleton().create("logBackgroundMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    logBackgroundMat->getTechnique(0)->getPass(0)->createTextureUnitState("glossy_button.png");
    logBackgroundMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    logBackgroundMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    logBackgroundMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    logBackgroundMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    //Arrow Material
    Ogre::MaterialPtr logArrowMat = Ogre::MaterialManager::getSingleton().create("logArrowMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    logArrowMat->getTechnique(0)->getPass(0)->createTextureUnitState("arrow_button.png");
    logArrowMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    logArrowMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    logArrowMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    logArrowMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    Ogre::MaterialPtr hilogArrowMat = Ogre::MaterialManager::getSingleton().create("hilogArrowMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    hilogArrowMat->getTechnique(0)->getPass(0)->createTextureUnitState("hi_arrow_button.png");
    hilogArrowMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    hilogArrowMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    hilogArrowMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    hilogArrowMat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    overlayMgr = Ogre::OverlayManager::getSingletonPtr();

    //Log Background
    logBackgroundOverlay = overlayMgr->create("logBackgroundOverlay");
    logBackgroundOverlay->setZOrder(200);
    logBackgroundOverlay->setScale(1,1);
    logBackgroundOverlay->hide();

    backgroundArea = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "backgroundArea"));
    backgroundArea->setMetricsMode(Ogre::GMM_RELATIVE);
    backgroundArea->setHorizontalAlignment(Ogre::GHA_LEFT);
    backgroundArea->setVerticalAlignment(Ogre::GVA_TOP);
    backgroundArea->setDimensions(1,1);
    backgroundArea->setPosition(0,0);
    logBackgroundOverlay->add2D(backgroundArea);
    backgroundArea->show();

    logBackground = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel","logBackground");
    logBackground->setMetricsMode(Ogre::GMM_RELATIVE);
    logBackground->setDimensions(0.75,0.75);
    logBackground->setPosition(0.125,0.125);
    logBackground->setParameter("uv_coords","0 0 1 1");
    logBackground->setMaterialName("logBackgroundMat");
    backgroundArea->addChild(logBackground);
    logBackground->show();

    //Log Foreground
    logOverlay = overlayMgr->create("logOverlay");
    logOverlay->setZOrder(210);
    logOverlay->setScale(1,1);
    logOverlay->hide();

    foregroundArea = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "foregroundArea"));
    foregroundArea->setMetricsMode(Ogre::GMM_RELATIVE);
    foregroundArea->setHorizontalAlignment(Ogre::GHA_LEFT);
    foregroundArea->setVerticalAlignment(Ogre::GVA_TOP);
    foregroundArea->setDimensions(1,1);
    foregroundArea->setPosition(0,0);
    logOverlay->add2D(foregroundArea);
    foregroundArea->show();

    nextArrow = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel","LogNextArrow");
    nextArrow->setMetricsMode(Ogre::GMM_RELATIVE);
    nextArrow->setDimensions(0.05,0.05);
    nextArrow->setPosition(0.905,0.5);
    nextArrow->setParameter("uv_coords","0 0 1 1");
    nextArrow->setMaterialName("logArrowMat");
    foregroundArea->addChild(nextArrow);
    nextArrow->show();

    prevArrow = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel","LogPrevArrow");
    prevArrow->setMetricsMode(Ogre::GMM_RELATIVE);
    prevArrow->setDimensions(0.05,0.05);
    prevArrow->setPosition(0.05, 0.5);
    prevArrow->setParameter("uv_coords","1 0 0 1");
    prevArrow->setMaterialName("logArrowMat");
    foregroundArea->addChild(prevArrow);
    prevArrow->show();

    logText = (Ogre::TextAreaOverlayElement*)overlayMgr->createOverlayElement("TextArea","logText");
    logText->setMetricsMode(Ogre::GMM_RELATIVE);
    logText->setDimensions(0.7,0.7);
    logText->setPosition(0.15, 0.15);
    logText->setFontName("bluehigh");
    logText->setCharHeight(0.03);
    logText->setAlignment(Ogre::TextAreaOverlayElement::Left);
    logText->setColour(Ogre::ColourValue(1,1,1,0.9));
    logText->setCaption("Testing");
    foregroundArea->addChild(logText);
    logText->show();
}

void EntropyLog::openLog(std::string logName)
{
    currentPage = 0;
    LogFilePtr logFile = lfm->load(logName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    pages = logFile->getTextPages();
    numPages = pages.size();
    logText->setCaption(pages.at(0));
    logOverlay->show();
    logBackgroundOverlay->show();
    status = Reading;
    GameMenu::getSingletonPtr()->mouseContainer->show();
}

void EntropyLog::closeLog()
{
    logOverlay->hide();
    logBackgroundOverlay->hide();
    status = NotReading;
    GameMenu::getSingletonPtr()->mouseContainer->hide();
}

void EntropyLog::changeLog()
{
    if(status==NotReading) {
        openLog("textideas.txt");
    } else {
        closeLog();
    }
}

int EntropyLog::getStatus()
{
    return status;
}

void EntropyLog::mouseMoved(float x, float y)
{
    //Check Prev Arrow
    if(
            x>=prevArrow->getLeft()&&
            x<=prevArrow->getLeft()+prevArrow->getWidth()&&
            y>=prevArrow->getTop()&&
            y<=prevArrow->getTop()+prevArrow->getHeight()
            ) {
        prevArrow->setMaterialName("hilogArrowMat");
    } else {
        prevArrow->setMaterialName("logArrowMat");
    }
    //Check Next Arrow
    if(
            x>=nextArrow->getLeft()&&
            x<=nextArrow->getLeft()+nextArrow->getWidth()&&
            y>=nextArrow->getTop()&&
            y<=nextArrow->getTop()+nextArrow->getHeight()
            ) {
        nextArrow->setMaterialName("hilogArrowMat");
    } else {
        nextArrow->setMaterialName("logArrowMat");
    }
}

void EntropyLog::mouseClicked(float x, float y)
{
    //Check Prev Arrow
    if(
            x>=prevArrow->getLeft()&&
            x<=prevArrow->getLeft()+prevArrow->getWidth()&&
            y>=prevArrow->getTop()&&
            y<=prevArrow->getTop()+prevArrow->getHeight()
            ) {
        currentPage = (currentPage-1);
        if(currentPage <0) {
            currentPage = numPages-1;
        }
        logText->setCaption(pages.at(currentPage));
    }
    //Check Next Arrow
    if(
            x>=nextArrow->getLeft()&&
            x<=nextArrow->getLeft()+nextArrow->getWidth()&&
            y>=nextArrow->getTop()&&
            y<=nextArrow->getTop()+nextArrow->getHeight()
            ) {
       currentPage = (currentPage+1)%numPages;
       logText->setCaption(pages.at(currentPage));
    }
}



#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> EntropyLog* Ogre::Singleton<EntropyLog>::ms_Singleton = 0;
EntropyLog* EntropyLog::getSingletonPtr(void)
{
    return ms_Singleton;
}
EntropyLog& EntropyLog::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> EntropyLog* Ogre::Singleton<EntropyLog>::msSingleton = 0;
EntropyLog* EntropyLog::getSingletonPtr(void)
{
    return msSingleton;
}
EntropyLog& EntropyLog::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
