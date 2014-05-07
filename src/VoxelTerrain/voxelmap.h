#ifndef VOXELMAP_H
#define VOXELMAP_H

#include "voxel.h"

#include "OGRE/OgreSphere.h"
#include "src/Utility/mutexlocker.h"

#include "triangle.h"
#include "trianglevector.h"
#include "quadVector3.h"
//#include <map>

//#include "blub/core/log.hpp"
#include "src/Utility/todolistener.h"
#include "boost/shared_ptr.hpp"

#include "map3.h"
#include "part3.h"

#include <boost/signals2.hpp>
#include "boost/unordered_map.hpp"
#include "Core/global.h"
//typedef int int32;

namespace voxel {

class VoxelMap3 : protected ToDoListener, public utility::mutex
{
public:
    typedef Part3 myPart;
    typedef boost::unordered::unordered_map<int, boost::shared_ptr<Part3> > t_part;

    // threadsafe
    VoxelMap3(entropy::uint32 sizeX, entropy::uint32 sizeY, entropy::uint32 sizeZ, bool calcTransvoxel);
    ~VoxelMap3();
    // threadsafe
    boost::shared_ptr<myPart> getPart(int id);
    // lock class while working on parts!!! so that part cannot get deleted while working on it
    boost::unordered::unordered_map<int, boost::shared_ptr<Part3> > * getAllParts();

    // threadsafe
    void edit(boost::shared_ptr<Edit> ed, bool calcGeometry);

    // threadsafe
    typedef boost::signals2::signal<void (int)> t_signalPartChanged;
    void addPartChangedListener(const t_signalPartChanged::slot_type &subscriber)
    {m_sigPartChanged.connect(subscriber);}

    // threadsafe
    typedef boost::signals2::signal<void (int, bool)> t_signalPartAddedOrRemoved;
    void addPartAddedOrRemovedListener(const t_signalPartAddedOrRemoved::slot_type &subscriber)
    {m_sigPartAddedOrRemoved.connect(subscriber);}

    // threadsafe
    void scale(double sc);

    // threadsafe
    void calculatePart(int part);
private:
    // voxelMap3
    void todoEdit (boost::shared_ptr<Edit> ed, bool calculateGeometry);

    static int getKey(int posX, int posY, int posZ);
    boost::shared_ptr<myPart> getPart(int posX, int posY, int posZ);

    // voxelMap3
    void aabbIntersectsParts(const Ogre::AxisAlignedBox& aabb, int *firstX, int *firstY, int *firstZ, int *lastX, int *lastY, int *lastZ);

protected:
    // voxelMap3
    // locks class
    void removePart(int id);
    void removePart(int posX, int posY, int posZ);
    // voxelMap3
    // locks class
    boost::shared_ptr<myPart> addPart(int posX, int posY, int posZ);

    void partChanged(int id);
    void toDoCalculate(int id);

    t_part m_parts;
private:
    t_signalPartChanged m_sigPartChanged;
    t_signalPartAddedOrRemoved m_sigPartAddedOrRemoved;

    int m_sizeX;
    int m_sizeY;
    int m_sizeZ;

    double m_scale;

    bool m_calcTransvoxel;
};

}

#endif // VOXELMAP_H
