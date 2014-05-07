#ifndef TESTALGORITHM_H
#define TESTALGORITHM_H

#include "algorithm.h"

namespace Degenerator {

class TestAlgorithm: public Algorithm
{
public:
    TestAlgorithm();

    AStruct generateMesh(std::vector<std::string> params);
};

}

#endif // TESTALGORITHM_H
