#ifndef PLAYERPROGRAM_H
#define PLAYERPROGRAM_H

#include "hackability.h"

namespace neuro
{

class PlayerProgram : HackAbility
{
public:
    PlayerProgram(
            Square pos,
            int starting_turn,
            int turns,
            std::string name,
            int progenitor_id,
            int target_id,
            double suspiciousness_effect,
            double life_effect
            );

    /**
     * @brief continuous_update Continuous update loop, for things like animations.
     */
    void continuous_update() {}

    /**
     * @brief update_action This called for each turn of completion of the hackability.
     */
    void update_action() {}

    /**
     * @brief complete_action
     */
    virtual void complete_action() = 0;

protected:
    double suspiciousness_effect;
    double life_effect;
    bool finished;
};

}

#endif // PLAYERPROGRAM_H
