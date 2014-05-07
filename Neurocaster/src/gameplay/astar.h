#ifndef ASTAR_H
#define ASTAR_H

//#include "boost/multi_array.hpp"
#include <vector>
#include <deque>
#include <queue>
#include <boost/smart_ptr.hpp>
#include "boost/unordered_map.hpp"
//#include "src/gameplay/entity.h"
#include "tile.h"
//#include "src/core/Global.h"
#include "src/gameplay/map.h"

namespace neuro
{

namespace astar
{

/**
 * @brief The Heuristic enum Defines various heuristics to be used with the A* Path finding algorithm. Manhattan is fast, but less accurate
 * DiagonalShortcut is slightly slower but more accurate, DiagonalHate rates generally diagnol movement much lower, and Euclidean is expensive but accurate
 */
enum Heuristic
{
    Manhattan,
    DiagonalShortcut,
    DiagonalHate,
    Euclidean
};

/**
 * @brief walkability_t typedef of walkability, it is simply a double. 0 is completely walkable, -1 is unpassable, higher than 0 incurs extra movement costs
 */
typedef double walkability_t;

/**
 * @brief FREE_WALK Value of freely walkable tiles. They incur no extra cost.
 */
const walkability_t FREE_WALK = 0;

/**
 * @brief UNWALKABLE Value of unwalkable tiles. They cannot be reached.
 */
const walkability_t UNWALKABLE = -1;

/**
 * @brief path_t Final path result type definition.
 */
typedef std::deque<Ogre::Vector2> path_t;

/**
 * @brief walk_map_t maps object types to walkability
 */
typedef boost::unordered::unordered_map<Tile::Type, walkability_t> walk_map_t;

/**
 * @brief initializeWalkMaps Initializes the default walk maps, which includes the HERO and VIRUS map currently
 */
void initializeWalkMaps();

/**
 * @brief setWalkMap Up4dates a walk map to a new set of mappings
 * @param tileObjectType Object type to update
 * @param walkMap The new walkability mappings
 */
void setWalkMap(Tile::Type tileObjectType, const walk_map_t& walkMap);

/**
 * @brief pathFind A simple implementation of the A* path finding algorithm. Given an origin, target, and object type, finds a path, if possible.
 * @param tileMap A reference to the Map to be used for the path finding function
 * @param origin The coordinates of the path finder.
 * @param target The coordinates of the object towards which we are finding a path.
 * @param finderType The type of the path finder object, ie, Hero, Enemy, etc...
 * @param result The path found, if any, by the A* algorithm. Supply your own path (&myPath), to obtain the result
 * @param heuristic The Heuristic function used to calculate fCosts for nodes. This greatly impacts the efficiency, accuracy, and shape of the algorithm.
 * @param diagonalMovement Flag declaring that diagonal movement is or is not allowed in the path.
 * @param checkTargetWalkable Flag to determine whether the algorithm will check the walkability of the target, exiting immediately if it is not.
 * @return Returns Returns true if a path was found.
 */
bool pathFind(Map::Ptr tileMap, const Ogre::Vector2& origin, const Ogre::Vector2& target, const Tile::Type& finderType,
              path_t* result, Heuristic heuristic = DiagonalShortcut, bool diagonalMovement = true, bool checkTargetWalkable = true);

} // astar namespace

} // neuro namespace

#endif // ASTAR_H
