#include "megachunk.h"

using namespace voxel;

MegaChunk::MegaChunk(Ogre::Vector3 pos, entropy::uint32 width, entropy::uint32 height, entropy::uint32 depth, float scale) :
    Edit(false),
    pos(pos),
    width(width),
    height(height),
    depth(depth),
    scale(scale),
    m_abb(Ogre::AxisAlignedBox(pos.x,pos.y,pos.z,pos.x+width,pos.y+height,pos.z+depth))
{
    for(int x=0;x<width*scale;++x)
    {
        editGroups.push_back(std::vector<std::vector<EditGroup> > ());
        for(int y=0;y<height*scale;++y)
        {
            editGroups.at(x).push_back(std::vector<EditGroup> ());
            for(int z=0;z<depth*scale;++z)
            {
                editGroups.at(x).at(y).push_back(EditGroup());
            }
        }
    }
}

//SPEED UP MEGACHUNK! Perhaps with quadrants/parts....Use Binary Tree princaples to partition it....

double MegaChunk::calcVoxel(const Ogre::Vector3& pos, const double &scale)
{
    //    std::cout << "calcVoxel(const Ogre::Vector3& pos, const double &scale) begin" << std::endl;
    double scalar = 0;
    int num = 0;
    bool oneContains = false;
    bool cont = true;
    if(pos.x <0)
        cont = false;
    if(pos.y <0)
        cont = false;
    if(pos.z <0)
        cont = false;
    if(pos.x >=width)
        cont = false;
    if(pos.y >=height)
        cont = false;
    if(pos.z >=depth)
        cont = false;

    if(cont) {
        Edit::vector::const_iterator it =  editGroups.at(pos.x*this->scale).at(pos.y*this->scale).at(pos.z*this->scale).edits.begin();
        while(it!=editGroups.at(pos.x*this->scale).at(pos.y*this->scale).at(pos.z*this->scale).edits.end()){
            if((*it)->calcVoxel(pos,scale) <0)
            {
                //                std::cout << "MEGA Hollow! " << pos.x<<","<<pos.y<<","<<pos.z<<std::endl;
                return -2.0;
            }
            ++it;
        }

        //    std::cout << "calcVoxel(const Ogre::Vector3& pos, const double &scale) end" << std::endl;
        return 1.0;
    } else {
        std::cout << "MEGACHUNK OUT OF BOUNDS!!!" << std::endl;
        return 1.0;
    }

    /*
    for(ChunkEdit::vector::const_iterator it = chunkEdits.begin();it!=chunkEdits.end();++it)
    {
        if((*it)->aab(scale).contains(pos))
        {
            //            scalar += (*it)->calcVoxel(pos,scale);
            //            num++;
            if((*it)->calcVoxel(pos,scale) <0)
            {
//                std::cout << "MEGA Hollow! " << pos.x<<","<<pos.y<<","<<pos.z<<std::endl;
                return -2.0;
            }
            oneContains = true;
        }
    }
    if(!oneContains)
    {
        //        std::cout << "DOES CONTAIN" << std::endl;
    } else
    {
        //        std::cout << "does not contain" << std::endl;
    }
    //    std::cout << "Filled! " << pos.x<<","<<pos.y<<","<<pos.z<<std::endl;
    */
}

void MegaChunk::insertChunk(ChunkEdit::ptr chunkEdit)
{
    chunkEdits.push_back(chunkEdit);
}

void MegaChunk::insertEdit(Edit::ptr edit)
{
    //    std::cout << "MegaChunk::insertEdit(Edit::ptr edit) beginning" << std::endl;
    int lowX,highX,lowY,highY,lowZ,highZ;

    lowX = edit->aab(1).getMinimum().x*scale;
    highX = edit->aab(1).getMaximum().x*scale;
    lowY = edit->aab(1).getMinimum().y*scale;
    highY = edit->aab(1).getMaximum().y*scale;
    lowZ = edit->aab(1).getMinimum().z*scale;
    highZ = edit->aab(1).getMaximum().z*scale;
    if(lowX<0)
        lowX=0;
    if(lowY<0)
        lowY=0;
    if(lowZ<0)
        lowZ=0;
    if(highX>=width*scale)
        highX=width*scale-1;
    if(highY>=height*scale)
        highY=height*scale-1;
    if(highZ>=depth*scale)
        highZ=depth*scale-1;

    for(int x=lowX;x<=highX;++x)
    {
        for(int y=lowY;y<=highY;++y)
        {
            for(int z=lowZ;z<=highZ;++z)
            {
                editGroups.at(x).at(y).at(z).edits.push_back(edit);
            }
        }
    }
    //    std::cout << "MegaChunk::insertEdit(Edit::ptr edit) end" << std::endl;
}

void MegaChunk::calculateAAb()
{
    for(ChunkEdit::vector::const_iterator it = chunkEdits.begin();it!=chunkEdits.end();++it)
    {
        (*it)->calculateAab();
    }
}

Ogre::AxisAlignedBox MegaChunk::aab(double scale) const
{
    Ogre::AxisAlignedBox aab(m_abb.getMinimum()/scale, m_abb.getMaximum()/scale);
    Ogre::AxisAlignedBox result(aab.getMinimum()-Ogre::Vector3(1,1,1), aab.getMaximum()+Ogre::Vector3(1,1,1));
    return result;
}

Ogre::AxisAlignedBox MegaChunk::scaleAab(const Ogre::AxisAlignedBox& aab, const double scale)
{
    return Ogre::AxisAlignedBox (aab.getMinimum()/scale, aab.getMaximum()/scale);
}

