#ifndef PLAYERNODEPROGRAM_H
#define PLAYERNODEPROGRAM_H

#include "playerprogram.h"

namespace neuro
{

class PlayerNodeProgram : public PlayerProgram
{
public:
    PlayerNodeProgram(
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

#endif // PLAYERNODEPROGRAM_H
