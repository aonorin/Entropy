#include "voxelmap.h"

using namespace voxel;


VoxelMap3::VoxelMap3(entropy::uint32 sizeX, entropy::uint32 sizeY, entropy::uint32 sizeZ, bool calcTransvoxel)
    : ToDoListener(1)
    , m_sizeX(sizeX)
    , m_sizeY(sizeY)
    , m_sizeZ(sizeZ)
    , m_scale(1.0)
    , m_calcTransvoxel(calcTransvoxel)
{
}

VoxelMap3::~VoxelMap3()
{
    std::cout << "VoxelMap3::~VoxelMap3()" << std::endl;
}

boost::shared_ptr<Part3> VoxelMap3::getPart(int id)
{
    utility::MutexLocker locker(*this); (void)locker;
    t_part::iterator it = m_parts.find(id);
    if (it == m_parts.end())
        return boost::shared_ptr<Part3>();
    return (*it).second;
}
boost::unordered::unordered_map<int, boost::shared_ptr<Part3> > * VoxelMap3::getAllParts()
{
    return &m_parts;
}

void VoxelMap3::edit(boost::shared_ptr<Edit> ed, bool calcGeometry)
{
    addToDo(boost::bind(&VoxelMap3::todoEdit, this, ed, calcGeometry));
}


void VoxelMap3::todoEdit (boost::shared_ptr<Edit> ed, bool calculateGeometry)
{
#ifdef DEBUG
    std::cout << "VoxelMap3::todoEdit (boost::shared_ptr<Edit> ed, bool calculateGeometry)" << std::endl;
#endif

    Ogre::AxisAlignedBox aabb(ed->aab(m_scale));

    // calc position of first part
    int firstX;
    int firstY;
    int firstZ;
    // calc position of last part
    int lastX;
    int lastY;
    int lastZ;
    aabbIntersectsParts(aabb, &firstX, &firstY, &firstZ, &lastX, &lastY, &lastZ);

    // loop parts
    for (int indX = firstX; indX <= lastX; ++indX)
    {
        for (int indY = firstY; indY <= lastY; ++indY)
        {
            for (int indZ = firstZ; indZ <= lastZ; ++indZ)
            {
                boost::shared_ptr<Part3> part = getPart(indX, indY, indZ);
                if (part == boost::shared_ptr<Part3>())
                    part = addPart(indX, indY, indZ);
                part->lock();

                part->edit(ed);

                part->unlock();

                m_sigPartAddedOrRemoved(getKey(indX, indY, indZ), false);

                // m_lockToCalculate.lock();
                if (calculateGeometry)
                    calculatePart(getKey(indX, indY, indZ));
                // m_lockToCalculate.unlock();

            }
        }
    }

    //    std::cout << "void edit (sphere sp, bool cut) end" << std::endl;
}

void VoxelMap3::calculatePart(int part)
{
    addToDo(boost::bind(&VoxelMap3::toDoCalculate, this, part));
}

int VoxelMap3::getKey(int posX, int posY, int posZ)
{
    return (posX+128) + 256*(posY+128) + 256*256*(posZ+128);
    // return (posX) + 256*(posY) + 256*256*(posZ);
}

boost::shared_ptr<Part3> VoxelMap3::getPart(int posX, int posY, int posZ)
{
    return getPart(getKey(posX, posY, posZ));
}


void VoxelMap3::aabbIntersectsParts(const Ogre::AxisAlignedBox& aabb, int *firstX, int *firstY, int *firstZ, int *lastX, int *lastY, int *lastZ)
{
    Ogre::Vector3 vec = aabb.getMinimum()-Ogre::Vector3(1.,1.,1.);
    int posX = vec.x;
    int posY = vec.y;
    int posZ = vec.z;
    vec = aabb.getSize()+Ogre::Vector3(2.,2.,2.);
    int sizeX = vec.x;
    int sizeY = vec.y;
    int sizeZ = vec.z;

    // calc position of first part
    *firstX = posX / m_sizeX;
    if (posX < 0) --(*firstX);
    *firstY = posY / m_sizeY;
    if (posY < 0) --(*firstY);
    *firstZ = posZ / m_sizeZ;
    if (posZ < 0) --(*firstZ);

    // calc position of last part
    *lastX = (posX + sizeX) / m_sizeX;
    if (posX + sizeX < 0) --(*lastX);
    *lastY = (posY + sizeY) / m_sizeY;
    if (posY + sizeY < 0) --(*lastY);
    *lastZ = (posZ + sizeZ) / m_sizeZ;
    if (posZ + sizeZ < 0) --(*lastZ);
}

void VoxelMap3::removePart(int key)
{
    m_sigPartAddedOrRemoved(key, true);
    utility::MutexLocker locker(*this);(void)locker;
    boost::unordered::unordered_map<int, boost::shared_ptr<myPart> >::iterator it = m_parts.find(key);
    if (it == m_parts.end())
        return;
    m_parts.erase(it);
}

void VoxelMap3::removePart(int posX, int posY, int posZ)
{
#ifdef DEBUG
    std::cout << "VoxelMap3::removePart("<<posX<<","<<posY<<","<<posZ<<")"<<std::endl;
#endif
    int key = getKey(posX, posY, posZ);
    removePart(key);
}

boost::shared_ptr<Part3> VoxelMap3::addPart(int posX, int posY, int posZ)
{
//#ifdef DEBUG
    std::cout << "VoxelMap3::addPart("<<posX<<","<<posY<<","<<posZ<<")"<<std::endl;
//#int
    int key = getKey(posX, posY, posZ);
    boost::shared_ptr<Part3> part(new Part3(posX*m_sizeX, posY*m_sizeY, posZ*m_sizeZ, m_sizeX, m_sizeY, m_sizeZ, m_scale, m_calcTransvoxel));
    part->addPartChangedListener(boost::bind(&VoxelMap3::partChanged, this, key));
    {
        utility::MutexLocker locker(*this);(void)locker;
        if (m_parts.find(key) != m_parts.end())
            std::cerr << "m_parts.find(key) != m_parts.end()" << std::endl;
        m_parts[key] = part;
    }
    //  m_sigPartAddedOrRemoved(key, false);

    return part;
}

void VoxelMap3::partChanged(int id)
{
#ifdef DEBUG
    std::cout << "VoxelMap3::partChanged("<<id<<")" << std::endl;
#endif
    m_sigPartChanged(id);
}

void VoxelMap3::toDoCalculate(int id)
{
#ifdef DEBUG
    std::cout << "VoxelMap3::toDoCalculate("<<id<<")" << std::endl;
#endif
    boost::shared_ptr<Part3> part (getPart(id));
    if (part != boost::shared_ptr<Part3>())
    {
        utility::MutexLocker locker(*part); (void)locker;
        part->calculateGeometry();
        if (part->map()->getNumAboveZero() == 0)
            removePart(id);
    }
    else
    {
        ;//BERROR("part.isNull()");
    }
}

void VoxelMap3::scale(double sc)
{
    m_scale = sc;
}


