#ifndef SQUARE_H
#define SQUARE_H

#include "OGRE/OgreVector2.h"
#include "OGRE/OgreVector3.h"

/**
 * @brief The Square class
 * This represents locations of things in Neurocaster on a 2x2 grid.
 */
class Square
{
public:
    Square();

    /**
     * @brief Square
     * @param x The x position of the square on the map grid.
     * @param y The y position of the square on the map grid.
     */
    Square(double x, double y);

    /**
     * @brief convert Converts this square to an Ogre::Vector2 coordinate.
     * @return
     */
    Ogre::Vector3 convert();

    Ogre::Vector3 relative_convert();

    Ogre::Vector2 toOgre() const;

    /**
     * @brief getX Get the x coordinate of the square on the grid.
     * @return
     */
    double getX();

    /**
     * @brief getY Get the y coordinate of the square on the grid.
     * @return
     */
    double getY();

    /**
     * @brief setX Set the x coordinate of the Square, on the grid.
     * @param x
     */
    void setX(double x);

    /**
     * @brief setY Set the y coordinate of the Square on the grid.
     * @param y
     */
    void setY(double y);

    /**
     * @brief x The x coordinate of the Square on the grid.
     */
    double x;

    /**
     * @brief y The y coordinate of the Square on the grid.
     */
    double y;

    bool int_equals(Square pos);

    Square rounded();

    static Square inverseConvert(Ogre::Vector3 pos);

private:

    /**
     * @brief ogre_pos The precomputed conversion of this square to an Ogre::Vector2.
     */
    Ogre::Vector3 ogre_pos;
};

#endif // SQUARE_H
