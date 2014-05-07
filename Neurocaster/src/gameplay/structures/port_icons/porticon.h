#ifndef PORTICON_H
#define PORTICON_H

#include "gameplay/neuron.h"

namespace neuro
{

class PortIcon : public NeuroManualObject
{
public:
    PortIcon(Square pos);
    void init_manual();
};

class PlayerPortIcon : public NeuroManualObject
{
public:
    PlayerPortIcon(Square pos);
    void init_manual();
};

}

#endif // PORTICON_H
