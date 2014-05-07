#ifndef LSYSTEMMESH_H
#define LSYSTEMMESH_H

#include "algorithm.h"

namespace Degenerator {

// Defaults
// F+Fv[+F-F^F]-, 0

enum LSystemArgs
{
    LRule, // String of rules characters
    LSeed // int
};

class LSystemMesh : public Algorithm
{
public:
    LSystemMesh();

    AStruct generateMesh(std::vector<std::string> params);
};

} // Degenerator namespace

#endif // LSYSTEMMESH_H
