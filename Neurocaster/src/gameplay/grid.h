#ifndef GRID_H
#define GRID_H

#include "neuron.h"

namespace neuro
{

class Grid : public NeuroManualObject
{
public:
    typedef boost::shared_ptr<Grid> Ptr;
    Grid();

    void init_manual();
};

}

#endif // GRID_H
