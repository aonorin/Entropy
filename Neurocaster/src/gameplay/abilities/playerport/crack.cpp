#include "crack.h"

using namespace neuro;

Crack::Crack(Square pos, int starting_turn, int progenitor_id, int target_id) :
    PlayerPortProgram(pos,starting_turn,14,"Crack",progenitor_id,target_id,0,0)
{
}

void Crack::complete_action()
{
    std::cout << "Crack is complete!" << std::endl;
}
