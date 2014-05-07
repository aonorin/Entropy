#ifndef NEUROAI_H
#define NEUROAI_H

#include "neuro/square.h"
#include "Global.h"
#include <string>

namespace Ogre {
    class OverlayContainer;
}

namespace neuro{

static int aid = 0;

class NeuroAI
{
public:
    enum AiStatus{Patrolling,Attacking,ScanningPort,ScanningNode,HackingPort,HackingNode,Protecting,Walking,AtDestination,Alarmed,Idle};
    NeuroAI();
    NeuroAI(Vec2f pos,std::string iconName);
    ~NeuroAI();

    void move();
    void setNextPoint(Vec2i nextPoint);
    bool checkForUpdate();
    Vec2f getPos();
    Vec2i getIPos();
    int getID();
    int getTargetID();
    void setTargetID(int tid);

    virtual void attack();
    virtual void update();
    virtual void updateProjectiles();
    void makeFriendly();

    //Status
    int getStatus();
    void setStatus(int status);
    int getMetaStatus();
    void setMetaStatus(int metaStatus);

protected:
    int status;
    int metaStatus;
    Vec2f pos;
    Vec2i nextPoint;
    Vec2i prevPoint;
    float updateR;
    int id;
    int targetID;
    float health;
    float speed;
    Direction facing;
//    std::vector<Projectile* > projectiles;
    bool enemy;
    Ogre::OverlayContainer* aiIcon;
    std::string iconName;
    void setupGUI();
};

}

#endif // NEUROAI_H
