#include "entity.h"

using namespace neuro;

int current_id = 0;

Entity::Entity() :
    id(current_id++)
{

}

int Entity::getID()
{
    return id;
}

void Entity::setID(int id)
{
    this->id = id;
}
