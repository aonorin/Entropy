#ifndef ABILITY_H
#define ABILITY_H

#include <string>
#include "gameplay/updateable.h"

//#ifdef __APPLE__
//#include <tr1/memory>
//#else
#include <boost/shared_ptr.hpp>
//#endif

namespace neuro
{

/**
 * @brief The Ability class
 * This class represents all timed abilities in the game world.
 * This includes everything from movement, to hacking, to attacking
 */
class Ability : public Updateable
{
public:
//#ifdef __APPLE__
//    typedef std::tr1::shared_ptr<Ability> Ptr;
//#else
    typedef boost::shared_ptr<Ability> Ptr;
//#endif

    enum Type
    {
        INFECTION,
        CRYPTOGRAPHY,
        CYBERNETICS
    };

    enum SubType
    {
        WHITE_HAT,
        BLACK_HAT
    };

    /**
     * @brief Ability
     * @param name The name of the ability
     * @param turns The number of turns the ability takes to complete.
     * @param progenitor_id The id of the entity that created this ability.
     * @param target_id The id of the entity that is the target of this ability.
     */
    Ability(std::string name,int starting_turn,int turns,int progenitor_id, int target_id);

    /**
     * @brief update_turn This is received from the main update loop.
     * @param turn_number This is the OVERALL turn number.
     * It is converted internally by the ability class, which keeps track of which turn the ability was created on.
     */
    void update_turn(double turn_number);

    /**
     * @brief continuous_update Continuous update loop, for things like animations
     */
    virtual void continuous_update() = 0;

    /**
     * @brief update_action Called after each turn of completion.
     */
    virtual void update_action() = 0;

    /**
     * @brief complete_action Called when the ability completes.
     */
    virtual void complete_action() = 0;

    /**
     * @brief type The basic ability type of this ability
     * @return The Ability::Type enum value
     */
    virtual const Type type() const = 0;

    const std::string& getName() const;

protected:
    /**
     * @brief name The name of the ability.
     */
    std::string name;

    /**
     * @brief progress The progress from of the ability, ranging from 0 to 1.
     */
    double progress;

    /**
     * @brief start_turn The turn the ability was created on.
     */
    double starting_turn;

    /**
     * @brief turns The number of turns till the ability completes.
     */
    int turns;

    /**
     * @brief complete Whether or not the ability has completed.
     */
    bool complete;

    /**
     * @brief progenitor_id The id of the enitity the Ability is called by.
     */
    int progenitor_id;

    /**
     * @brief target_id The id of the entity the Ability is targeting.
     */
    int target_id;
};

}

#endif // ABILITY_H
