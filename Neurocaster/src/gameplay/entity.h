#ifndef ENTITY_H
#define ENTITY_H

namespace neuro
{

/**
 * @brief The Entity class
 * This represents a basic entity in Neurocaster, with an associated unique id, which is assigned automagically.
 */
class Entity
{
public:
    Entity();

    /**
     * @brief getID Returns the id of the Entity.
     * @return
     */
    int getID();

    /**
     * @brief setID Sets the id of the Entity. This is handled automagically, so you better have a good reason for wanting to use this.
     * @param id
     */
    void setID(int id);

protected:
    /**
     * @brief id The unique id of the entity.
     */
    int id;
};

}

#endif // ENTITY_H
