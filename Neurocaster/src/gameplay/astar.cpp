#include "boost/unordered_set.hpp"
#include "boost/unordered_map.hpp"
#include "astar.h"

namespace neuro
{

namespace astar
{

// A very simple hash function for a Vector2. It collapses 2D coordinates to a 1D coordinate. Since the tile maps can't have two entries for any location, this is fast hasher.
unsigned int hashCoord(const Ogre::Vector2& coord, Map::Ptr tileMap)
{
    return coord.x + (coord.y * tileMap->getWidth());
}

/**
 * @brief The PathNode class Describes a single Node in a map. Contains several important bits of information for path finding and reconstruction
 */
class PathNode
{
public:
    PathNode() :
        coord(Ogre::Vector2::ZERO),
        fCost(0),
        gCost(0),
        hash(0),
        type(Tile::BLANK),
        parentHash(0)
    {

    }

    /**
     * @brief PathNode Constructor for Path Node.
     * @param coord The TileMap coordinate for this Node.
     * @param tileMap The TileMap where that this PathNode references. Used to generate hash key and Tile::Object type
     */
    PathNode(const Ogre::Vector2& coord, Map::Ptr tileMap) :
        coord(coord),
        fCost(0),
        gCost(0)

    {
        hash = hashCoord(coord, tileMap);
        parentHash = hash; // declare itself as the parent, this signifies it had no previous node, implying it is the origin
        type = (Tile::Type) tileMap->getTile(coord)->getChar();
    }

    /**
     * @brief PathNode Constructor for Path Node.
     * @param coord The TileMap coordinate for this Node.
     * @param tileMap The TileMap where that this PathNode references. Used to generate hash key and Tile::Object type
     * @param previousNode The node before this in the path finding process. Used to store the parent hash which enables path reconstruction at the end
     */
    PathNode(const Ogre::Vector2& coord, Map::Ptr tileMap, const PathNode& previousNode) :
        coord(coord),
        fCost(0),
        gCost(0),
        parentHash(previousNode.hash)

    {
        hash = hashCoord(coord, tileMap);
        type = (Tile::Type) tileMap->getTile(coord)->getChar();
        gCost = previousNode.gCost + coord.distance(previousNode.coord); // gCost is cumulative, effectively storing the gCost of the path up to this node
    }

    /**
     * @brief coord The coordinate of this Path Node in the TileMap
     */
    Ogre::Vector2 coord;

    /**
     * @brief gCost Running distance based cost, establishes a running cost for itself and previous nodes that preceeded it in a given path.
     */
    walkability_t gCost;

    /**
     * @brief fCost A complimentary cost that takes into consideration the specified Heuristic function
     */
    walkability_t fCost;

    /**
     * @brief hash Used for closed and open set hashing, enables fast look up and path reconstruction
     */
    unsigned int hash;

    /**
     * @brief type Tile type which effects walkability and move cost
     */
    Tile::Type type;

    /**
     * @brief parentHash Hash key for the parent object, facillitates final path reconstruction
     */
    unsigned int parentHash;
};

/**
 * @brief The CompareNodes class A simple functor class to be used in conjunction with std::priority_queue. used to sort the open set in the A* algorithm
 */
class CompareNodes
{
public:

    /**
     * @brief operator () Functor operator. Walk cost functor used in priority_queue_t for ordering nodes. We want the Node with the least fCost to be on the top.
     * @param a Object A in the comparison
     * @param b Object B in the comparison
     * @return Returns true if a.fCost < b.fCost
     */
    bool operator ()(const PathNode& a, const PathNode& b) const
    {
        return a.fCost > b.fCost;
    }

};

/**
 * @brief priority_queue_t The priority queue used during the A* path finding algorithm. Uses PathNodes with the deque class, utilizing CompareNodes for sorting
 */
typedef std::priority_queue<PathNode, std::deque<PathNode>, CompareNodes> priority_queue_t;

typedef boost::unordered::unordered_map<Tile::Type, walk_map_t> meta_walk_map_t; // contains all the currently define walk maps
typedef boost::unordered::unordered_set<unsigned int> coord_set_t; // stores hashes for PathNodes
typedef boost::unordered::unordered_map<unsigned int, PathNode> coord_map_t; // maps hashes to hash nodes, used for path reconstruction
typedef walkability_t (*heuristic_func) (const Ogre::Vector2& origin, const Ogre::Vector2& target); // function definition for heuristic functions

// The global walkability mappings
meta_walk_map_t meta_walk_map;
const double SQRT_TWO = 1.41421356237309504880;
const double SQRT_TWO_TIMES_TWO = SQRT_TWO * 2;
const double INTERESTING_PATH_MULTIPLIER = 2.0; // makes slightly more interesting paths than just squares or super diagonal land

// simple but efficient, not very accurate
walkability_t manhattan(const Ogre::Vector2& origin, const Ogre::Vector2& target)
{
    return abs(origin.x - target.x) + abs(origin.y - target.y);
}

// Slightly less efficient, but slightly more accurate
walkability_t diagonal_shortcut(const Ogre::Vector2& origin, const Ogre::Vector2& target)
{
    Ogre::Vector2 distance(abs(origin.x - target.x), abs(origin.y - target.y));

    if(distance.x > distance.y)
    {
         return SQRT_TWO * distance.y + (distance.x - distance.y);
    }

    else
    {
        return SQRT_TWO * distance.x + (distance.y - distance.x);
    }
}

// Emphasizes lateral movement over generally diagonal movement
walkability_t diagonal_hate(const Ogre::Vector2& origin, const Ogre::Vector2& target)
{
    Ogre::Vector2 distance(abs(origin.x - target.x), abs(origin.y - target.y));

    if(distance.x > distance.y)
    {
         return INTERESTING_PATH_MULTIPLIER * distance.y + (distance.x - distance.y);
    }

    else if(distance.y > distance.x)
    {
        return INTERESTING_PATH_MULTIPLIER * distance.x + (distance.y - distance.x);
    }

    else
    {
        return distance.x / 2;
    }
}

// Least efficient but most accurate
walkability_t euclidean(const Ogre::Vector2& origin, const Ogre::Vector2& target)
{
    return sqrt(pow((origin.x - target.x), 2) + pow((origin.y - target.y), 2));
}

// initializes the walk maps for various types. This allows the path finding algorithm to traverse a tile map differently for different finder types such as the hero or a virus.
void initializeWalkMaps()
{
    walk_map_t mapGenWalkMap;

    mapGenWalkMap[Tile::BLANK] = FREE_WALK; // 0 is completely passable
    mapGenWalkMap[Tile::FLOOR] = UNWALKABLE;
    mapGenWalkMap[Tile::HALLWAY] = FREE_WALK;
    mapGenWalkMap[Tile::CORNER] = UNWALKABLE;
    mapGenWalkMap[Tile::TOPCORNER] = UNWALKABLE;
    mapGenWalkMap[Tile::SIDEWALL] = UNWALKABLE;
    mapGenWalkMap[Tile::TOPWALL] = UNWALKABLE;
    mapGenWalkMap[Tile::HERO] = UNWALKABLE;
    mapGenWalkMap[Tile::VIRUS] = UNWALKABLE;
    mapGenWalkMap[Tile::IO] = UNWALKABLE;
    mapGenWalkMap[Tile::SCANNER] = UNWALKABLE;
    mapGenWalkMap[Tile::DATA] = UNWALKABLE;
    mapGenWalkMap[Tile::CONTROL] = UNWALKABLE;
    mapGenWalkMap[Tile::SPAWNER] = UNWALKABLE;
    mapGenWalkMap[Tile::WETWARE] = UNWALKABLE;
    mapGenWalkMap[Tile::ENEMYPORT] = FREE_WALK;
    mapGenWalkMap[Tile::PORT] = FREE_WALK;
    mapGenWalkMap[Tile::NODE] = UNWALKABLE;
    mapGenWalkMap[Tile::ENEMYNODE] = UNWALKABLE;

    meta_walk_map[Tile::MAPGEN] = mapGenWalkMap;

    walk_map_t heroWalkMap;

    heroWalkMap[Tile::BLANK] = UNWALKABLE; // -1 is unpassable
    heroWalkMap[Tile::FLOOR] = FREE_WALK; // 0 is completely passable
    heroWalkMap[Tile::HALLWAY] = FREE_WALK;
    heroWalkMap[Tile::CORNER] = UNWALKABLE;
    heroWalkMap[Tile::TOPCORNER] = UNWALKABLE;
    heroWalkMap[Tile::SIDEWALL] = UNWALKABLE;
    heroWalkMap[Tile::TOPWALL] = UNWALKABLE;
    heroWalkMap[Tile::HERO] = FREE_WALK;
    heroWalkMap[Tile::VIRUS] = FREE_WALK;
    heroWalkMap[Tile::IO] = FREE_WALK;
    heroWalkMap[Tile::SCANNER] = FREE_WALK;
    heroWalkMap[Tile::DATA] = FREE_WALK;
    heroWalkMap[Tile::CONTROL] = FREE_WALK;
    heroWalkMap[Tile::SPAWNER] = FREE_WALK;
    heroWalkMap[Tile::WETWARE] = FREE_WALK;
    heroWalkMap[Tile::ENEMYPORT] = UNWALKABLE;
    heroWalkMap[Tile::PORT] = FREE_WALK;
    heroWalkMap[Tile::NODE] = FREE_WALK;
    heroWalkMap[Tile::ENEMYNODE] = FREE_WALK;

    meta_walk_map[Tile::HERO] = heroWalkMap;

    walk_map_t virusWalkMap;

    virusWalkMap[Tile::BLANK] = UNWALKABLE; // -1 is unpassable
    virusWalkMap[Tile::FLOOR] = FREE_WALK; // 0 is completely passable
    virusWalkMap[Tile::HALLWAY] = FREE_WALK;
    virusWalkMap[Tile::CORNER] = UNWALKABLE;
    virusWalkMap[Tile::TOPCORNER] = UNWALKABLE;
    virusWalkMap[Tile::SIDEWALL] = UNWALKABLE;
    virusWalkMap[Tile::TOPWALL] = UNWALKABLE;
    virusWalkMap[Tile::HERO] = FREE_WALK;
    virusWalkMap[Tile::VIRUS] = FREE_WALK;
    virusWalkMap[Tile::IO] = FREE_WALK;
    virusWalkMap[Tile::SCANNER] = FREE_WALK;
    virusWalkMap[Tile::DATA] = FREE_WALK;
    virusWalkMap[Tile::CONTROL] = FREE_WALK;
    virusWalkMap[Tile::SPAWNER] = FREE_WALK;
    virusWalkMap[Tile::WETWARE] = FREE_WALK;
    virusWalkMap[Tile::ENEMYPORT] = FREE_WALK;
    virusWalkMap[Tile::PORT] = FREE_WALK;
    virusWalkMap[Tile::NODE] = FREE_WALK;
    virusWalkMap[Tile::ENEMYNODE] = FREE_WALK;

    meta_walk_map[Tile::SENTRY] = virusWalkMap;
}

// Associate a Tile::Object with a walk_map_t, allowing for real time updates to a walkability map
void setWalkMap(Tile::Type tileObjectType, const walk_map_t &walkMap)
{
    meta_walk_map[tileObjectType] = walkMap;
}

bool setContainsNode(unsigned int hash, const coord_set_t& set)
{
    return set.find(hash) != set.end();
}

// Reconstructs the path starting from the target and working backwards to the origin. Each node stores both it's own hash and the hash of the previous node in the path.
// We recursively crawl the map by searching for parent nodes. If we find a node whose parent node's hash is identical to it's own, we've found the origin.
void reconstructPath(const coord_map_t& cameFrom, const PathNode& node, path_t* path)
{
    path->push_front(node.coord);

    if(node.hash == node.parentHash || cameFrom.find(node.parentHash) == cameFrom.end()) // If the node.hash == node.parentHash, we've found the origin.
        return;

    return reconstructPath(cameFrom, cameFrom.at(node.parentHash), path); // tail call
}

// Returns the walkability of a coordinate based on a tileMap (which gives us the Tile::Object type) and a walk_map (which maps that type to a walkability rating)
walkability_t getWalkability(const Ogre::Vector2& coord, Map::Ptr tileMap, const walk_map_t& walk_map)
{
    walk_map_t::const_iterator find = walk_map.find((Tile::Type) tileMap->getTile(coord)->getChar());

    if(find == walk_map.end())
    {
        std::cerr << "Walk mapping not found in walk map: " << (unsigned char) tileMap->getTile(coord)->getChar() << std::endl;
        return UNWALKABLE;
    }

    else
    {
        return find->second;
    }
}

bool isCorner(const Ogre::Vector2& direction)
{
    if(direction.x == 0 || direction.y == 0)
        return false;

    return true;
}

// Returns the walkability of a trajector based on a tileMap and walk_map. Consider corner cutting and walkability mappings.
bool isWalkable(const Ogre::Vector2& from, const Ogre::Vector2& to, Map::Ptr tileMap, const walk_map_t& walk_map)
{
    if(to.x >= 0 && to.y >= 0 && to.x < tileMap->getWidth() && to.y < tileMap->getHeight()) // Check within bounds
    {
        if(getWalkability(to, tileMap, walk_map) != UNWALKABLE) // Make sure the to vector is walkable
        {
            const Ogre::Vector2 direction = to - from;

            if(isCorner(direction)) // check corner cutting
            {
                if(getWalkability(from + Ogre::Vector2(direction.x, 0), tileMap, walk_map) == UNWALKABLE
                        || getWalkability(from + Ogre::Vector2(0, direction.y), tileMap, walk_map) == UNWALKABLE)
                    return false; // false indicates corner cutting
            }

            return true; // target is walkable and we're not cutting corners
        }
    }

    return false; // outside of bounds or target unwalkable
}

void debug_print(Map::Ptr tileMap, const Ogre::Vector2& origin, const Ogre::Vector2& target, const priority_queue_t& openSet, const coord_set_t& closedSet, const coord_map_t& cameFrom)
{
    unsigned char print_map[tileMap->getWidth()][tileMap->getHeight()];

    for(unsigned int x = 0; x < tileMap->getWidth(); ++x)
    {
        for(unsigned int y = 0; y < tileMap->getHeight(); ++y)
        {
            print_map[x][y] = Tile::BLANK;
        }
    }

    for(coord_map_t::const_iterator iter = cameFrom.begin(); iter != cameFrom.end(); ++iter)
    {
        const PathNode& node = iter->second;
        print_map[(unsigned int) node.coord.x][(unsigned int) node.coord.y] = 'F';
    }

    for(coord_set_t::const_iterator iter = closedSet.begin(); iter != closedSet.end(); ++iter)
    {
        unsigned int hash = *iter;
        Ogre::Vector2 node(hash % tileMap->getWidth(), (unsigned int) hash / (unsigned int) tileMap->getWidth());
        print_map[(unsigned int) node.x][(unsigned int) node.y] = 'C';
    }

    PathNode currentNode = openSet.top();
    print_map[(unsigned int) currentNode.coord.x][(unsigned int) currentNode.coord.y] = 'O';
    print_map[(unsigned int) origin.x][(unsigned int) origin.y] = 'S';
    print_map[(unsigned int) target.x][(unsigned int) target.y] = 'T';

    std::cout << "////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;
    std::cout << "// A* PathFind debug print" << std::endl;
    std::cout << "////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << std::endl;

    for(unsigned int x = 0; x < tileMap->getWidth(); ++x)
    {
        for(unsigned int y = 0; y < tileMap->getHeight(); ++y)
        {
            std::cout << print_map[x][y];
        }

        std::cout << std::endl;
    }
}

bool pathFind(Map::Ptr tileMap, const Ogre::Vector2& origin, const Ogre::Vector2& target, const Tile::Type& finderType, path_t* result, Heuristic heuristic, bool diagonalMovement,
              bool checkTargetWalkable)
{
    // determine the walk_map we're using
    const walk_map_t& walk_map = meta_walk_map.at(finderType);

    ///////////////////////////////////////////////
    // Check initial conditions for immediate exit

    // If we're already at the target, path is already complete. Return true.
    if(origin == target)
        return true;

    // If the target is unwalkable, the path cannot be found. Return false.
    if(checkTargetWalkable)
    {
        if(getWalkability(target, tileMap, walk_map) == UNWALKABLE)
        {
            std::cout << "ASTAR: Target is unwalkable." << std::endl;
            return false;
        }
    }


    // Initialized variables for path finding
    coord_set_t closedSet; // Set of visited Path Nodes
    priority_queue_t openSet; // Set of Path Nodes to visit
    coord_set_t openSetMembers; // Fast lookup for presence in openSet
    coord_map_t cameFrom; // Used to reconstruct path, starting from the target and working backwards
    heuristic_func calc_heuristic; // The heuristic function used to calculate fCost for the Path Nodes

    // determin heuristic function and assign to calc_heuristic
    switch(heuristic)
    {
    case DiagonalShortcut:
        calc_heuristic = diagonal_shortcut;
        break;

    case DiagonalHate:
        calc_heuristic = diagonal_hate;
        break;

    case Euclidean:
        calc_heuristic = euclidean;
        break;

    case Manhattan:
    default:
        calc_heuristic = manhattan;
        break;
    }

    // push the origin into the priority queue
    PathNode currentNode(origin, tileMap); // Create the current node at the origin
    currentNode.fCost = currentNode.gCost + calc_heuristic(origin, target);  // calculate fCost as gCost plus the result of the heuristic function
    openSet.push(currentNode); // push into open set
    openSetMembers.insert(currentNode.hash); // add to the openSetMember set for fast look up of presence in the openSet priority queue
    cameFrom[currentNode.hash] = currentNode;

    while(openSet.size())
    {
        currentNode = openSet.top();
        openSet.pop();
        openSetMembers.erase(currentNode.hash);
        // std::cout << "Current Node: " << currentNode.coord << ", target node: " << target << std::endl;

        closedSet.insert(currentNode.hash); // Since we haven't yet found the target, add the current node to the closed set to prevent further lookup

        // For each neighbor
        for(int x = -1; x <= 1; ++x) // scan x neighbors
        {
            for(int y = -1; y <= 1; ++y) // scan y neighbors
            {
                if(!diagonalMovement)
                    if(isCorner(Ogre::Vector2(x, y)))
                        continue; // don't move diagonally if flag is false

                Ogre::Vector2 newCoord = currentNode.coord + Ogre::Vector2(x, y);

                if((x == 0 && y == 0) || !isWalkable(currentNode.coord, newCoord, tileMap, walk_map))
                    continue; // don't evaluate the current node or unwalkable nodes

                PathNode neighbor(newCoord, tileMap, currentNode);

                if(neighbor.coord == target) // If we've found the target, reconstruct the path by pushing nodes into the front of the path_t
                {
                    reconstructPath(cameFrom, neighbor, result);
                    return true; // return true, signifying that a path was successfully found
                }

                // If walkable, not in the closed set, and not in the open set
                if(!setContainsNode(neighbor.hash, closedSet))
                {
                    if(setContainsNode(neighbor.hash, openSetMembers))
                        if(neighbor.gCost >= cameFrom.at(neighbor.hash).gCost)
                            continue;

                    neighbor.fCost = neighbor.gCost + calc_heuristic(newCoord, target);
                    cameFrom[neighbor.hash] = neighbor;
                    openSet.push(neighbor);
                    openSetMembers.insert(neighbor.hash);
                } // is walkable && not in closedSet && not in openSet

                // debug_print(tileMap, origin, target, openSet, closedSet, cameFrom);

            } // scan y neigbords
        } // scan x neghbors
    } // while open set has nodes

    std::cout << "ASTAR: No path found.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    return false; // Unable to find a path
} // pathFind function

} // astar namespace

} // neuro namespace
