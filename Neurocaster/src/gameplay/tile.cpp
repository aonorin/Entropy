#include "tile.h"

using namespace neuro;

Tile::Tile(int character, Square pos) :
    Entity(),
    character(character),
    pos(pos)
{
}

int Tile::getChar()
{
    return character;
}

void Tile::setChar(int character)
{
    this->character = character;
}

int Tile::getX()
{
    return pos.getX();
}

int Tile::getY()
{
    return pos.getY();
}

void Tile::setX(int x)
{
    pos.setX(x);
}

void Tile::setY(int y)
{
    pos.setY(y);
}

void Tile::setPos(Square pos)
{
    this->pos = pos;
}

Square Tile::getPos()
{
    return pos;
}

void Tile::setStatus(Loc_Status status)
{
    this->status = status;
}

Tile::Loc_Status Tile::getStatus()
{
    return status;
}
