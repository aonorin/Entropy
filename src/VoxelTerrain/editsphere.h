#ifndef EDITSPHERE_H
#define EDITSPHERE_H

#include "OGRE/OgreAxisAlignedBox.h"
#include "edit.h"
#include "OGRE/OgreSphere.h"

namespace voxel {

class EditSphere : public Edit
{
public:
    typedef boost::shared_ptr<EditSphere> ptr;
    EditSphere(Ogre::Sphere sp, bool cut);

    Ogre::AxisAlignedBox aab(double scale) const;
    double calcVoxel(const Ogre::Vector3 &pos, const double &scale);
protected:
    static Ogre::Sphere scaleSphere(const Ogre::Sphere sp, const double scale);

    Ogre::Sphere m_sp;
};
}

#endif // EDITSPHERE_H
