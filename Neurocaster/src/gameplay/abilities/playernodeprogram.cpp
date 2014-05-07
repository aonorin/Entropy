#include "playernodeprogram.h"

using namespace neuro;

PlayerNodeProgram::PlayerNodeProgram(Square pos, int starting_turn, int turns, std::string name, int progenitor_id, int target_id, double suspiciousness_effect, double life_effect) :
    PlayerProgram(pos,starting_turn,turns,name,progenitor_id,target_id,suspiciousness_effect,life_effect)
{
}
