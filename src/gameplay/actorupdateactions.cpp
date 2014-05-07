// Local includes
#include "actorupdateactions.h"
#include "src/Physics/physicsobject.h"
#include "src/gameplay/actors.h"
#include "Core/global.h"
namespace gameplay {

entropy::utility::AStar3D astar(MAPSIZE_X,MAPSIZE_Y,MAPSIZE_Z,40,1,entropy::FloatArrayPtr_t());

/////////////////////
// ActorUpdateAction
/////////////////////

ActorUpdateAction::ActorUpdateAction(physics::PhysicsActor *actor) :
    actor(actor)
{

}

void ActorUpdateAction::update()
{

}

UpdateActionType ActorUpdateAction::type()
{
    return AbstractAction;
}

void ActorUpdateAction::setActor(physics::PhysicsActor* actor)
{
    this->actor = actor;
}

void ActorUpdateAction::newActorUpdateAction(std::string actorName)
{
    physics::PhysicsActor* actor = actors::getActor(actorName);

    if(actor)
    {
        ActorUpdateAction::ptr aua(new ActorUpdateAction(actor));
         actor->addUpdateAction(aua);
    }
}

///////////////////
// PathFindUpdate
///////////////////

PathFindUpdate::PathFindUpdate(physics::PhysicsActor *actor, int targetID) :
    ActorUpdateAction(actor),
    targetID(targetID)
{

}

PathFindUpdate::PathFindUpdate(physics::PhysicsActor *actor, physics::PhysicsActor *target) :
    ActorUpdateAction(actor),
    targetID(target->getPathID())
{

}

void PathFindUpdate::update()
{
    // DO PATH FINDING HERE
    if(actor)
    {
//        std::cout << "PathFindUpdate::update()" << std::endl;
        actor->move(targetID);
    }
}

UpdateActionType PathFindUpdate::type()
{
    return PathFindAction;
}

void PathFindUpdate::setTarget(int targetID)
{
    this->targetID = targetID;
}

void PathFindUpdate::setTarget(physics::PhysicsActor *target)
{
    this->targetID = target->getPathID();
}

// Use static methods like this, bind to Lua, to create new actions
void PathFindUpdate::newPathFindAction(std::string actorName, std::string targetName)
{
    physics::PhysicsActor* actor = actors::getActor(actorName);
    physics::PhysicsActor* target = actors::getActor(targetName);

    if(actor && target)
    {
        ActorUpdateAction::ptr pfu(new PathFindUpdate(actor, target->getPathID()));
         actor->addUpdateAction(pfu);
    }
}

} // gameplay namespace
