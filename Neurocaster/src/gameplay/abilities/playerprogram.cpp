#include "playerprogram.h"

using namespace neuro;

PlayerProgram::PlayerProgram(Square pos, int starting_turn, int turns, std::string name, int progenitor_id, int target_id, double suspiciousness_effect, double life_effect) :
    HackAbility(pos,starting_turn,turns,name,progenitor_id,target_id),
    suspiciousness_effect(suspiciousness_effect),
    life_effect(life_effect)
{
}
