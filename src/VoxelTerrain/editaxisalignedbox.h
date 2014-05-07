#ifndef EDITAXISALIGNEDBOX_H
#define EDITAXISALIGNEDBOX_H

#include "edit.h"
#include "OGRE/OgreAxisAlignedBox.h"

namespace voxel {

class EditAxisAlignedBox : public Edit
{
public:
    typedef boost::shared_ptr<EditAxisAlignedBox> ptr;

    EditAxisAlignedBox(Ogre::AxisAlignedBox abb, bool cut);

    Ogre::AxisAlignedBox aab(double scale) const;
    double calcVoxel(const Ogre::Vector3 &pos, const double &scale);
protected:
    static Ogre::AxisAlignedBox scaleAab(const Ogre::AxisAlignedBox& aab, const double scale);

    Ogre::AxisAlignedBox m_abb;
};

}

#endif // EDITAXISALIGNEDBOX_H
