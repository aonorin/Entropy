#include "algoedit.h"

using namespace voxel;

AlgoEdit::AlgoEdit(Ogre::Vector3 dimensions, Ogre::Vector3 pos, int seed) :
    Edit(false),
    dimensions(dimensions),
    pos(pos),
    seed(seed),
    abb(Ogre::AxisAlignedBox(pos.x,pos.y,pos.z,pos.x+dimensions.x,pos.y+dimensions.y,pos.z+dimensions.z))
{
}

double AlgoEdit::calcVoxel(const Ogre::Vector3 &pos, const double &scale)
{

}

Ogre::AxisAlignedBox AlgoEdit::aab(double scale) const
{
    Ogre::AxisAlignedBox aab2(abb.getMinimum()/scale, abb.getMaximum()/scale);
    Ogre::AxisAlignedBox result (aab2.getMinimum()-Ogre::Vector3(1,1,1), aab2.getMaximum()+Ogre::Vector3(1,1,1));
    return result;
}
