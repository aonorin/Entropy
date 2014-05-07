#include "part3.h"
#include "transvoxel.h"
#include "boost/unordered_map.hpp"
#include "boost/foreach.hpp"
namespace voxel {


Part3::Part3(int xStart, int yStart, int zStart, entropy::uint32 xSize, entropy::uint32 ySize, entropy::uint32 zSize, double scale, bool calcTransvoxel)
    : m_map(1, 1, 1, xSize+3, ySize+3, zSize+3)
    , m_mapTransvoxel(0)
    , m_startX(xStart)
    , m_startY(yStart)
    , m_startZ(zStart)
    , m_sizeX(xSize)
    , m_sizeY(ySize)
    , m_sizeZ(zSize)
    //    , m_hullVerticesRelease()
    //    , m_hullNormalsRelease(0)
    , m_scale(scale)
    , m_calcTransvoxel(calcTransvoxel)
{
    for(entropy::uint16 i=0;i<1+6;++i) {
        m_hullTrianglesRelease.at(0) = Triangle::vectorPtr();
    }
    //    m_hullTrianglesRelease[0] = 0;

    if (m_calcTransvoxel)
    {   // init transvoxel maps
        m_mapTransvoxel = new Map3[6];
        m_mapTransvoxel[0] = Map3(1, 1, 0, xSize*2+1+3, ySize*2+1+3, 1);
        m_mapTransvoxel[1] = Map3(1, 1, -zSize*2, xSize*2+1+3, ySize*2+1+3, 1);

        m_mapTransvoxel[2] = Map3(1, 0, 1, xSize*2+1+3, 1, zSize*2+1+3);
        m_mapTransvoxel[3] = Map3(1, -ySize*2, 1, xSize*2+1+3, 1, zSize*2+1+3);

        m_mapTransvoxel[4] = Map3(0, 1, 1, 1, ySize*2+1+3, zSize*2+1+3);
        m_mapTransvoxel[5] = Map3(-xSize*2, 1, 1, 1, ySize*2+1+3, zSize*2+1+3);
    }
}

Part3::~Part3()
{
    std::cout << "Part3::~Part3()" << std::endl;
    if (m_mapTransvoxel)
        delete [] m_mapTransvoxel;
}

void Part3::calculateGeometry()
{
#ifdef DEBUG
    std::cout << "Part3::calculateGeometry()" << std::endl;
#endif
    // cleanUp
    clearGeometry();
    m_map.begin();
    if (m_calcTransvoxel)
    {
        for (entropy::uint32 ind = 0; ind < 6; ++ind)
            m_mapTransvoxel[ind].begin();
    }
    //axisAlignedBox delta;
    //delta.setNull();
    for (std::vector<t_edit>::iterator it = m_edits.begin(); it != m_edits.end(); ++it)
    {
        //        std::cout << "iterating edits: "<< std::endl;
        Edit *ed = (*it).get();
        ed->createVoxel(&m_map,
                        m_startX-1, m_startY-1, m_startZ-1,
                        m_startX+m_sizeX+2, m_startY+m_sizeY+2, m_startZ+m_sizeZ+2,
                        m_scale); // voxel get created, slower but saves !lots! of memory
        //delta.merge(ed->aab(1.));
        if (m_calcTransvoxel)
        {
            m_startX*=2;m_startY*=2;m_startZ*=2;
            ed->createVoxel(&m_mapTransvoxel[0],
                            m_startX-1, m_startY-1, m_startZ-0,
                            m_startX+m_sizeX*2+2, m_startY+m_sizeY*2+2, m_startZ+1,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            ed->createVoxel(&m_mapTransvoxel[1],
                            m_startX-1, m_startY-1, m_startZ+m_sizeZ*2-0,
                            m_startX+m_sizeX*2+2, m_startY+m_sizeY*2+2, m_startZ+m_sizeZ*2+1,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            ed->createVoxel(&m_mapTransvoxel[2],
                            m_startX-1, m_startY+0, m_startZ-1,
                            m_startX+m_sizeX*2+2, m_startY+1, m_startZ+m_sizeZ*2+2,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            ed->createVoxel(&m_mapTransvoxel[3],
                            m_startX-1, m_startY+m_sizeY*2-0, m_startZ-1,
                            m_startX+m_sizeX*2+2, m_startY+m_sizeY*2+1, m_startZ+m_sizeZ*2+2,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            ed->createVoxel(&m_mapTransvoxel[4],
                            m_startX-0, m_startY-1, m_startZ-1,
                            m_startX+1, m_startY+m_sizeY*2+2, m_startZ+m_sizeZ*2+2,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            ed->createVoxel(&m_mapTransvoxel[5],
                            m_startX+m_sizeX*2-0, m_startY-1, m_startZ-1,
                            m_startX+m_sizeX*2+1, m_startY+m_sizeY*2+2, m_startZ+m_sizeZ*2+2,
                            m_scale/2.); // voxel get created, slower but saves !lots! of memory
            m_startX/=2;m_startY/=2;m_startZ/=2;
        }
    }
    //    std::cout << "Part3 edit voxels created." << std::endl;
    //delta.move(-vector3(m_startX, m_startY, m_startZ));
    //delta.scale(vector3(1./m_scale));
    // Generate isosurface.
    // axisAlignedBox delta = m_map.delta();
    int startX = /*delta.getMinimum().x-1; if (startX < 0) startX = */0;
    int startY = /*delta.getMinimum().y-1; if (startY < 0) startY = */0;
    int startZ = /*delta.getMinimum().z-1; if (startZ < 0) startZ = */0;

    // +2 because of "<" in later for
    int endX = /*delta.getMaximum().x+3; if (endX > (signed)m_sizeX+2) endX = */m_sizeX+2;
    int endY = /*delta.getMaximum().y+3; if (endY > (signed)m_sizeY+2) endY = */m_sizeY+2;
    int endZ = /*delta.getMaximum().z+3; if (endZ > (signed)m_sizeZ+2) endZ = */m_sizeZ+2;
#ifdef DEBUG
    std::cout << "Part3 calculate marchingcubes begin" << std::endl;
#endif
    calculateMarchingCubes(startX+1, startY+1, startZ+1, endX-1, endY-1, endZ-1, &m_map, &m_hullVerticesCalc, &m_hullTrianglesCalc[0]);
#ifdef DEBUG
    std::cout << "Part3 calculate marchingcubes end" << std::endl;
#endif
    const int offsetsNormalCorrection[][6] = {
        {startX, startY, startZ, endX, endY, startZ+1},
        {startX, startY, endZ-1, endX, endY, endZ},
        {startX, startY, startZ, endX, startY+1, endZ},
        {startX, endY-1, startZ, endX, endY, endZ},
        {startX, startY, startZ, startX+1, endY, endZ},
        {endX-1, startY, startZ, endX, endY, endZ},
    };
    t_hullVerticesCalc verticesBuffer;
    for (entropy::uint16 ind = 0; ind < 6; ++ind)
    {
        calculateMarchingCubes(offsetsNormalCorrection[ind][0],
                               offsetsNormalCorrection[ind][1],
                               offsetsNormalCorrection[ind][2],
                               offsetsNormalCorrection[ind][3],
                               offsetsNormalCorrection[ind][4],
                               offsetsNormalCorrection[ind][5],
                               &m_map, &verticesBuffer,0);
    }
    for (t_hullVerticesCalc::iterator it = verticesBuffer.begin(); it != verticesBuffer.end(); ++it)
    {
        t_hullVerticesCalc::iterator it2 = m_hullVerticesCalc.find((*it).first);
        if (it2 != m_hullVerticesCalc.end())
            (*it2).second.normal += (*it2).second.normal;
    }


    if (m_calcTransvoxel)
    {
        calculateTransvoxel(startX+1, startY+1, 1, endX-1, endY-1, 2, &m_map, &m_mapTransvoxel[0], &m_hullVerticesCalc, &m_hullTrianglesCalc[1], false, 1);
        calculateTransvoxel(startX+1, startY+1, m_sizeZ+1, endX-1, endY-1, m_sizeZ+2, &m_map, &m_mapTransvoxel[1], &m_hullVerticesCalc, &m_hullTrianglesCalc[2], true, 1);

        calculateTransvoxel(startX+1, 1, startZ+1, endX-1, 2, endZ-1, &m_map, &m_mapTransvoxel[2], &m_hullVerticesCalc, &m_hullTrianglesCalc[3], true, 2);
        calculateTransvoxel(startX+1, m_sizeY+1, startZ+1, endX-1, m_sizeY+2, endZ-1, &m_map, &m_mapTransvoxel[3], &m_hullVerticesCalc, &m_hullTrianglesCalc[4], false, 2);

        calculateTransvoxel(1, startY+1, startZ+1, 2, endY-1, endZ-1, &m_map, &m_mapTransvoxel[4], &m_hullVerticesCalc, &m_hullTrianglesCalc[5], false, 0);
        calculateTransvoxel(m_sizeX+1, startY+1, startZ+1, m_sizeX+2, endY-1, endZ-1, &m_map, &m_mapTransvoxel[5], &m_hullVerticesCalc, &m_hullTrianglesCalc[6], true, 0);

    }

#ifdef DEBUG
    std::cout << "Part3 m_map.end" << std::endl;
#endif
    m_map.end(); // voxels get deleted - save lots of memory
    if (m_calcTransvoxel)
    {
        for (entropy::uint16 ind = 0; ind < 6; ++ind)
            m_mapTransvoxel[ind].end();
    }
#ifdef DEBUG
    std::cout << "Part3 CopyCalcToRelease" << std::endl;
#endif
    copyCalcToRelease();
#ifdef DEBUG
    std::cout << "END Part3::calculateGeometry() END" << std::endl;
#endif
    m_sigPartChanged();

    //    BOUT("calculateGeometry(bool reduce) done num Triangles: " + blub::string::number(m_hullTriangles.size()));
}


Ogre::AxisAlignedBox Part3::aabb()
{
    return Ogre::AxisAlignedBox(m_startX*m_scale,
                                m_startY*m_scale,
                                m_startZ*m_scale,
                                m_startX*m_scale + m_sizeX*m_scale,
                                m_startY*m_scale + m_sizeY*m_scale,
                                m_startZ*m_scale + m_sizeZ*m_scale);
}

void Part3::copyCalcToRelease()
{
    m_numVertices = 0;
    for (entropy::uint16 ind = 0; ind < 1+6; ++ind)
        m_numTriangles[ind] = 0;

    entropy::uint32 numVertices = m_hullVerticesCalc.size();
    // no vertices --> no indices
    if (numVertices == 0)
        return;
    m_hullVerticesRelease = Ogre::Vector3Ptr(new std::vector<Ogre::Vector3>());
    m_hullVerticesRelease->reserve(numVertices);
    m_hullNormalsRelease = Ogre::Vector3Ptr(new std::vector<Ogre::Vector3>());
    m_hullNormalsRelease->reserve(numVertices);

    std::map<entropy::uint32, entropy::uint32> vertexIndices;
    //    boost::unordered::unordered_map<entropy::uint32,entropy::uint32> vertexIndices;
    //    vertexIndices.
    //    vertexIndices.rehash(ceil(numVertices / vertexIndices.max_load_factor()));
    for (t_hullVerticesCalc::iterator it = m_hullVerticesCalc.begin(); it != m_hullVerticesCalc.end(); ++it)
    {
        Ogre::Vector3 pos = (*it).second.pos-Ogre::Vector3(1.,1.,1.)*m_scale;
        Ogre::Vector3 normal = (*it).second.normal;
        m_hullVerticesRelease->push_back(pos);
        m_hullNormalsRelease->push_back(normal.normalisedCopy());
        vertexIndices[(*it).first] = m_numVertices;
        ++m_numVertices;
    }

    // calc triangles
    for (entropy::uint16 ind = 0; ind < 1+6; ++ind)
    {
        entropy::uint32 numTriangles = m_hullTrianglesCalc[ind].size();
        m_hullTrianglesRelease[ind] = Triangle::vectorPtr(new Triangle::vector(numTriangles,Triangle()));
        for (std::vector<Triangle>::iterator it = m_hullTrianglesCalc[ind].begin(); it != m_hullTrianglesCalc[ind].end(); ++it)
        {
            for (entropy::uint16 ind2 = 0; ind2 < 3; ++ind2)
            {
                if(vertexIndices.find((*it).ind[ind2]) != vertexIndices.end()) {
                    entropy::uint32 newId = (*(vertexIndices.find((*it).ind[ind2]))).second;
                    m_hullTrianglesRelease[ind]->at(m_numTriangles[ind]).ind[ind2] = newId;
                } else {
                    m_hullTrianglesRelease[ind]->at(m_numTriangles[ind]).ind[ind2] = 0;
                }

            }
            ++m_numTriangles[ind];
        }
    }
    // clean up
    m_hullVerticesCalc.clear();
    for (entropy::uint16 ind = 0; ind < 1+6; ++ind)
    {
        m_hullTrianglesCalc[ind].clear();
    }
}

Ogre::Vector3 Part3::calculateIntersection(entropy::uint32 nX, entropy::uint32 nY, entropy::uint32 nZ, entropy::uint16 corner0, entropy::uint16 corner1, Map3 *mp)
{
    Ogre::Vector3 offset(nX, nY, nZ);
    Ogre::Vector3 corn0 = calculateCorner(offset, corner0);
    Ogre::Vector3 corn1 = calculateCorner(offset, corner1);
    double val0 = voxelScalar(corn0.x, corn0.y, corn0.z, mp);
    double val1 = voxelScalar(corn1.x, corn1.y, corn1.z, mp);
    Ogre::Vector3 result = interpolate(corn0.x, corn0.y, corn0.z, corn1.x, corn1.y, corn1.z, val0, val1);
    return result;
}

Ogre::Vector3 Part3::calculateIntersectionTransvoxel(entropy::uint32 nX, entropy::uint32 nY, entropy::uint32 nZ, entropy::uint16 corner0, entropy::uint16 corner1, Map3 *mp, const int voxel[9][3])
{
    Ogre::Vector3 offset(nX, nY, nZ);
    Ogre::Vector3 corn0 = calculateCornerTransvoxel(offset, corner0, voxel);
    Ogre::Vector3 corn1 = calculateCornerTransvoxel(offset, corner1, voxel);
    double val0 = voxelScalarTrans(corn0.x*2.-1, corn0.y*2.-1, corn0.z*2.-1, mp);
    double val1 = voxelScalarTrans(corn1.x*2.-1, corn1.y*2.-1, corn1.z*2.-1, mp);
    Ogre::Vector3 result = interpolate(corn0.x, corn0.y, corn0.z, corn1.x, corn1.y, corn1.z, val0, val1);
    return result;
}

Ogre::Vector3 Part3::calculateCorner(Ogre::Vector3 offset, entropy::uint16 corner)
{
    switch (corner)
    {
    case 0:
        return offset;
    case 1:
        return offset + Ogre::Vector3(1.,0.,0.);
    case 2:
        return offset + Ogre::Vector3(0.,0.,1.);
    case 3:
        return offset + Ogre::Vector3(1.,0.,1.);
    case 4:
        return offset + Ogre::Vector3(0.,1.,0.);
    case 5:
        return offset + Ogre::Vector3(1.,1.,0.);
    case 6:
        return offset + Ogre::Vector3(0.,1.,1.);
    case 7:
        return offset + Ogre::Vector3(1.,1.,1.);
    default:
        // fatal
        assert(0);
    }
    // never reached
    assert(0);
    return Ogre::Vector3(0,0,0);
}

Ogre::Vector3 Part3::calculateCornerTransvoxel(Ogre::Vector3 offset, const entropy::uint16 corner, const int voxel[9][3])
{
    Ogre::Vector3 result;
    switch (corner)
    {
    case 0x0:
        result = Ogre::Vector3(0.,0.,0.);
        break;
    case 0x1:
        result = Ogre::Vector3(voxel[1][0], voxel[1][1], voxel[1][2]);
        break;
    case 0x2:
        result = Ogre::Vector3(voxel[2][0], voxel[2][1], voxel[2][2]);
        break;
    case 0x3:
        result = Ogre::Vector3(voxel[7][0], voxel[7][1], voxel[7][2]);
        break;
    case 0x4:
        result = Ogre::Vector3(voxel[8][0], voxel[8][1], voxel[8][2]);
        break;
    case 0x5:
        result = Ogre::Vector3(voxel[3][0], voxel[3][1], voxel[3][2]);
        break;
    case 0x6:
        result = Ogre::Vector3(voxel[6][0], voxel[6][1], voxel[6][2]);
        break;
    case 0x7:
        result = Ogre::Vector3(voxel[5][0], voxel[5][1], voxel[5][2]);
        break;
    case 0x8:
        result = Ogre::Vector3(voxel[4][0], voxel[4][1], voxel[4][2]);
        break;
    case 0x9:
    case 0xA:
    case 0xB:
    case 0xC:
        break;
    default:
        assert(0);
    }
    return offset + result / 2.;
}

void Part3::calculateMarchingCubes(int startX, int startY, int startZ, int endX, int endY, int endZ, Map3 *mp, boost::unordered::unordered_map<entropy::uint32, vertex> *verticesCalc, std::vector<Triangle> *triangleCalc)
{
    /*if (m_calcTransvoxel)
        return;
*/
    const double isoLevel(0.);
    for (entropy::uint32 z = startZ; z < (unsigned)endZ; ++z)
    {
        for (entropy::uint32 y = startY; y < (unsigned)endY; ++y)
        {
            for (entropy::uint32 x = startX; x < (unsigned)endX; ++x)
            {
                entropy::uint32 tableIndex = 0;
                if (voxelScalar(x, y, z, mp) < isoLevel)
                    tableIndex |= 1;
                if (voxelScalar(x+1, y, z, mp) < isoLevel)
                    tableIndex |= 2;
                if (voxelScalar(x, y, z+1, mp) < isoLevel)
                    tableIndex |= 4;
                if (voxelScalar(x+1, y, z+1, mp) < isoLevel)
                    tableIndex |= 8;
                if (voxelScalar(x, y+1, z, mp) < isoLevel)
                    tableIndex |= 16;
                if (voxelScalar(x+1, y+1, z, mp) < isoLevel)
                    tableIndex |= 32;
                if (voxelScalar(x, y+1, z+1, mp) < isoLevel)
                    tableIndex |= 64;
                if (voxelScalar(x+1, y+1, z+1, mp) < isoLevel)
                    tableIndex |= 128;

                // Now create a triangulation of the isosurface in this
                // cell.
                if (tableIndex != 0 && tableIndex != 255)
                {
                    const RegularCellData *data = &regularCellData[regularCellClass[tableIndex]];
                    entropy::uint32 ids[12];
                    for (entropy::uint16 ind = 0; ind < data->GetVertexCount(); ++ind)
                    {
                        entropy::uint16 data2 = regularVertexData[tableIndex][ind];
                        entropy::uint16 corner0 = data2 & 0x0F;
                        entropy::uint16 corner1 = (data2 & 0xF0) >> 4;
                        entropy::uint32 id = edgeID(x, y, z, data2 >> 8);
                        ids[ind]=id;
                        // entropy::uint16 owner = (data2 >> 8) & 0x0F;
                        if (/*owner == 8 || owner == 9) && */verticesCalc->find(id) == verticesCalc->end())
                        {
                            Ogre::Vector3 pt = calculateIntersection(x, y, z, corner0, corner1, mp);
                            // pt = (calculateCorner(vector3(x, y, z), corner0) + calculateCorner(vector3(x, y, z), corner1)) / 2.;
                            pt*=m_scale;
                            verticesCalc->insert(std::pair<entropy::uint32,Ogre::Vector3>(id,pt));
                        }
                    }
                    for (entropy::uint16 ind = 0; ind < data->GetTriangleCount()*3; ind+=3)
                    {
                        // calc normal
                        vertex *pos0(&(*verticesCalc)[ids[data->vertexIndex[(int)ind+0]]]);
                        vertex *pos1(&(*verticesCalc)[ids[data->vertexIndex[(int)ind+1]]]);
                        vertex *pos2(&(*verticesCalc)[ids[data->vertexIndex[(int)ind+2]]]);
                        if (pos0->pos == pos1->pos || pos1->pos == pos2->pos || pos0->pos == pos2->pos)
                            continue; // triangle with 0 space
                        Ogre::Vector3 addNormal = (pos1->pos - pos0->pos).crossProduct(pos2->pos - pos0->pos);
                        {
                            pos0->normal += addNormal;
                            pos1->normal += addNormal;
                            pos2->normal += addNormal;
                        }
                        if (triangleCalc)
                        {
                            // insert new triangle
                            Triangle tri(ids[data->vertexIndex[ind+0]],
                                         ids[data->vertexIndex[ind+1]],
                                         ids[data->vertexIndex[ind+2]]);
                            triangleCalc->push_back(tri);
                        }
                    }
                }
            }
        }
    }
}

void Part3::calculateTransvoxel(int startX, int startY, int startZ,
                                int endX, int endY, int endZ,
                                Map3 *mp, Map3 *mpTrans,
                                boost::unordered::unordered_map<entropy::uint32, vertex> *verticesCalc, std::vector<Triangle> *triangleCalc,
                                bool invertTriangles, int coord)
{
    (void)mp;
    const int voxel[][9][3] = {
        {{0, 0, 0},{0, 1, 0},{0, 2, 0},{0, 2, 1},{0, 2, 2},{0, 1, 2},{0, 0, 2},{0, 0, 1},{0, 1, 1}},
        {{0, 0, 0},{1, 0, 0},{2, 0, 0},{2, 1, 0},{2, 2, 0},{1, 2, 0},{0, 2, 0},{0, 1, 0},{1, 1, 0}},
        {{0, 0, 0},{1, 0, 0},{2, 0, 0},{2, 0, 1},{2, 0, 2},{1, 0, 2},{0, 0, 2},{0, 0, 1},{1, 0, 1}},
    };
    const double isoLevel(0.);
    for (entropy::uint32 z = startZ; z < (unsigned)endZ; ++z)
    {
        for (entropy::uint32 y = startY; y < (unsigned)endY; ++y)
        {
            for (entropy::uint32 x = startX; x < (unsigned)endX; ++x)
            {
                {
                    x*=2;y*=2;z*=2;
                    x-=1;y-=1;z-=1;
                    entropy::uint32 tableIndex = 0;
                    entropy::uint32 add = 1;
                    for (entropy::uint16 ind = 0; ind < 9; ++ind)
                    {
                        if (voxelScalarTrans(x+voxel[coord][ind][0], y+voxel[coord][ind][1], z+voxel[coord][ind][2], mpTrans) < isoLevel)
                            tableIndex |= add;
                        add*=2;
                    }

                    // litte "small voxel" versus "big voxel" check if same for y
                    /*#ifdef BLUB_DEBUG
                    bool first = (voxelScalarTrans(x,y,z, mpTrans) < isoLevel) == (voxelScalar((x+1)/2,(y+1)/2,(z+1)/2,mp) < isoLevel);
                    bool snd = (voxelScalarTrans(x+2,y,z, mpTrans) < isoLevel) == (voxelScalar((x+1)/2 +1 ,(y+1)/2,(z+1)/2,mp) < isoLevel);
                    bool third = (voxelScalarTrans(x,y,z+2, mpTrans) < isoLevel) == (voxelScalar((x+1)/2,(y+1)/2,(z+1)/2 +1 ,mp) < isoLevel);
                    bool fourth = (voxelScalarTrans(x+2,y,z+2, mpTrans) < isoLevel) == (voxelScalar((x+1)/2 +1 ,(y+1)/2,(z+1)/2 +1 ,mp) < isoLevel);
                    if (!first || !snd || !third || !fourth)
                        BERROR("!first || !snd || !third || !fourth");
#endif*/

                    x+=1;y+=1;z+=1;
                    x/=2;y/=2;z/=2;

                    entropy::uint32 classIndex = transitionCellClass[tableIndex];
                    const TransitionCellData *data = &transitionCellData[classIndex & 0x7F]; // only the last 7 bit count
                    entropy::uint32 ids[12];
                    Ogre::Vector3 normal(0,0,0);

                    for (entropy::uint16 ind = 0; ind < data->GetVertexCount(); ++ind)
                    {
                        entropy::uint16 data2 = transitionVertexData[tableIndex][ind];
                        entropy::uint16 edge = data2 >> 8;
                        entropy::uint16 edgeId = edge & 0x0F;
                        if (edgeId == 0x9 || edgeId == 0x8)
                        {
                            entropy::uint32 id;
                            if (coord == 1)
                                edgeId-=6;
                            if (coord == 2)
                            {
                                if (edgeId == 9)
                                    edgeId = 1;
                                else
                                    edgeId = 2;
                            }
                            if (coord == 0)
                            {
                                if (edgeId == 9)
                                    edgeId = 1;
                                else
                                    edgeId = 3;
                            }

                            entropy::uint16 owner = (edge & 0xF0) >> 4;
                            short diffX = owner%2;
                            short diffY = (owner >> 1)%2;
                            short diffZ = 1;

                            if (coord == 0)
                            {
                                diffX = 1;
                                diffY = owner%2;
                                diffZ = (owner >> 1)%2;
                            }
                            if (coord == 2)
                            {
                                diffY = 1;
                                diffZ = (owner >> 1)%2;
                            }

                            id = vertexID(x - diffX, y - diffY, z - diffZ) + (edgeId-1);
                            // id = vertexID(x, y, z) + 20 * (std::pow(19, 3) + 1) + ind;
                            ids[ind]=id;
                            t_hullVerticesCalc::iterator it = verticesCalc->find(id);
                            if (it != verticesCalc->end())
                                normal = (*it).second.normal;
                            else
                                std::cerr << "ERROR !!!!" << std::endl;
                        }
                    }
                    for (entropy::uint16 ind = 0; ind < data->GetVertexCount(); ++ind)
                    {
                        entropy::uint16 data2 = transitionVertexData[tableIndex][ind];
                        entropy::uint16 edge = data2 >> 8;
                        entropy::uint16 edgeId = edge & 0x0F;
                        if (!(edgeId == 0x9 || edgeId == 0x8))
                        {
                            entropy::uint32 id;
                            entropy::uint16 corner0 = data2 & 0x0F;
                            entropy::uint16 corner1 = (data2 & 0xF0) >> 4;
                            // vector3 pt = (calculateCornerTransvoxel(vector3(x, y, z), corner0) + calculateCornerTransvoxel(vector3(x, y, z), corner1)) / 2.;
                            Ogre::Vector3 pt = calculateIntersectionTransvoxel(x, y, z, corner0, corner1, mpTrans, voxel[coord]);
                            pt*=m_scale;

                            entropy::uint16 owner = (edge & 0xF0) >> 4;
                            entropy::uint16 diffX = owner%2;
                            entropy::uint16 diffY = (owner >> 1)%2;
                            entropy::uint16 diffZ = 1;
                            if (coord == 0)
                            {
                                diffX = 1;
                                diffY = owner%2;
                                diffZ = (owner >> 1)%2;
                            }
                            if (coord == 2)
                            {
                                diffY = 1;
                                diffZ = (owner >> 1)%2;
                            }

                            id = vertexID(x - diffX, y - diffY, z - diffZ) + (edgeId-1) + 20 * (coord+1) * (m_sizeX+3)*(m_sizeY+3)*(m_sizeZ+3);
                            if (owner == 4) // wont get reused
                                id = vertexID(x, y, z) + 200 * (coord+1) * (m_sizeX+3)*(m_sizeY+3)*(m_sizeZ+3) + ind;
//                            verticesCalc->insert(std::pair<entropy::uint32,vertex>(id,vertex(pt,normal)));
                            ((boost::unordered::unordered_map<entropy::uint32,vertex>)(*verticesCalc))[id] = vertex(pt,normal);
                            ids[ind]=id;
                        }
                    }
                    for (entropy::uint16 ind = 0; ind < data->GetTriangleCount()*3; ind+=3)
                    {
                        if (normal == Ogre::Vector3(0,0,0))
                        {
                            // calc normal
                            vertex *pos0(&(*verticesCalc)[ids[data->vertexIndex[ind+0]]]);
                            vertex *pos1(&(*verticesCalc)[ids[data->vertexIndex[ind+1]]]);
                            vertex *pos2(&(*verticesCalc)[ids[data->vertexIndex[ind+2]]]);
                            if (pos0->pos == pos1->pos || pos1->pos == pos2->pos || pos0->pos == pos2->pos)
                                continue; // triangle with 0 space
                            Ogre::Vector3 addNormal = (pos1->pos - pos0->pos).crossProduct(pos2->pos - pos0->pos);
                            {
                                pos0->normal += addNormal;
                                pos1->normal += addNormal;
                                pos2->normal += addNormal;
                            }
                        }

                        Triangle tri;
                        entropy::uint16 invert = 1;
                        if (invertTriangles)
                            invert = 0;
                        if ((classIndex >> 7) % 2 == invert)
                        {
                            tri = Triangle(ids[data->vertexIndex[ind+0]],
                                           ids[data->vertexIndex[ind+1]],
                                           ids[data->vertexIndex[ind+2]]);
                        }
                        else
                        {
                            tri = Triangle(ids[data->vertexIndex[ind+1]],
                                           ids[data->vertexIndex[ind+0]],
                                           ids[data->vertexIndex[ind+2]]);
                        }
                        triangleCalc->push_back(tri);
                    }
                }
            }
        }
    }
}

Ogre::Vector3 Part3::interpolate(double fX1, double fY1, double fZ1, double fX2, double fY2, double fZ2, double tVal1, double tVal2)
{
    Ogre::Vector3 interpolation;
    double mu;

    mu = ((0. - tVal1))/(tVal2 - tVal1);
    interpolation.x = fX1 + mu*(fX2 - fX1);
    interpolation.y = fY1 + mu*(fY2 - fY1);
    interpolation.z = fZ1 + mu*(fZ2 - fZ1);

    return interpolation;
}

double Part3::voxelScalar(entropy::uint32 x, entropy::uint32 y, entropy::uint32 z, Map3 *mp)
{
    Voxel *vox = mp->getVoxel(((int)x)-1, ((int)y)-1, ((int)z)-1);
    return vox->scalar;
}

double Part3::voxelScalarTrans(entropy::uint32 x, entropy::uint32 y, entropy::uint32 z, Map3 *mp)
{
    Voxel *vox = mp->getVoxel(((int)x)-1, ((int)y)-1, ((int)z)-1);
    return vox->scalar;
}
entropy::uint32 Part3::edgeID(entropy::uint32 nX, entropy::uint32 nY, entropy::uint32 nZ, entropy::uint16 edgeId)
{
    entropy::uint16 edge = edgeId & 0x0F;
    entropy::uint16 owner = (edgeId & 0xF0) >> 4;
    entropy::uint32 result(-1);
    entropy::uint16 diffX = owner%2;
    entropy::uint16 diffY = (owner >> 2)%2;
    entropy::uint16 diffZ = (owner >> 1)%2;
    result = vertexID(nX - diffX, nY - diffY, nZ - diffZ) + (edge-1);
    return result;
}
entropy::uint32 Part3::vertexID(entropy::uint32 nX, entropy::uint32 nY, entropy::uint32 nZ)
{
    return 20*(nZ*(m_sizeY + 3)*(m_sizeX + 3) + nY*(m_sizeX + 3) + nX);
}

Map3* Part3::map()
{
    return &m_map;
}

void Part3::clearGeometry(void)
{
    //    if (m_hullVerticesRelease != 0)
    //        delete [] m_hullVerticesRelease; m_hullVerticesRelease = 0;
//    m_hullVerticesRelease->clear();
    if(m_hullVerticesRelease!=Ogre::Vector3Ptr())
    {
        m_hullVerticesRelease->clear();
    }
    if(m_hullNormalsRelease!=Ogre::Vector3Ptr())
    {
        m_hullNormalsRelease->clear();
    }
    m_hullVerticesRelease.reset();
    m_hullNormalsRelease.reset();
    m_hullVerticesRelease = Ogre::Vector3Ptr();
    m_hullNormalsRelease = Ogre::Vector3Ptr();
    //    if (m_hullNormalsRelease != 0)
    //        delete [] m_hullNormalsRelease; m_hullNormalsRelease = 0;
    //    if (m_hullTrianglesRelease[0].size() >0)
    //        delete [] m_hullTrianglesRelease[0]; m_hullTrianglesRelease[0] = 0;
    for(entropy::uint16 i=0;i<1+6;++i)
    {
        if(m_hullTrianglesRelease.at(i)!=Triangle::vectorPtr())
        {
            m_hullTrianglesRelease[i]->clear();
            m_hullTrianglesRelease[i].reset();
            m_hullTrianglesRelease[i] = Triangle::vectorPtr();
        }
    }

    //EXPERIMENTAL
    m_hullVerticesCalc.clear();
    m_hullTrianglesCalc[0].clear();
}

void Part3::edit(boost::shared_ptr<Edit> ed)
{
    m_edits.push_back(ed);
}

Ogre::Vector3Ptr Part3::hullVertices()
{
    return m_hullVerticesRelease;
}

Ogre::Vector3Ptr Part3::hullNormals()
{
    return m_hullNormalsRelease;
}

Triangle::vectorPtr Part3::hullTriangles(entropy::uint16 ind)
{
    return m_hullTrianglesRelease[ind];
}

entropy::uint32 Part3::numVertices() const
{
    return m_numVertices;
}

entropy::uint32 Part3::numTriangles(entropy::uint16 ind) const
{
    return m_numTriangles[ind];
}

}
