#include "hackable.h"

using namespace neuro;

Hackable::Hackable(int character, Square pos) :
    Tile(character,pos)
{

}

void Hackable::continuous_update()
{
    for(ProgramMap::iterator it = programs.begin();it!=programs.end();++it)
    {
        (*it).second->continuous_update();
    }
}

void Hackable::update_turn(double turn_number)
{
    for(ProgramMap::iterator it = programs.begin();it!=programs.end();++it)
    {
        (*it).second->update_turn(turn_number);
    }
}

void Hackable::addProgram(int id, HackAbility::Ptr program)
{
    programs[id] = program;
}
