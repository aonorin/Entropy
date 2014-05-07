#ifndef NODEICON_H
#define NODEICON_H

#include "gameplay/neuron.h"

namespace neuro
{

class NodeIcon : public NeuroManualObject
{
public:
    NodeIcon(Square pos);
    void init_manual();
};

class PlayerNodeIcon : public NeuroManualObject
{
public:
    PlayerNodeIcon(Square pos);
    void init_manual();
};

}

#endif // NODEICON_H
