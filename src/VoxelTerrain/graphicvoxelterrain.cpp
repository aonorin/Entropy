#include "graphicvoxelterrain.h"

#include "Graphics/manualentity.h"
#include "OGRE/OgreSceneManager.h"
#include "Graphics/entity.h"
#include "Utility/mutexlocker.h"
#include "Core/Entropy.h"
#include "OGRE/OgreMaterialManager.h"

#ifdef BLUB_DEBUG
#include <boost/chrono.hpp>
#endif

using namespace voxel;

GraphicVoxelTerrain::GraphicVoxelTerrain(VoxelTerrain *terrain, Ogre::SceneManager *sceneManager, CollisionCamera *cam)
    : m_terrain(terrain)
    , m_scene(sceneManager)
    , m_camera(cam)
    , m_material(0)
    , running(false)
    #ifdef BLUB_DEBUG
    , m_timeCalcVisible(0.)
    #endif
{
    m_lodDistances = new double[3];
    float lodScale = 40.0f;
    m_lodDistances[0] = 80.*lodScale;
    m_lodDistances[1] = 200.*lodScale;
    m_lodDistances[2] = 500.*lodScale;

    entropy::uint16 numLod = m_terrain->numLod();
    m_material = new graphics::Material[numLod];

    for (entropy::uint16 ind = 0; ind < numLod; ++ind)
    {
//                m_material[ind] = graphics::Material("Examples/Water7");
//        m_material[ind] = graphics::Material("DeferredDemo/RockWall");
        //        m_material[ind] = graphics::Material("Examples/OgreLogo");
        m_material[ind] = graphics::Material("SimplexMaterial");
    }



    m_terrain->addPartChangedListener(boost::bind(&GraphicVoxelTerrain::partChanged, this, _1, _2));
    m_terrain->addPartVoxelChangedListener(boost::bind(&GraphicVoxelTerrain::partVoxelChanged, this, _1, _2, _3));

    t_leaf data(new leaf(-1, -1, Ogre::AxisAlignedBox(Ogre::Vector3(-100000.,-100000.,-100000.), Ogre::Vector3(100000.,100000.,100000.))));
//    t_leaf data(new leaf(-1, -1, Ogre::AxisAlignedBox(Ogre::Vector3(0,0,0), Ogre::Vector3(1,1,1))));
    m_octree.data(data);

    //    addFrameListener(boost::bind(&GraphicVoxelTerrain::updateVisibility, this, _1));
    //    run(20, true);
    start();
}

void GraphicVoxelTerrain::start()
{
    if(running) {
        stop();
    }
    thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&GraphicVoxelTerrain::run,this)));
    running = true;
}

void GraphicVoxelTerrain::stop()
{
    running = false;
    thread->join();
}

void GraphicVoxelTerrain::run()
{
    while(running) {
        updateVisibility(0.0);
        boost::this_thread::sleep(boost::posix_time::milliseconds(50));
    }
}

GraphicVoxelTerrain::~GraphicVoxelTerrain()
{
    std::cout << "GraphicVoxelTerrain::~GraphicVoxelTerrain() " << std::endl;
    delete [] m_material;
}

void GraphicVoxelTerrain::partChanged(entropy::uint16 lod, entropy::int32 id)
{
    // graphic thread shall do
    //#ifdef DEBUG
    //    std::cout << "GraphicVoxelTerrain::partChanged("<<lod<<","<< id<<");"<<std::endl;
    //#endif
    //    Entropy::getSingletonPtr()->addToDo(boost::bind(&GraphicVoxelTerrain::partChangedGraphic, this, lod, id));
    try{
        Entropy::getSingletonPtr()->addQeueudFunction(boost::bind(&GraphicVoxelTerrain::partChangedGraphic, this, lod, id));
    } catch(std::exception &e) {
        std::cout << "Error adding partChanged function to queuedFunction thread: " << e.what() << std::endl;
    }

    //    m_scene->system()->addToDo(boost::bind(&GraphicVoxelTerrain::partChangedGraphic, this, lod, id));
}


void GraphicVoxelTerrain::deleteEntityGraphic(graphics::ManualEntity *ent)
{
    if (ent)
        delete ent;
}


void GraphicVoxelTerrain::deleteLeaf(t_leaf lf)
{
    deleteEntity(lf);
    lf->lod = -1;
    lf->id = -1;
    lf->loading = false;
    VoxelPhysics::getSingletonPtr()->removePhysicsObject(lf->lod,lf->id);
}

void GraphicVoxelTerrain::deleteEntity(t_leaf lf)
{
    if (lf->entity)
    {
#ifdef DEBUG
        std::cout << "Deleting Entity: " << lf->id << std::endl;
#endif
        //NEED TO REIMPLEMENT THIS LINE!!!!
        //        Entropy::getSingletonPtr()->addToDo(boost::bind(&GraphicVoxelTerrain::deleteEntityGraphic, this, lf->entity));
        Entropy::getSingletonPtr()->addQueuedDeleteManualEntity(lf->entity);
        //        delete lf->entity;
        //        m_scene->system()->addToDo(boost::bind(&GraphicVoxelTerrain::deleteEntityGraphic, this, lf->entity));
    }
    lf->entity = 0;
    lf->mat = 0;
    lf->loading = false;
}

void GraphicVoxelTerrain::partVoxelChanged(entropy::uint16 lod, entropy::int32 id, bool removePart)
{
    // voxelMap3Thread shall insert/remove part into octree
    // depending on distance it shall calculate hull
    utility::MutexLocker locker(m_lockOctree); (void)locker;
#ifdef DEBUG
    std::cout << "GraphicVoxelTerrain::partVoxelChanged(entropy::uint16 lod, entropy::int32 id, bool removePart)" << std::endl;
#endif
    typedef VoxelTerrain::t_part3Ptr t_part3Ptr;
    t_part3Ptr part = m_terrain->part(lod, id);

    if (part == t_part3Ptr())
    {
        // remove
        // fatal should not happen even if part gets removed
        return;
    }
    // (void)locker2; already locked by caller
    // mutexLocker locker2(*part.data());
    if (removePart)
    {
        utility::Octree<t_leaf> *child = findNode(part->aabb(), &m_octree);
        if (!child)
        {
            // already removed?
            std::cerr << "!child" << std::endl;
            return;
        }
        deleteLeaf(child->data());
    }
    else
    {
        // add
        utility::Octree<t_leaf> *current = &m_octree;
        Ogre::AxisAlignedBox aabNew = part->aabb();
        Ogre::AxisAlignedBox  aabCurrent = current->data()->aab;
        t_leaf lf(new leaf(lod, id, aabNew, 0));
        for (;;)
        {
            entropy::int16 quad = calcQuadrant(aabCurrent, aabNew.getCenter());
            // is in quad?
            if (quad != -1)
            {
                utility::Octree<t_leaf> **child = current->child(quad);
                // is there a chid in that quad
                if (*child)
                {
                    Ogre::AxisAlignedBox aabChild = (**child).data()->aab;
                    if (aabChild == aabNew)
                    {
                        // lod level
                        if ((**child).data()->entity)
                        {
                            /*m_scene->system()->addToDo(boost::bind(&voxelTerrain::deleteLeafGraphic, this, (**child).data()));*/
                            lf->entity = (**child).data()->entity;
                            lf->loading = true;
                            std::cout << "if ((**child).data()->entity)" << std::endl;
                            m_terrain->voxelMap(lod)->calculatePart(lf->id);
                        }
                        (**child).data(lf);
                        break;
                    }
                    else
                    {
                        // has child larger size?
                        if (aabChild.getSize().x > aabNew.getSize().x && calcQuadrant(aabChild, aabNew.getCenter()) != -1)
                        {
                            current->data()->hasVisilbeChild[quad] = true;
                            aabCurrent = aabChild;
                            current = *child;
                            continue;
                        }
                        else
                        {
                            // create octree between current and child
                            utility::Octree<t_leaf> *buffer = *child;
                            *child = new utility::Octree<t_leaf>();
                            entropy::uint32 sizeX = aabChild.getSize().x;
                            entropy::uint32 sizeY = aabChild.getSize().y;
                            entropy::uint32 sizeZ = aabChild.getSize().z;
                            Ogre::Vector3 minNew(aabChild.getMinimum() - Ogre::Vector3(((entropy::int32)aabChild.getMinimum().x)%(sizeX*2), ((entropy::int32)aabChild.getMinimum().y)%(sizeY*2), ((entropy::int32)aabChild.getMinimum().z)%(sizeZ*2)));
                            Ogre::AxisAlignedBox aabNew2(minNew, minNew + aabChild.getSize()*2.);
                            t_leaf lf2(new leaf(-1, -1, aabNew2, 0));
                            (*child)->data(lf2);
                            entropy::int16 quad2 = calcQuadrant(aabNew2, aabChild.getCenter());
                            if (quad2 == -1)
                            {
                                std::cerr << "fatal!: create octree between current and child" << std::endl;
                                return;
                            }
                            (*child)->child(quad2, buffer);
                            continue;
                        }
                    }
                }
                else
                {
                    // insert here
                    *child = new utility::Octree<t_leaf>();
                    (**child).data(lf);
                    break;
                }
            }
            else
            {
                std::cerr << "fatal!: if (quad != -1) else" << std::endl;
                return;
            }
        }
    }
}

short GraphicVoxelTerrain::calcQuadrant(Ogre::AxisAlignedBox aab, Ogre::Vector3 check)
{
    if (!aab.intersects(check))
    {
        return -1;
    }
    Ogre::Vector3 center(aab.getCenter());
    if (center.x < check.x && center.y < check.y && center.z < check.z)
        return 0;
    if (center.x < check.x && center.y < check.y && center.z > check.z)
        return 1;
    if (center.x < check.x && center.y > check.y && center.z < check.z)
        return 2;
    if (center.x < check.x && center.y > check.y && center.z > check.z)
        return 3;
    if (center.x > check.x && center.y < check.y && center.z < check.z)
        return 4;
    if (center.x > check.x && center.y < check.y && center.z > check.z)
        return 5;
    if (center.x > check.x && center.y > check.y && center.z < check.z)
        return 6;
    if (center.x > check.x && center.y > check.y && center.z > check.z)
        return 7;
    std::cerr << "fatal!: calcQuadrant" << std::endl;
    return -1;
}

utility::Octree< GraphicVoxelTerrain::t_leaf > *GraphicVoxelTerrain::findNode(Ogre::AxisAlignedBox aab, utility::Octree<t_leaf> *tree)
{
    if (!tree)
        return 0;
    if (tree->data()->aab == aab)
        return tree;
    if (tree->data()->aab.getSize().x < aab.getSize().x)
        return 0;
    short id = calcQuadrant(tree->data()->aab, aab.getCenter());
    if (id < 0)
        return 0;
    return findNode(aab, *tree->child(id));
}

void GraphicVoxelTerrain::partChangedGraphic(entropy::uint16 lod, entropy::int32 id)
{
    //    BWARNING("void voxelTerrain::partChangedGraphic(entropy::uint16 lod, entropy::int32 id)");
    /*if (lod != 0)
        return;*/

    // get part
    //#ifdef DEBUG
    //    std::cout << "GraphicVoxelTerrain::partChangedGraphic("<<lod<<","<<id<<");"<<std::endl;
    //#endif
    boost::shared_ptr<Part3> part = m_terrain->part(lod, id);
    if (part == boost::shared_ptr<Part3>())
    {
        // should not happen, but may occur because of async
        // untested because error quite difficult to construct
        //        std::cout << "part.isNull(): " << "lod: " << lod << " id: " << id <<std::endl;
        //        std::cout << "part.isNull(): " << "lod: " << lod << " id: " << id <<std::endl;
        return; // TODO : fix bool loading!!!
    } else if(part->numVertices()==0) {
        return;
    }

    utility::MutexLocker locker2(*part); (void)locker2; // dont switch locks... deadlock
    utility::MutexLocker locker(m_lockOctree); (void)locker;

    utility::Octree<t_leaf> *node = findNode(part->aabb(), &m_octree);
    if (!node)
    {
        // should not happen, but may occur because of async
        //        std::cerr << "ERROR !node" << std::endl;
        return;
    }
    t_leaf data = node->data();
    data->loading = false;

    graphics::ManualEntity* gr = data->entity;
    graphics::Material* mat = data->mat;
    uint16 numSubmeshes(1);

    if (lod > 0)
        numSubmeshes = 1+6;
    if (gr)
    {
        // deleteLeafGraphic(data); // not possible because of lock in it
        delete gr;
        gr = 0;
    }
    gr = new graphics::ManualEntity(m_scene, numSubmeshes);
    mat = &m_material[lod];

    //    Ogre::Vector3* positions = part->hullVertices();
    //    Ogre::Vector3* normals = part->hullNormals();
    if(part->hullVertices() == Ogre::Vector3Ptr()) {
        std::cout << "0 hull vertices, blank Part" << std::endl;
        return;
    }
//    gr->numVertices(part->hullVertices()->size());
//    gr->position(part->hullVertices(), lod);
//    gr->normal(part->hullNormals(), lod);

    // indices
    for (entropy::uint16 ind = 0; ind < numSubmeshes-1; ++ind)
        data->indexSubEntities[ind] = -1;
    entropy::uint16 counter(0);
    for (entropy::uint16 ind = 0; ind < numSubmeshes; ++ind)
    {
        entropy::uint32 num = part->numTriangles(ind);
        if (num == 0)
        {
            if (ind > 0) {
                data->indexSubEntities[ind-1] = -1;
            }
            data->triangleCount = 0;
            if(ind==0) {
                return;
            }
            continue;
        }
        data->triangleCount = num;
        gr->numTriangles(num, ind);
//        gr->triangle(part->hullTriangles(ind),ind);
        if (ind > 0)
        {
            data->indexSubEntities[ind-1] = counter;
        }

        ++counter;
    }

    Ogre::AxisAlignedBox aabb = part->aabb();
    // aabb
    gr->aabb(Ogre::AxisAlignedBox(Ogre::Vector3(0,0,0), aabb.getSize()));
    // pos
    gr->getNode()->setPosition(aabb.getMinimum());

    // (re)init Graphics
    gr->reInitGraphic(mat,part->hullVertices(),part->hullNormals(),part->hullTriangles(0));

    // save memory
    part->clearGeometry();

    // remove Buffer Geometry. Geometry is now in HardwareVertexBuffer, so delete buffer;
//    gr->deleteAllGeometry();

    // set all transvoxel invisible
    if (gr->entity() && lod > 0)
    {
        for (entropy::uint16 ind = 0; ind < 6; ++ind)
        {
            entropy::int16 id2 = data->indexSubEntities[ind];
            if (id2 > -1)
            {
                gr->entity()->subEntityVisible(id2, false);
            }
        }
    }

    // save gr
    data->entity = gr;
    data->mat = mat;
}

Ogre::AxisAlignedBox GraphicVoxelTerrain::calcAabOneHigher(const Ogre::AxisAlignedBox& aab)
{
    Ogre::Vector3 pos = aab.getMinimum();
    Ogre::Vector3 size = aab.getSize();
    // pos in higher lod
    size *= 2.;
    Ogre::Vector3 delta(std::abs(((int)pos.x)%(int)size.x),
                        std::abs(((int)pos.y)%(int)size.y),
                        std::abs(((int)pos.z)%(int)size.z)
                        );
    Ogre::Vector3 posOneLodHigher = pos - delta;
    return Ogre::AxisAlignedBox(posOneLodHigher, posOneLodHigher + size);
}

bool GraphicVoxelTerrain::isAabVisible(const Ogre::AxisAlignedBox& aab, const Ogre::Vector3 & camPos, const entropy::uint16 lod, bool *tooFarOut)
{
    Ogre::AxisAlignedBox aabbOneLodHigher = calcAabOneHigher(aab);

    /*double radiusAAb = aab.getCenter().squaredDistance(aab.getMaximum());
    double radius = aabbOneLodHigher.getCenter().squaredDistance(aabbOneLodHigher.getMaximum());
*/
    bool tooNear(false);
    bool tooFar(false);
    Ogre::Vector3 scaledBoxCenter = aab.getCenter()*GRAPHICS_SCALE;
    Ogre::Vector3 scaledOneLodHigherCenter = aabbOneLodHigher.getCenter()*GRAPHICS_SCALE;
    double near2 (scaledBoxCenter.squaredDistance(camPos));
    if (lod > 0)
    {
        tooNear = near2 <= m_lodDistances[lod-1]*m_lodDistances[lod-1];
    }
    double far2(scaledOneLodHigherCenter.squaredDistance(camPos));
    tooFar = far2 >= m_lodDistances[lod]*m_lodDistances[lod];
    if (tooFarOut)
        *tooFarOut = tooFar;


    return  !tooNear && !tooFar;
}

bool GraphicVoxelTerrain::updateVisibilityRecursive(utility::Octree<t_leaf> *node, int *debugCounter)
{
    if (!node)
        return false;
    ++(*debugCounter);
    //    std::cout << "GraphicVoxelTerrain::updateVisibilityRecursive(utility::Octree<t_leaf> *node, entropy::uint32 *debugCounter) "<< (*debugCounter) << std::endl;
    const entropy::uint16 numLod = m_terrain->numLod();
    const Ogre::Vector3 camPos = m_posCamera;

    t_leaf lf = node->data();
    entropy::uint16 lod = lf->lod;

    Ogre::AxisAlignedBox aabb = lf->aab;

    bool shouldBeVisible(false);
    /*bool tooNear(false);*/
    bool tooFar(false);
    if (lod < numLod)
    {
        // delete children
        if(lf->shouldBeVisible && lf->entity)
        {
            for (entropy::uint16 ind = 0; ind < 8; ++ind)
            {
                utility::Octree<t_leaf> *child = *node->child(ind);
                if (lf->hasVisilbeChild[ind] && child)
                {
                    if (child->data() == t_leaf())
                    {
                        if (child->data()->entity)
                        {
                            deleteEntity(child->data());
                        }
                    }
                }
            }
        }
        // BOUT("\n" + string::number(lf->id));
        shouldBeVisible = isAabVisible(aabb, camPos, lod, &tooFar);
        shouldBeVisible = shouldBeVisible && (lf->triangleCount > 0);
        /*
        shouldBeVisible = (lod == 1 && lf->id == 8421504) || (lod == 0 && !axisAlignedBox(vector3(0), vector3(32)).contains(aabb.center()));
        shouldBeVisible = (lod == 2);
        shouldBeVisible = (lod == 1 && axisAlignedBox(vector3(32), vector3(32+32)).contains(aabb.center())) || (lod == 0 && !axisAlignedBox(vector3(32), vector3(32+32)).contains(aabb.center()));
        shouldBeVisible = (lod == 1 && axisAlignedBox(vector3(0, 0, 32), vector3(32, 32, 64)).contains(aabb.center())) || (lod == 0 && !axisAlignedBox(vector3(0, 0, 32), vector3(32, 32, 64)).contains(aabb.center()));
*/        // shouldBeVisible = (lod == 2 && axisAlignedBox(vector3(0), vector3(64)).contains(aabb.center())) || (lod == 1 && !axisAlignedBox(vector3(0), vector3(64)).contains(aabb.center()));

        if (lf->id > -1)
        {
            if (lod > 0 && shouldBeVisible && lf->entity)
            {
                if (lf->entity->entity())
                {
                    const entropy::int16 indices[] = {0, 0, -1,
                                             0, 0, 1,
                                             0, -1, 0,
                                             0, 1, 0,
                                             -1, 0, 0,
                                             1, 0, 0};
                    // axisAlignedBox aabHigher = calcAabOneHigher(aabb);
                    for (entropy::uint16 ind = 0; ind < 6; ++ind)
                    {
                        Ogre::Vector3 vec = aabb.getSize()*Ogre::Vector3(indices[ind*3+0], indices[ind*3+1], indices[ind*3+2]);
                        Ogre::AxisAlignedBox neigh = Ogre::AxisAlignedBox(vec+aabb.getMinimum(),vec+aabb.getMaximum());
                        if (neigh.getCenter().squaredDistance(camPos) > aabb.getCenter().squaredDistance(camPos))
                            continue;
                        bool buffer = isAabVisible(neigh, camPos, lod, 0);
                        entropy::int16 id = lf->indexSubEntities[ind];
                        if (id > -1)
                        {
                            lf->entity->entity()->subEntityVisible(id, !buffer);
                        }
                    }
                }
            }

            lf->shouldBeVisible = shouldBeVisible;
            if (!lf->entity && !lf->loading && shouldBeVisible)
            {
                std::cout << "if (!lf->entity && !lf->loading && shouldBeVisible)" << std::endl;
                m_terrain->voxelMap(lod)->calculatePart(lf->id);
                lf->loading = true;
            }
            if (lf->entity && !shouldBeVisible && lod == numLod-1 && tooFar)
            {
                deleteEntity(lf);
//                VoxelPhysics::getSingletonPtr()->removePhysicsObject(lod,lf->id);
            }
        }
        else
            lf->shouldBeVisible = false;
    }

    bool childVis(false);
    for (entropy::uint16 ind = 0; ind < 8; ++ind)
    {
        if (!lf->hasVisilbeChild[ind] && tooFar)
        {
            continue;
        }
        bool childVis2 = updateVisibilityRecursive(*node->child(ind), debugCounter);
        lf->hasVisilbeChild[ind] = childVis2;
        childVis = childVis || childVis2;
    }


    // delete yourself
    if(!lf->shouldBeVisible && lf->entity && lf->lod > 0 && !tooFar)
    {
        bool del(true);
        for (entropy::uint16 ind = 0; ind < 8; ++ind)
        {
            utility::Octree<t_leaf> *child = *node->child(ind);
            if (child)
            {
                if (child->data() != t_leaf())
                {
                    if (!child->data()->entity && child->data()->shouldBeVisible && child->data()->id > -1)
                    {
                        del = false;
                        break;
                    }
                }
            }
        }
        if (del)
        {
            deleteEntity(lf);
//            VoxelPhysics::getSingletonPtr()->removePhysicsObject(lod,lf->id);
        }
    }



    return lf->entity != 0 || lf->loading || childVis;
}

bool GraphicVoxelTerrain::updateCameraPos(double)
{
    //    utility::MutexLocker locker(m_lockOctree); (void)locker;
    m_posCamera = Entropy::getSingletonPtr()->mCameraMan->getCamera()->getPosition();
    //    m_posCamera = m_camera->positionAbsolut();
    return true;
}

bool GraphicVoxelTerrain::updateVisibility(double)
{
    //    std::cout << "GraphicVoxelTerrain::updateVisibility(double)" << std::endl;
    /*#ifdef BLUB_DEBUG
    boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
#endif*/
    utility::MutexLocker locker(m_lockOctree); (void)locker;
    counter = 0;
    m_posCamera = Entropy::getSingletonPtr()->mCameraMan->getCamera()->getPosition();
    updateVisibilityRecursive(&m_octree, &(counter));
    /*#ifdef BLUB_DEBUG
    boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
    m_timeCalcVisible = sec.count();
#endif*/
    return true;
}

#ifdef BLUB_DEBUG
double GraphicVoxelTerrain::timeCalcVisible()
{
    return m_timeCalcVisible;
}
#endif


