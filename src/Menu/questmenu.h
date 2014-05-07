#ifndef QUESTMENU_H
#define QUESTMENU_H

#include <OGRE/OgreSingleton.h>
#include <vector>
#ifndef WIN32
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif
#include "boost/thread/locks.hpp"

//Forward declerations
namespace Ogre {
    class OverlayManager;
    class OverlayContainer;
    class OverlayElement;
    class TextAreaOverlayElement;
}

struct QuestMenuItem {
    std::string name;
    std::string parent;
    std::string description;
};

struct QuestMenuEntry {
    std::string name;
    std::string description;
    std::vector<QuestMenuItem> items;
};

class QuestMenu : public Ogre::Singleton<QuestMenu>
{
public:
    QuestMenu();
    ~QuestMenu();

    void change();
    void open();
    void close();
    bool isOpen();

    static QuestMenu& getSingleton(void);
    static QuestMenu* getSingletonPtr(void);

    void addQuestIcon(std::string questName, std::string description);
    void addQuestItemIcon(std::string questItemName, std::string parent, std::string description);
    void removeQuestIcon(std::string questName);
    void removeQuestItemIcon(std::string questItemName, std::string parent);

    void addQueuedQuests();
    void addQueuedQuestItems();

private:
    void organizeEntries();
    bool opened;
    Ogre::OverlayManager*   overlayMgr;
    Ogre::Overlay*   questOverlay;
    Ogre::OverlayContainer* questContainer;
    Ogre::OverlayContainer* questDescContainer;

    std::vector<QuestMenuEntry > queuedQuests;
    std::vector<QuestMenuItem > queuedQuestItems;
    std::vector<QuestMenuEntry > quests;

   boost::shared_mutex questMutex;
};

#endif // QUESTMENU_H
