#ifndef LOG_H
#define LOG_H

#include <Ogre.h>
#include <OgreSingleton.h>
#include "OgreTextAreaOverlayElement.h"
#include "logfilemanager.h"

class EntropyLog : public Ogre::Singleton<EntropyLog>
{
public:
    EntropyLog();
    ~EntropyLog();

    enum LogStatus{Reading,NotReading};
    void openLog(std::string logName);
    void closeLog();
    void changeLog();
    void nextPage();
    void prevPage();
    int getStatus();
    static EntropyLog& getSingleton(void);
    static EntropyLog* getSingletonPtr(void);
    void mouseMoved(float x,float y);
    void mouseClicked(float x, float y);

private:
    Ogre::OverlayManager*    overlayMgr;
    Ogre::Overlay*           logOverlay;
    Ogre::Overlay*           logBackgroundOverlay;

    Ogre::OverlayContainer* backgroundArea;
    Ogre::OverlayElement* logBackground;

    Ogre::OverlayContainer* foregroundArea;
    Ogre::OverlayElement* nextArrow;
    Ogre::OverlayElement* prevArrow;
    Ogre::TextAreaOverlayElement* logText;

    LogStatus status;
    int numPages;
    int currentPage;
    std::vector<std::string> pages;
    LogFileManager* lfm;
};

#endif // LOG_H
