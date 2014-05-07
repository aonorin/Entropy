// Entropy includes
#include "collisiongroupmanager.h"
#include "physicstypes.h"
#include "src/Core/collisioncamera.h"

namespace physics {

std::vector<std::string> CollisionGroupManager::collisionGroups;

void CollisionGroupManager::createDefaultCollisionGroups()
{
    VOXEL_GROUP_ID = CollisionGroupManager::addCollisionGroup(VOXEL_GROUP_NAME);
    PLAYER_GROUP_ID = CollisionGroupManager::addCollisionGroup(PLAYER_GROUP_NAME);
    EQUIPMENT_GROUP_ID = CollisionGroupManager::addCollisionGroup(EQUIPMENT_GROUP_NAME);
    MAIN_GROUP_ID = CollisionGroupManager::addCollisionGroup(MAIN_GROUP_NAME);
    SENSOR_GROUP_ID = CollisionGroupManager::addCollisionGroup(SENSOR_GROUP_NAME);
}

collision_group_id_t CollisionGroupManager::addCollisionGroup(const std::string& groupName)
{
    assert(collisionGroups.size() < MAX_COLLISION_GROUPS); // NEVER go above max groups

    collision_group_id_t index;

    if(!containsCollisionGroup(groupName, index))
    {
        collisionGroups.push_back(groupName);
        getCollisionGroupInt(groupName, index);
    }

    return index;
}

void CollisionGroupManager::removeCollisionGroup(const std::string& groupName)
{
    collision_group_id_t index = 0;

    if(containsCollisionGroup(groupName, index))
    {
        collisionGroups.erase(collisionGroups.begin() + index);
    }
}

void CollisionGroupManager::clearCollisionGroups()
{
    collisionGroups.clear();
}

bool CollisionGroupManager::getCollisionGroupInt(std::string groupName, collision_group_id_t &collisionGroupInt)
{
    collision_group_id_t index = 0;
    if(containsCollisionGroup(groupName, index))
    {
        collisionGroupInt = 1LL << index;
        return true;
    }

    else
    {
        return false;
    }
}

bool CollisionGroupManager::getCollisionCombination(std::string group1, std::string group2, collision_group_id_t& collisionCombinationInt)
{
    collision_group_id_t index1, index2;

    if(containsCollisionGroup(group1, index1) && containsCollisionGroup(group2, index2))
    {
        index1 = 1LL << index1;
        index2 = 1LL << index2;
        collisionCombinationInt = index1 | index2;
        return true;
    }

    else
    {
        return false;
    }
}

bool CollisionGroupManager::getCollisionCombination(collision_group_id_t& collisionGroupInt0,
                                                    collision_group_id_t& collisionGroupInt1,
                                                    collision_group_id_t &collisionCombinationInt)
{
    if(containsCollisionGroup(collisionGroupInt0) && containsCollisionGroup(collisionGroupInt1))
    {
        collisionCombinationInt = collisionGroupInt0 | collisionGroupInt1;
        return true;
    }

    else
    {
        return false;
    }
}

bool CollisionGroupManager::containsCollisionGroup(std::string groupName, collision_group_id_t &index)
{
    collision_group_id_t currentIndex = 0;
    std::vector<std::string>::iterator iter = collisionGroups.begin();
    while(iter != collisionGroups.end())
    {
        if(iter->compare(groupName) == 0)
        {
            index = currentIndex;
            return true;
        }

        ++currentIndex;
        ++iter;
    }

    return false;
}

bool CollisionGroupManager::containsCollisionGroup(collision_group_id_t collisionGroupInt)
{
    collision_group_id_t index = log((float)collisionGroupInt) / log(2.0f);

    if(collisionGroups.size() < index)
        return true;
    else
        return false;
}

} // physics namespace
