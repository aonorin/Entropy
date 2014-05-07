#ifndef QUESTITEM_H
#define QUESTITEM_H

#include <string>

namespace Ogre {
    class OverlayManager;
    class Overlay;
    class TextAreaOverlayElement;
    class OverlayElement;
    class OverlayContainer;
}

class QuestEntry
{

public:
    enum Quests{
        FindResearcher,
        RadioBase,
        ObtainCompound
    };
    QuestEntry(int id);
    ~QuestEntry();
    int getID();
    std::string getName();
    void setPos(int pos);
    void activate();
    void deactivate();
    bool isActive();
    void hilight();
    void unhilight();
    bool isInsideElement(float x, float y);

private:
    bool activated;
    int rows;
    Ogre::OverlayManager*   overlayMgr;
    Ogre::OverlayContainer* questContainer;
    Ogre::OverlayElement* questElement;
    Ogre::TextAreaOverlayElement* questDesc;
    int id;
    int pos;
    std::string name;
    std::string iconName;
    std::string description;
};

#endif // QUESTITEM_H
