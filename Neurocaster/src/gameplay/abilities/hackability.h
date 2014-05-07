#ifndef HACKABILITY_H
#define HACKABILITY_H

#include "ability.h"
#include "gameplay/neuron.h"
#include "gameplay/square.h"

namespace neuro
{

/**
 * @brief The HackAbility class
 * Represents all abilities that have to do with hacking Ports and Nodes
 */
class HackAbility : public Ability, public NeuroManualObject
{
public:

    typedef boost::shared_ptr<HackAbility> Ptr;
    typedef std::vector<Ptr> PtrArray;

    /**
     * @brief HackAbility
     * @param pos The position of the hackability on the map grid, in squares
     * @param starting_turn The turn the hackability was created on.
     * @param turns The number of turns till the hackability is completed.
     * @param name The name of the hackability.
     * @param progenitor_id The entity that called the hackability.
     * @param target_id The entity the hackability targets.
     */
    HackAbility(Square pos, int starting_turn, int turns, std::string name, int progenitor_id, int target_id);

    /**
     * @brief continuous_update Continuous update loop, for things like animations.
     */
    virtual void continuous_update() = 0;

    /**
     * @brief update_action This called for each turn of completion of the hackability.
     */
    virtual void update_action() = 0;

    /**
     * @brief update_turn - Called to update the turns. Continuous between turn numbers
     * @param turn_number - The turn number, as a double, with continuous updates between the turns.
     */
    void update_turn(double turn_number) ;

    /**
     * @brief complete_action
     */
    virtual void complete_action() = 0;

    bool isCompleted();

protected:
    bool complete;
    Ogre::OverlayElement* progressBar;
    Ogre::OverlayElement* textArea;
};

}

#endif // HACKABILITY_H
