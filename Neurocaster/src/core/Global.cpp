#include "Global.h"

namespace neuro
{

int x_square_mul;
int y_square_mul;
int MAPSIZEX = 48;
int MAPSIZEY = 40;

inline static bool checkMapBounds(Square pos) {
    if(
            (pos.getX()>0&&pos.getX()<MAPSIZEX)&&
            (pos.getY()>0&&pos.getY()<MAPSIZEY)
            ) {
        return true;
    } else {
        return false;
    }
}

unsigned int num_unique = 0;

std::string get_unique_name(std::string name)
{
    std::stringstream ss;
    ss << name << num_unique++;
    return ss.str();
}

}
