#ifndef MEGACHUNK_H
#define MEGACHUNK_H

#include "edit.h"
#include "chunkedit.h"
#include "Core/global.h"

//typedef unsigned short uint16;
//typedef unsigned int uint32;

namespace voxel {

struct EditGroup {
    typedef boost::shared_ptr<EditGroup> ptr;
    typedef std::vector<ptr> vector;
    Edit::vector edits;
};

class MegaChunk : public Edit
{
public:
    typedef boost::shared_ptr<MegaChunk> ptr;
    typedef std::vector<std::vector<std::vector<EditGroup > > > t_EditGroup3DVector;
    MegaChunk(Ogre::Vector3 pos,entropy::uint32 width, entropy::uint32 height, entropy::uint32 depth,float scale);

    Ogre::AxisAlignedBox m_abb;
    Ogre::AxisAlignedBox aab(double scale) const;
    double calcVoxel(const Ogre::Vector3& pos, const double &scale);

    void insertChunk(ChunkEdit::ptr chunkEdit);
    void calculateAAb();
    void insertEdit(Edit::ptr edit);

    ChunkEdit::vector chunkEdits;

protected:
    Ogre::Vector3 pos;
    int width,height,depth;
    float scale;
    t_EditGroup3DVector editGroups;
    Ogre::AxisAlignedBox scaleAab(const Ogre::AxisAlignedBox& aab, const double scale);

};

}

#endif // MEGACHUNK_H
