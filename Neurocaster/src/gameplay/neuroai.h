#ifndef NEUROAI_H
#define NEUROAI_H

#include "actor.h"

namespace neuro
{

/**
 * @brief The NeuroAI class
 * Represents any entities that have AI.
 */
class NeuroAI : public Actor
{
public:


    typedef boost::shared_ptr<NeuroAI> Ptr;
    typedef std::vector<Ptr> PtrArray;

    /**
     * @brief NeuroAI
     * @param pos The position of the NeuroAI in squares
     * @param name The name of the NeuroAI.
     */
    NeuroAI(Square pos, std::string name,Tile::Type type, double speed);

    void update_turn(double turn_number);
    void continuous_update();

    void update_action() {}
    void complete_action() {}

    virtual void move();

    virtual void findPath(Square targetPos) {}

    virtual void init_ai(){}

    virtual void update_ai(){}
};

}

#endif // NEUROAI_H
