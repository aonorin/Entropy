#ifndef HALLWAY_H
#define HALLWAY_H

#include "src/gameplay/neuron.h"

namespace neuro
{

class Hallway : public NeuroManualObject
{
public:

    typedef boost::shared_ptr<Hallway> Ptr;
    typedef std::vector<Ptr> PtrArray;

    Hallway(const Square& pos);
    void init_manual();
};

} // neuro namespace

#endif // HALLWAY_H
