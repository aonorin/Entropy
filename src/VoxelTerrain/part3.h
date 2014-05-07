#ifndef PART3_H
#define PART3_H

#include "triangle.h"
#include "mutex.h"
//#include "hashmap.h"
#include "map3.h"
#include "voxel.h"
#include "edit.h"
#include "boost/signals2.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/unordered_map.hpp"

#ifndef WIN32
#include "tr1/array"
#else
#include <array>
#endif

#include "Core/global.h"
//typedef unsigned int uint32;
//typedef unsigned short uint16;

namespace Ogre{
    typedef boost::shared_ptr<std::vector<Ogre::Vector3> > Vector3Ptr;
}

namespace voxel {

class Triangle;

class Part3 : public utility::mutex
{
public:
    typedef boost::shared_ptr<Part3> ptr;
    Part3(int xStart, int yStart, int zStart, entropy::uint32 xSize, entropy::uint32 ySize, entropy::uint32 zSize, double scale, bool calcTransvoxel);
    ~Part3();

    // lock class part3 before
    void calculateGeometry();

//    Ogre::Vector3* hullVertices(void);
    Ogre::Vector3Ptr hullVertices();
    Ogre::Vector3Ptr hullNormals();
    // first is lod 0; the rest 6 are the transvoxel-lod
    Triangle::vectorPtr hullTriangles(entropy::uint16 ind);
    entropy::uint32 numVertices() const;
    // first is lod 0; the rest 6 are the transvoxel-lod
    entropy::uint32 numTriangles(entropy::uint16 ind) const;

    // deletes all vertices and triangles; for memorysaves; to recalculate geometry call calculateGeometry();
    void clearGeometry(void);

    typedef boost::signals2::signal<void ()> t_signalPartChanged;
    void addPartChangedListener(const t_signalPartChanged::slot_type &subscriber)
    {m_sigPartChanged.connect(subscriber);}

    Ogre::AxisAlignedBox aabb();

    // class part3 has to be locked
    void edit(boost::shared_ptr<Edit> ed);

    // class part3 has to be locked
    Map3* map();

private:
    struct vertex
    {
        vertex(Ogre::Vector3 _pos, Ogre::Vector3 _normal)
            : pos(_pos)
            , normal(_normal)
        {;}
        vertex(Ogre::Vector3 _pos)
            : pos(_pos)
            , normal(0,0,0)
        {;}
        vertex()
            : pos(0,0,0)
            , normal(0,0,0)
        {;}
        Ogre::Vector3 pos;
        Ogre::Vector3 normal;
    };

    void copyCalcToRelease();
    static Ogre::Vector3 calculateIntersection(entropy::uint32 nX, entropy::uint32 nY, entropy::uint32 nZ, entropy::uint16 corner0, entropy::uint16 corner1, Map3 *mp);
    Ogre::Vector3 calculateIntersectionTransvoxel(entropy::uint32 nX, entropy::uint32 nY, entropy::uint32 nZ, entropy::uint16 corner0, entropy::uint16 corner1, Map3 *mp, const int voxel[9][3]);
    static Ogre::Vector3 calculateCorner(Ogre::Vector3 offset, entropy::uint16 corner);
    static Ogre::Vector3 calculateCornerTransvoxel(Ogre::Vector3 offset, const entropy::uint16 corner, const int voxel[9][3]);
    void calculateMarchingCubes(int startX, int startY, int startZ,
                                int endX, int endY, int endZ,
                                Map3 *mp,
                                boost::unordered::unordered_map<entropy::uint32, vertex>* verticesCalc,
                                std::vector<Triangle>* triangleCalc);
    void calculateTransvoxel(int startX, int startY, int startZ,
                             int endX, int endY, int endZ,
                             Map3 *mp,
                             Map3 *mpTrans,
                             boost::unordered::unordered_map<entropy::uint32, vertex> *verticesCalc,
                             std::vector<Triangle>* triangleCalc, bool invertTriangles, int coord);

    static Ogre::Vector3 interpolate(double fX1, double fY1, double fZ1, double fX2, double fY2, double fZ2, double tVal1, double tVal2);
    // blub::real voxelScalar(entropy::uint32 x, entropy::uint32 y, entropy::uint32 z);
    static double voxelScalar(entropy::uint32 x, entropy::uint32 y, entropy::uint32 z, Map3 *mp);
    static double voxelScalarTrans(entropy::uint32 x, entropy::uint32 y, entropy::uint32 z, Map3 *mp);
    entropy::uint32 edgeID(entropy::uint32 nX, entropy::uint32 nY, entropy::uint32 nZ, entropy::uint16 edgeId);
    entropy::uint32 vertexID(entropy::uint32 nX, entropy::uint32 nY, entropy::uint32 nZ);

    Map3 m_map;
    Map3 *m_mapTransvoxel;

    int m_startX;
    int m_startY;
    int m_startZ;
    entropy::uint32 m_sizeX;
    entropy::uint32 m_sizeY;
    entropy::uint32 m_sizeZ;

    Ogre::Vector3Ptr m_hullVerticesRelease;
    Ogre::Vector3Ptr m_hullNormalsRelease;
    // one mainpart, 6 with lod 0 and 6 with lod 1
//    Triangle* m_hullTrianglesRelease[1+6];

    std::tr1::array<Triangle::vectorPtr,1+6> m_hullTrianglesRelease;
    entropy::uint32 m_numVertices;
    entropy::uint32 m_numTriangles[1+6];

    typedef boost::unordered::unordered_map<entropy::uint32, vertex> t_hullVerticesCalc;
    // one mainpart, 6 with lod 0 and 6 with lod 1
    t_hullVerticesCalc m_hullVerticesCalc;
    // one mainpart, 6 with lod 0 and 6 with lod 1
    std::vector<Triangle> m_hullTrianglesCalc[1+6];

    t_signalPartChanged m_sigPartChanged;

    static const entropy::uint32 m_edgeTable[256];
    static const int m_triTable[256][16];

    double m_scale;
    typedef boost::shared_ptr<Edit> t_edit;
    std::vector<t_edit> m_edits;

    bool m_calcTransvoxel;
};

}

#endif // PART3_H
