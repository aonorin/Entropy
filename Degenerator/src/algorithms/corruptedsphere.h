#ifndef CORRUPTEDSPHERE_H
#define CORRUPTEDSPHERE_H

#include "algorithm.h"

namespace Degenerator {


class CorruptedSphere: public Algorithm
{
public:
    CorruptedSphere();

    AStruct generateMesh(std::vector<std::string> params);
};

} // Degenerator

#endif // CORRUPTEDSPHERE_H
