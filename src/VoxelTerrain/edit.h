#ifndef EDIT_H
#define EDIT_H

#include "OGRE/OgreAxisAlignedBox.h"

namespace voxel {

class Map3;
class Edit {

public:
    typedef boost::shared_ptr<Edit> ptr;
    typedef std::vector<ptr> vector;
    typedef vector::const_iterator iterator;

    Edit(bool cut) : cut(cut) {;}

    virtual Ogre::AxisAlignedBox aab(double scale) const = 0;
    // mp has to be locked! AND begin had to be called
    void createVoxel(Map3 *mp,
                     const int startX,
                     const int startY,
                     const int startZ,
                     const int endX,
                     const int endY,
                     const int endZ,
                     const double scale);
    virtual double calcVoxel(const Ogre::Vector3& pos, const double &scale) = 0;
protected:
    bool cut;
};

}

#endif // EDIT_H
