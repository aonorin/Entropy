#pragma once

#include <stdlib.h>
//#include <time.h>
//#include <vector>
//#include <list>
//#include <algorithm>
//#include <stdio.h>
//#include <iostream>
#include "neuro/square.h"

namespace neuro {

enum GameState{InProgress,Lost,Won};

static int nrand(int min, int max) {
    return ((rand()%(max-min+1)) + min);
}

static int MAPSIZEX = 48;
static int MAPSIZEY = 40;

static bool checkMapBounds(Vec2i pos) {
    if(
            (pos.x>0&&pos.x<MAPSIZEX)&&
            (pos.y>0&&pos.y<MAPSIZEY)
            ) {
        return true;
    } else {
        return false;
    }
}

static bool checkMapBounds(Vec2f pos) {
    if(
            (pos.x>0&&pos.x<MAPSIZEX)&&
            (pos.y>0&&pos.y<MAPSIZEY)
            ) {
        return true;
    } else {
        return false;
    }
}

enum Direction{UP,DOWN,LEFT,RIGHT};
}
