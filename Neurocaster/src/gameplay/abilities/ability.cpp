#include "ability.h"

using namespace neuro;

Ability::Ability(std::string name, int start_turn, int turns, int progenitor_id, int target_id) :
    name(name),
    turns(turns),
    progenitor_id(progenitor_id),
    target_id(target_id),
    progress(0),
    starting_turn(start_turn),
    complete(false)
{

}

void Ability::update_turn(double turn_number)
{
    progress = (turn_number - starting_turn) / turns;
    if( progress >= 1.0 && complete == false)
    {
        complete = true;
        complete_action();
    }
}

const std::string& Ability::getName() const
{
    return name;
}
