#ifndef VIRUS_H
#define VIRUS_H

#include "neuro/square.h"

namespace neuro{

class Virus : public Square
{
public:
    Virus();
    Virus(int c,int x,int y);
};

}
#endif // VIRUS_H
