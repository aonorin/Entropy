#ifndef WOLFRAHAMCA_H
#define WOLFRAHAMCA_H

#ifndef WIN32
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

#include "src/ArtificialLife/abstractca.h"

namespace alife {

namespace cell {

class WolframCA : public AbstractStream
{

public:

    WolframCA(unsigned int ruleSet = 0, unsigned int possibleStates = 2, unsigned int initialState = 1,
              unsigned int size = 1025);

    void advance();

protected:

};

typedef std::vector< std::vector<unsigned int> > slice_t; // One vertical slice of the cell, which is a 2D grid

class WolframCA3D : public AbstractAutomata // 3D version of the WolframCA, takes 2D sheets and builds up
{
public:
    WolframCA3D(unsigned int ruleSet = 0, unsigned int possibleStates = 2, unsigned int initialState = 1,
                unsigned int width = 50, unsigned int depth = 50);

    void advance();
    void randomizeCurrentState();
    const slice_t& getCurrentState();

    unsigned int size;
    unsigned int ruleArraySize;

protected:

    void calculateRuleArraySize();
    void createRuleArray();

    unsigned int width, depth;
    std::tr1::unordered_map<double, unsigned int> ruleMap;
    std::vector<double> ruleAverages; // Collection of averages for CA with more than 3 possible states
    std::vector<unsigned int> ruleArray;
    slice_t futureState, currentState;
};

} // cell namespace

} // alife namespace

#endif // WOLFRAHAMCA_H
