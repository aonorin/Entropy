#include "square.h"

namespace neuro {

Square::Square() :
    c(32),
    x(0),
    y(0),
    status(UNEXPLORED)
{

}

Square::Square(int c,int x, int y) :
    c(c),
    x(x),
    y(y),
    status(UNEXPLORED),
    pos(Vec2i(x,y))
{

}

void Square::draw()
{

}

char Square::getDrawChar()
{

    return c;
}

void Square::setChar(int c)
{
    this->c = c;
}

void Square::setStatus(STATUS status)
{
    this->status = status;
}

void Square::setPos(Vec2i pos)
{
    this->x = pos.x;
    this->y = pos.y;
}

Vec2i Square::getPos()
{
    return Vec2i(x,y);
}

void Square::setX(int x)
{
    this->x = x;
}

void Square::setY(int y)
{
    this->y = y;
}

int Square::getChar()
{
    try{
        return c;
    } catch(...) {
        return BLANK;
    }
}

int Square::getX()
{
    return x;
}

int Square::getY()
{
    return y;
}

STATUS Square::getStatus()
{
    return status;
}

}
