#include "virus.h"

namespace neuro {

Virus::Virus()
{
    this->c = VIRUS;
    x = 0;
    y = 0;
    status = EXPLORED;
}


Virus::Virus(int c,int x, int y)
{
    this->c = c;
    this->x = x;
    this->y = y;
     status = EXPLORED;
}

}
