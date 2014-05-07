#ifndef BLOBEDIT_H
#define BLOBEDIT_H


#include "VoxelTerrain/edit.h"
#include "Utility/Vector.h"
#include "boost/unordered_map.hpp"
#include <bitset>
#include "Core/global.h"

namespace voxel {

//typedef unsigned short uint16;
//typedef boost::unordered::unordered_map<geometry::Vec3ui16,uint16> BlobMap_t;
//typedef boost::unordered::unordered_map<int,char> BlobMap_t;
//typedef boost::unordered::unordered_map<std::bitset<8>,std::bitset<8> > BlobMap_t;
//typedef std::bitset<33000000> BlobMap_t;
//typedef char int8;
//typedef std::vector<int8> BlobMap_t;
//typedef std::map<int,char> BlobMap_t;

class BlobEdit : public voxel::Edit
{
public:
    typedef boost::shared_ptr<BlobEdit> ptr;
    typedef std::vector<ptr> vector;
    BlobEdit(entropy::uint16 width,entropy::uint16 height,entropy::uint16 depth,Ogre::Vector3 pos = Ogre::Vector3(0,0,0));
    ~BlobEdit();
    void insertPoint(Ogre::Vector3 pos, float val);
//    entropy::Int8ArrayPtr_t blobMap;
    entropy::FloatArrayPtr_t blobMap;
    Ogre::Vector3 pos;

    Ogre::AxisAlignedBox m_abb;
    Ogre::AxisAlignedBox aab(double scale) const;
    double calcVoxel(const Ogre::Vector3& pos, const double &scale);

private:
    int width,height,depth;
    float totalSize;

};

}
#endif // BLOBEDIT_H
