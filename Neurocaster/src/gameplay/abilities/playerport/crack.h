#ifndef CRACK_H
#define CRACK_H

#include "gameplay/abilities/playerportprogram.h"

namespace neuro
{

class Crack : PlayerPortProgram
{
public:
    Crack(Square pos, int starting_turn, int progenitor_id, int target_id);

    void complete_action();
};

}

#endif // CRACK_H
