#ifndef COLLISIONGROUPMANAGER_H
#define COLLISIONGROUPMANAGER_H

// System includes
#include <string>
#include <vector>

// Entropy includes
#include "src/Physics/physicstypes.h"

namespace physics {

// Static class for managing CollisionGroup definitions and number mapping
class CollisionGroupManager
{
public:

    static void createDefaultCollisionGroups();
    static collision_group_id_t addCollisionGroup(const std::string& groupName);
    // DON'T REMOVE GROUPS DURING GAMEPLAY BECAUSE WILL INVALIDATE ALL GROUP INTs AND COMBINATIONS
    static void removeCollisionGroup(const std::string& groupName);
    // DON'T REMOVE GROUPS DURING GAMEPLAY BECAUSE WILL INVALIDATE ALL GROUP INTs AND COMBINATIONS
    static void clearCollisionGroups();
    static bool getCollisionGroupInt(std::string groupName, collision_group_id_t& collisionGroupInt);
    // Returns the bitOr of the group ints for the two groups.
    // Used for creating/removing collision events in EventDispatcher
    static bool getCollisionCombination(std::string group1, std::string group2,
                                        collision_group_id_t& collisionCombinationInt);
    static bool getCollisionCombination(collision_group_id_t& collisionGroupInt0,
                                        collision_group_id_t& collisionGroupInt1,
                                        collision_group_id_t& collisionCombinationInt);
    static bool containsCollisionGroup(std::string groupName, collision_group_id_t& index);
    static bool containsCollisionGroup(collision_group_id_t collisionGroupInt);

    static collision_group_id_t defaultGroupID;

private:

    static std::vector<std::string> collisionGroups;
};

} // physics namespace

#endif // COLLISIONGROUPMANAGER_H
