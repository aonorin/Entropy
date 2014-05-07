#ifndef ENEMYHACK_H
#define ENEMYHACK_H

#include "hackability.h"

namespace neuro
{

class EnemyHack : public HackAbility
{
public:
    EnemyHack(Square pos, int starting_turn, int turns, std::string name, int progenitor_id, int target_id);
};

}

#endif // ENEMYHACK_H
