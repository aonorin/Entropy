#include "caveedit.h"

using namespace voxel;

CaveEdit::CaveEdit(Ogre::AxisAlignedBox a_abb, alife::DiamondSquared::ptr diamondSquared, bool cut) :
    EditAxisAlignedBox(a_abb,cut),
    diamondSquared(diamondSquared),
    terrainSize(4)
{
    for(entropy::uint16 i=0;i<7;i++) {
        atlasPositions.push_back(Ogre::Vector2((totalCount*terrainSize)%diamondSquared->getWidth(),(totalCount*terrainSize)/diamondSquared->getHeight()));
        totalCount++;
    }
}

double CaveEdit::calcVoxel(const Ogre::Vector3& pos, const double &scale)
{
    Ogre::AxisAlignedBox _aab = scaleAab(m_abb, scale*1);
//    if (aab(scale*2).contains(pos)) {
    /*
        Ogre::Vector3 direction = getDirection(_aab.getCenter(),pos);

        int face;
        float m,s,t;
        float x = direction.x;
        float y = direction.y;
        float z = direction.z;

        //Convert to face + u/v, just like Cube Mapping
        if((abs(x) > abs(y)) && (abs(x) > abs(z)))
        {
            if (x>=0)
            {
                face = 0; m = x; s = -z; t = -y;
            }
            else
            {
                face = 1; m = -x; s = z; t = -y;
            }
        }
        else if((abs(y) > abs(x)) && (abs(y) > abs(z)))
        {
            if (y>=0)
            {
                face = 2; m = y; s = x; t = z;
            }
            else
            {
                face = 3; m = -y; s = x; t = -z;
            }
        }
        else
        {
            if (z>=0)
            {
                face = 4; m = z; s = x; t = -y;
            }
            else
            {
                face = 5; m = -z; s = -x; t = -y;
            }
        }

        float u = (s/m + 1.f) /2.f;
        float v = (t/m + 1.f) /2.f;

        Ogre::Vector2 atlasPos = texelToAtlas(face,Ogre::Vector2(u,v));

        double scalar = (diamondSquared->getBicubicSample(atlasPos.x,atlasPos.y)*0.75);
//        scalar = (_aab.getCenter().distance(pos));// * (scalar+0.75);
        */
        if (_aab.contains(pos)) {
            return 1;
        } else {
            return -2;
        }


//        return scalar;
//    } else {
//        return -2.;
//    }
}

Ogre::Vector3 CaveEdit::getDirection(const Ogre::Vector3 &pos,const Ogre::Vector3 &pos2) const
{
    return (pos - pos2).normalisedCopy();
}

Ogre::Vector2 CaveEdit::texelToAtlas(int face, const Ogre::Vector2 pos) const
{
    return ((pos + 0.5) * terrainSize) + atlasPositions.at(face);
}

/*
Ogre::AxisAlignedBox CaveEdit::aab(double scale) const
{
    return EditAxisAlignedBox::aab(scale);
}
*/
