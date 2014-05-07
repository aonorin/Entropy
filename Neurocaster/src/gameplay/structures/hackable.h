#ifndef HACKABLE_H
#define HACKABLE_H

#include "gameplay/entity.h"
#include "gameplay/tile.h"
#include "gameplay/updateable.h"
#include "gameplay/abilities/hackability.h"

#include <map>

namespace neuro
{

class Hackable : public Tile, public Updateable
{
public:
    typedef boost::shared_ptr<Hackable> Ptr;
    typedef std::vector<Ptr> PtrArray;

    Hackable(int character, Square pos);

    void continuous_update();
    void update_turn(double turn_number);

    void addProgram(int id, HackAbility::Ptr program);

protected:
    //Organized by the entity id of who spawned it.
    typedef std::map<int,HackAbility::Ptr> ProgramMap;
    ProgramMap programs;

};

}
#endif // HACKABLE_H
