#include "hackability.h"

using namespace neuro;

HackAbility::HackAbility(Square pos, int starting_turn, int turns, std::string name, int progenitor_id, int target_id) :
    NeuroManualObject(pos,name),
    Ability(name,starting_turn,turns,progenitor_id,target_id),
    complete(false)
{
}

void HackAbility::update_turn(double turn_number)
{
    if(turn_number > starting_turn + turns)
    {
        complete = true;
        complete_action();
    }
}

bool HackAbility::isCompleted()
{
    return complete;
}
