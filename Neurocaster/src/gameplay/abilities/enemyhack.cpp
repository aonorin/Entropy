#include "enemyhack.h"

using namespace neuro;

EnemyHack::EnemyHack(Square pos, int starting_turn, int turns, std::string name, int progenitor_id, int target_id) :
    HackAbility(pos,starting_turn,turns,name,progenitor_id,target_id)
{
}
