#ifndef PLAYERPORTPROGRAM_H
#define PLAYERPORTPROGRAM_H

#include "playerprogram.h"

namespace neuro
{

class PlayerPortProgram : public PlayerProgram
{
public:
    PlayerPortProgram(
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
     * @brief complete_action
     */
    virtual void complete_action() = 0;
};

}

#endif // PLAYERPORTPROGRAM_H
