#include "square.h"
#include "core/Global.h"
#include "core/neurocaster.h"
#include "OGRE/OgreCamera.h"
#include "OGRE/OgreViewport.h"

Ogre::Vector3 internal_convert(double x, double y)
{

    Ogre::Vector3 vec3;
    vec3.x = (x / (double)Neurocaster::getSingletonPtr()->current_map_x) * 2 - 1;
    vec3.y = (y / (double)Neurocaster::getSingletonPtr()->current_map_y) * 2 - 1;

    double width = Neurocaster::getSingletonPtr()->mCamera->getViewport()->getActualWidth();
    double height = Neurocaster::getSingletonPtr()->mCamera->getViewport()->getActualHeight();
    double aspectRatio = ((double)height)/((double)width);

    vec3.x = vec3.x * aspectRatio;
    vec3.z = 0;
    return vec3;
}

Square Square::inverseConvert(Ogre::Vector3 pos)
{
    Square inv(
                ( (pos.x) ) * (double)Neurocaster::getSingletonPtr()->current_map_x,
                ( (pos.y) ) * (double)Neurocaster::getSingletonPtr()->current_map_y
                );
    return inv;
}

Square::Square() :
    x(1),
    y(1),
    ogre_pos(internal_convert(x,y))
{

}

Square::Square(double x, double y) :
    x(x),
    y(y),
    ogre_pos(internal_convert(x,y))
{

}

Ogre::Vector3 Square::convert()
{
    ogre_pos = internal_convert(x,y);
    return ogre_pos;
}

Ogre::Vector2 Square::toOgre() const
{
    return Ogre::Vector2(x, y);
}

Ogre::Vector3 Square::relative_convert()
{
    Ogre::Vector3 vec3;
    vec3.x = (x / (double)Neurocaster::getSingletonPtr()->current_map_x) * 2;
    vec3.y = (y / (double)Neurocaster::getSingletonPtr()->current_map_y) * 2;
    double width = Neurocaster::getSingletonPtr()->mCamera->getViewport()->getActualWidth();
    double height = Neurocaster::getSingletonPtr()->mCamera->getViewport()->getActualHeight();
    double aspectRatio = ((double)height)/((double)width);
    vec3.x = vec3.x * aspectRatio;
    vec3.z = 0;
    return vec3;
}

void Square::setX(double x)
{
    this->x = x;
    ogre_pos = internal_convert(x,y);
}

void Square::setY(double y)
{
    this->y = y;
    ogre_pos = internal_convert(x,y);
}

double Square::getX()
{
    return x;
}

double Square::getY()
{
    return y;
}

bool Square::int_equals(Square pos)
{
    if( round(this->x) == round(pos.getX()) )
    {
        if( round(this->y) == round(pos.getY()) )
        {
            return true;
        }
    }

    return false;
}

Square Square::rounded()
{
    return Square(round(x),round(y));
}
