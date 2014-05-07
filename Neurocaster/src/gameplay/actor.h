#ifndef ACTOR_H
#define ACTOR_H

#include <boost/unordered_map.hpp>

//#include "astar.h"
//#include "core/Global.h"
#include "neuron.h"
#include "entity.h"
#include "updateable.h"
#include "tile.h"

namespace neuro
{

namespace astar{

/**
 * @brief path_t Final path result type definition.
 */
typedef std::deque<Ogre::Vector2> path_t;


}

/**
 * @brief The Actor class represents moveable enemies and players.
 */
class Actor : public NeuroManualObject, public Entity, public Updateable
{
public:
    typedef boost::shared_ptr<Actor> Ptr;
    typedef std::vector<Ptr> PtrArray;

    /**
     * @brief Actor - The Actor class represents moveable enemies and players.
     * @param pos - Initial position of the Actor.
     * @param name - Name of the Actor.
     * @param type - The type of the actor (used to look up pathfinding map).
     */
    Actor(Square pos, std::string name, Tile::Type type, double speed);

    void update_turn(double turn_number);
    void continuous_update();

    void update_action() {}
    void complete_action() {}

    virtual void findPath(Square targetPos) {}

    /**
     * @brief move - Moves the actor in a direction.
     */
    virtual void move();

    /**
     * @brief setSpeed - Sets the speed of the Actor, used for movement calculations.
     * @param speed - The new speed of the Actor.
     */
    void setSpeed(double speed);

protected:

    /**
     * @brief speed - The speed of the actor, used for movement calculations.
     */
    double speed;

    neuro::astar::path_t path;
};

}

#endif // ACTOR_H
