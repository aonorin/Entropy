#ifndef ACTORUPDATEACTIONS_H
#define ACTORUPDATEACTIONS_H

// system includes
#include <string>
#include "Utility/astar3d.h"

namespace physics {
    class PhysicsActor;
}

namespace gameplay {

enum UpdateActionType // Purposely enumerated for clarity. These are referred to by number for call from Lua
{
    AbstractAction = 0,
    PathFindAction = 1
};

extern entropy::utility::AStar3D astar;

class ActorUpdateAction
{
public:
    typedef boost::shared_ptr<ActorUpdateAction> ptr;
    ActorUpdateAction(physics::PhysicsActor* actor = 0);

    virtual void update(); // Overload this!
    virtual UpdateActionType type(); // Overload this!
    void setActor(physics::PhysicsActor *actor);

    // Use static methods like this, bind to Lua, to create new actions
    static void newActorUpdateAction(std::string actorName); // Don't use this, just an example

protected:

    physics::PhysicsActor* actor;
};

class PathFindUpdate : public ActorUpdateAction
{
public:

    PathFindUpdate(physics::PhysicsActor* actor = 0, int targetID = 0);
    PathFindUpdate(physics::PhysicsActor* actor = 0, physics::PhysicsActor* target = 0);

    void update();
    UpdateActionType type();
    void setTarget(int targetID);
    void setTarget(physics::PhysicsActor* target);

    // Use static methods like this, bind to Lua, to create new actions
    static void newPathFindAction(std::string actorName, std::string targetName);

private:

    int targetID;
};

} // gameplay namespace

#endif // ACTORUPDATEACTIONS_H
