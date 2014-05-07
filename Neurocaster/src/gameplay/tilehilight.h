#ifndef TILEHILIGHT_H
#define TILEHILIGHT_H

#include "neuron.h"

namespace neuro
{

class TileHilight : public NeuroManualObject
{
public:
    typedef boost::shared_ptr<TileHilight> Ptr;
    TileHilight();

    void init_manual();
};

}

#endif // TILEHILIGHT_H
