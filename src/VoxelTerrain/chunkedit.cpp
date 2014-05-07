#include "chunkedit.h"



namespace voxel {

ChunkEdit::ChunkEdit(Ogre::Vector3 min, Ogre::Vector3 max) :
    width(width),
    height(height),
    depth(depth),
    m_abb(min,max),
    Edit(false)
{

}

void ChunkEdit::calculateAab()
{
    m_abb = Ogre::AxisAlignedBox(m_abb.getCenter(),m_abb.getCenter());
    for(Edit::iterator it = edits.begin();it!=edits.end();++it)
    {
        if( (*it)->aab(1).getMinimum().x<m_abb.getMinimum().x )
        {
            m_abb.setMinimumX((*it)->aab(1).getMinimum().x);
        }
        if( (*it)->aab(1).getMinimum().y<m_abb.getMinimum().y )
        {
            m_abb.setMinimumY((*it)->aab(1).getMinimum().y);
        }
        if( (*it)->aab(1).getMinimum().z<m_abb.getMinimum().z )
        {
            m_abb.setMinimumZ((*it)->aab(1).getMinimum().z);
        }
        if( (*it)->aab(1).getMaximum().x>m_abb.getMaximum().x )
        {
            m_abb.setMaximumX((*it)->aab(1).getMaximum().x);
        }
        if( (*it)->aab(1).getMaximum().y>m_abb.getMaximum().y )
        {
            m_abb.setMaximumY((*it)->aab(1).getMaximum().y);
        }
        if( (*it)->aab(1).getMaximum().z>m_abb.getMaximum().z )
        {
            m_abb.setMaximumZ((*it)->aab(1).getMaximum().z);
        }
    }
//    internalAbb = m_abb;
//    m_abb = Ogre::AxisAlignedBox( m_abb.getMinimum() - Ogre::Vector3(-1,-1,-1), m_abb.getMaximum() + Ogre::Vector3(1,1,1) );
}

bool ChunkEdit::lessThan(const Ogre::Vector3 &a, const Ogre::Vector3 &b) const
{
    if(a.x < b.x) {
        return true;
    } else if(a.x > b.x) {
        return false;
    } else {
        if(a.y<b.y) {
            return true;
        } else if(a.y > b.y) {
            return false;
        } else {
            if(a.z < b.z) {
                return true;
            } else if(a.z > b.z) {
                return false;
            } else {
                return false;
            }
        }
    }
}

bool ChunkEdit::greaterThan(const Ogre::Vector3 &a, const Ogre::Vector3 &b) const
{
    if(a.x > b.x) {
        return true;
    } else if(a.x > b.x) {
        return false;
    } else {
        if(a.y>b.y) {
            return true;
        } else if(a.y > b.y) {
            return false;
        } else {
            if(a.z < b.z) {
                return true;
            } else if(a.z > b.z) {
                return false;
            } else {
                return false;
            }
        }
    }
}

Ogre::AxisAlignedBox ChunkEdit::aab(double scale) const
{
    Ogre::AxisAlignedBox aab(m_abb.getMinimum()/scale, m_abb.getMaximum()/scale);
    Ogre::AxisAlignedBox result(aab.getMinimum()-Ogre::Vector3(1,1,1), aab.getMaximum()+Ogre::Vector3(1,1,1));
    return result;
}
//Maybe MegaChunk shouldn't average? even one -1 = -1!
double ChunkEdit::calcVoxel(const Ogre::Vector3& pos, const double &scale)
{
    double scalar = 0;
    int num = 0;
    if (aab(scale).contains(pos)) {
//        std::cout << "Chunk contains" << std::endl;
        for(Edit::iterator it = edits.begin();it!=edits.end();++it) {
//            if((*it)->aab(scale).contains(pos)) {
                double tempScalar = (*it)->calcVoxel(pos,scale);
                if(tempScalar > 1.) {
                    scalar += 1.;
                    num++;
                } else if(tempScalar > -1.) {
                    scalar += tempScalar;
                    num++;
                } else {
                    scalar += -1.;
                    num++;
                }
//            }
        }
        if(num) {
            scalar = scalar/((double)num);
            if(scalar>-1.) {
//                std::cout << "Chunk hollow"<< std::endl;
                return -2.;
            } else {
                return 1.;
            }
        } else {
//            scalar = -2.0;
            scalar = 1.;
        }
        //        std::cout << "ChunkEdit scalar: " << scalar << std::endl;
        return scalar;
    } else {
//        if(aab(scale).contains(pos)) {
//            return 1.0;
//        } else {
//            return -2.0;
//        }
        return 1.0;
    }
}

void ChunkEdit::insertEdit(Edit::ptr edit)
{
    edits.push_back(edit);
}

Ogre::AxisAlignedBox ChunkEdit::scaleAab(const Ogre::AxisAlignedBox& aab, const double scale)
{
    return Ogre::AxisAlignedBox (aab.getMinimum()/scale, aab.getMaximum()/scale);
}

}
