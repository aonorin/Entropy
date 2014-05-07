#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include "src/ArtificialLife/abstractca.h"

namespace alife {

namespace cell {

///////////////
// GameOfLife
///////////////

class GameOfLife : public AbstractGrid
{
public:

    GameOfLife(unsigned int width, unsigned int height, bool wrapping = false);

    virtual void update();
    static void updateExternalGrid(std::vector< std::vector<unsigned int> >* grid, bool wrapping = false);

    unsigned int width;
    unsigned int height;
    bool wrapping;

    static const unsigned int numNeighbors = 8;

protected:

    virtual unsigned int activeNeighbors(int x, int y);
    static unsigned int externalActiveNeighbors(int x, int y, std::vector< std::vector<unsigned int> >* grid, bool wrapping);
    void initializeGrid();
};


////////////////
// GameOfLife3D
////////////////

struct GameOfLifeRules
{
    // Numbers of neighbors for death, survival, and birth;
    unsigned int minDeath, maxDeath, minBirth, maxBirth;
    GameOfLifeRules(unsigned int minDeath = 6, unsigned int maxDeath = 11, unsigned int minBirth = 9, unsigned int maxBirth = 11) :
        minDeath(minDeath),
        maxDeath(maxDeath),
        minBirth(minBirth),
        maxBirth(maxBirth){}
};

class GameOfLife3D : public AbstractGrid
{
public:

    GameOfLife3D(unsigned int width, unsigned int height, unsigned int depth, bool wrapping = false,
                 GameOfLifeRules rules = GameOfLifeRules());

    virtual void update();
    void setOneEvery(unsigned int numSkip, unsigned int value = 1);
    const GameOfLifeRules& getRules();
    void setRules(GameOfLifeRules rules);

    unsigned int width, height, depth;
    bool wrapping;
    static const unsigned int numNeighbors = 26;

protected:

    unsigned int activeNeighbors(int neighborX, int neighborY, int neighborZ);
    void initializeGrid();

    GameOfLifeRules rules;
};
} // cell namespace

} // alife namespace

#endif // GAMEOFLIFE_H
