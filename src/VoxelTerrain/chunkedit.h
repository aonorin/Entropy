#ifndef CHUNKEDIT_H
#define CHUNKEDIT_H

#include "edit.h"
#include "src/Utility/Vector.h"
#include "vector"
#include "Core/global.h"

namespace voxel {

//typedef unsigned short uint16;

class ChunkEdit : public Edit
{
public:
    typedef boost::shared_ptr<ChunkEdit> ptr;
    typedef std::vector<ptr> vector;
    ChunkEdit(Ogre::Vector3 min, Ogre::Vector3 max);

    Edit::vector edits;
    void insertEdit(Edit::ptr edit);

    Ogre::AxisAlignedBox m_abb;
    Ogre::AxisAlignedBox internalAbb;
    Ogre::AxisAlignedBox aab(double scale) const;
    void calculateAab();

    double calcVoxel(const Ogre::Vector3& pos, const double &scale);

private:
    int width,height,depth;
    Ogre::AxisAlignedBox scaleAab(const Ogre::AxisAlignedBox& aab, const double scale);
    inline bool lessThan(const Ogre::Vector3 &a,const Ogre::Vector3 &b) const;
    inline bool greaterThan(const Ogre::Vector3 &a,const Ogre::Vector3 &b) const;
};

}

#endif // CHUNKEDIT_H
