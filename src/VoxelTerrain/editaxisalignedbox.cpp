#include "editaxisalignedbox.h"

namespace voxel {

EditAxisAlignedBox::EditAxisAlignedBox(Ogre::AxisAlignedBox abb, bool cut)
    : Edit(cut)
    , m_abb(abb)
{
}

Ogre::AxisAlignedBox EditAxisAlignedBox::aab(double scale) const
{
    Ogre::AxisAlignedBox aab = scaleAab(m_abb, scale);
    Ogre::AxisAlignedBox result (aab.getMinimum()-Ogre::Vector3(1,1,1), aab.getMaximum()+Ogre::Vector3(1,1,1));
    return result;
}

double EditAxisAlignedBox::calcVoxel(const Ogre::Vector3 &pos, const double &scale)
{
    Ogre::AxisAlignedBox aab = scaleAab(m_abb, scale);
    if (aab.contains(pos))
        return 1.;
    else
        return -2.;
}

Ogre::AxisAlignedBox EditAxisAlignedBox::scaleAab(const Ogre::AxisAlignedBox& aab, const double scale)
{
    return Ogre::AxisAlignedBox (aab.getMinimum()/scale, aab.getMaximum()/scale);
}

}
