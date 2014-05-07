#ifndef CAVEEDIT_H
#define CAVEEDIT_H

#include "ArtificialLife/diamondsquared.h"
#include "editaxisalignedbox.h"
#include "editsphere.h"
#include "OGRE/OgreVector2.h"
#include "Core/global.h"

//typedef unsigned short uint16;
//typedef unsigned int uint32;

namespace voxel {
static entropy::uint32 totalCount = 0;
class CaveEdit : public EditAxisAlignedBox
{
public:
    typedef boost::shared_ptr<CaveEdit> ptr;
    CaveEdit(Ogre::AxisAlignedBox a_abb, alife::DiamondSquared::ptr diamondSquared, bool cut = false);

    double calcVoxel(const Ogre::Vector3 &pos, const double &scale);
//    Ogre::AxisAlignedBox aab(double scale) const;

protected:
    alife::DiamondSquared::ptr diamondSquared;
    entropy::uint32 terrainSize;
    std::vector<Ogre::Vector2> atlasPositions;

    Ogre::Vector2 texelToAtlas(int face, const Ogre::Vector2 pos) const;
    Ogre::Vector3 getDirection(const Ogre::Vector3 &pos, const Ogre::Vector3 &pos2) const;
};

}

#endif // CAVEEDIT_H
