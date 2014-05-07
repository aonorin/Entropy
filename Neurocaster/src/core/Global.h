#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include "gameplay/square.h"

namespace neuro {


std::string get_unique_name(std::string name);

enum GameState{InProgress,Lost,Won};
enum Direction{UP,DOWN,LEFT,RIGHT};

static int nrand(int min, int max) {
    return ((rand()%(max-min+1)) + min);
}

//extern int MAPSIZEX;
//extern int MAPSIZEY;

//extern int x_square_mul;
//extern int y_square_mul;

//bool checkMapBounds(Square pos);
}

#endif // GLOBAL_H
