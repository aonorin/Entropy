#include "blobedit.h"

using namespace voxel;

BlobEdit::BlobEdit(entropy::uint16 width, entropy::uint16 height, entropy::uint16 depth, Ogre::Vector3 pos) :
    Edit(false),
    width(width),
    height(height),
    depth(depth),
    pos(pos),
    m_abb(Ogre::AxisAlignedBox(pos.x,pos.y,pos.z,pos.x+width,pos.y+height,pos.z+depth)),
//    blobMap(new entropy::Int8Array_t(MAPSIZE_X*MAPSIZE_Y*MAPSIZE_Z,1))
    blobMap(new std::vector<float>(MAPSIZE_X*MAPSIZE_Y*MAPSIZE_Z,0))
{

}

BlobEdit::~BlobEdit()
{
    std::cout << "BlobEdit::~BlobEdit()" << std::endl;
}

Ogre::AxisAlignedBox BlobEdit::aab(double scale) const
{
    Ogre::AxisAlignedBox aab(m_abb.getMinimum()/scale, m_abb.getMaximum()/scale);
    Ogre::AxisAlignedBox result (aab.getMinimum()-Ogre::Vector3(1,1,1), aab.getMaximum()+Ogre::Vector3(1,1,1));
    return result;
}

double BlobEdit::calcVoxel(const Ogre::Vector3& pos, const double &scale)
{
    int oneDpos = ((int)(pos.x-this->pos.x)) + width  * (((int)(pos.y-this->pos.y)) + depth * ((int)(pos.z-this->pos.z)));
    oneDpos *= (1/scale);

    if(oneDpos>0&&oneDpos<(MAPSIZE_X*MAPSIZE_Y*MAPSIZE_Z*scale))
    {
        /*
        if(blobMap.test(oneDpos))
        {
            return -1.0f;
        } else {
            return 1.0f;
        }
        */
        return blobMap->at(oneDpos);
    }
}


void BlobEdit::insertPoint(Ogre::Vector3 pos, float val)
{
    int oneDpos = ((int)(pos.x-this->pos.x)) + width  * (((int)(pos.y-this->pos.y)) + depth * ((int)(pos.z-this->pos.z)));
    if(oneDpos>0&&oneDpos<MAPSIZE_X*MAPSIZE_Y*MAPSIZE_Z)
    {
//        if(val<0)
//        {
            blobMap->at(oneDpos) = val;
            //            blobMap->at(oneDpos) = -1;
            //            blobMap.set(oneDpos,1);
//        }
    }
}
