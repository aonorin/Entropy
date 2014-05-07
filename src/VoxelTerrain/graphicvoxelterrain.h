#ifndef GRAPHICVOXELTERRAIN_H
#define GRAPHICVOXELTERRAIN_H

//#include <map>
#include "src/VoxelTerrain/mutex.h"
#include "src/Utility/octree.h"
#include "src/Utility/framelistener.h"
#include "OGRE/OgreAxisAlignedBox.h"
#include "boost/shared_ptr.hpp"
#include "src/Graphics/material.h"
#include "voxelterrain.h"
#include "src/Graphics/manualentity.h"
#include "OGRE/OgreSceneManager.h"
#include "src/Core/collisioncamera.h"
#include "boost/thread.hpp"
#include "voxelphysics.h"

namespace voxel {

class GraphicVoxelTerrain
//        : private utility::FrameListener
{
public:
    GraphicVoxelTerrain(VoxelTerrain *terrain, Ogre::SceneManager *sc, CollisionCamera *cam);
    ~GraphicVoxelTerrain();

    bool updateCameraPos(double);
    int counter;
    void start();
    void stop();
#ifdef BLUB_DEBUG
    double timeCalcVisible();
#endif
protected:
    // totally ansync and called parallel; called from voxelMap3threads
    void partChanged(entropy::uint16 lod, int id);
    // totally ansync but called from only one thread at a time; called from voxelMap3threads
    void partVoxelChanged(entropy::uint16 lod, int id, bool removePart);

    void run();
    bool running;
    boost::shared_ptr<boost::thread> thread;
    bool updateVisibility(double);
private:
    struct leaf
    {
        leaf(entropy::uint16 _lod, int _id, Ogre::AxisAlignedBox _aab, graphics::ManualEntity* _entity = 0, graphics::Material* _mat = 0)
            : lod(_lod)
            , id(_id)
            , aab(_aab)
            , entity(_entity)
            , mat(_mat)
            , loading(false)
            , shouldBeVisible(false)
            , triangleCount(0)
        {for (entropy::uint16 ind = 0; ind < 8; ++ind) hasVisilbeChild[ind] = true;}
        ~leaf()
        {
        }

        entropy::uint16 lod;
        int id;
        Ogre::AxisAlignedBox  aab;
        graphics::ManualEntity* entity;
        graphics::Material *mat;
        bool loading;
        bool shouldBeVisible;
        bool hasVisilbeChild[8];
        short indexSubEntities[6];
        int triangleCount;
    };
    typedef boost::shared_ptr<leaf> t_leaf;

    // lock tree before!
    static utility::Octree<t_leaf> *findNode(Ogre::AxisAlignedBox aab, utility::Octree<t_leaf> *tree) ;

    Ogre::AxisAlignedBox calcAabOneHigher(const Ogre::AxisAlignedBox& aab);
    bool isAabVisible(const Ogre::AxisAlignedBox& aab, const Ogre::Vector3& camPos, const entropy::uint16 lod, bool *tooFar);
    bool updateVisibilityRecursive(utility::Octree<t_leaf> *node, int *debugCounter);

    void partChangedGraphic(entropy::uint16 lod, int id);
    static short calcQuadrant(Ogre::AxisAlignedBox aab, Ogre::Vector3 check);

    // graphicThread
    void deleteEntityGraphic(graphics::ManualEntity*);
    // lock m_lockOctree before
    void deleteLeaf(t_leaf node);
    // lock m_lockOctree before
    void deleteEntity(t_leaf node);

    VoxelTerrain *m_terrain;
    Ogre::SceneManager *m_scene;
    CollisionCamera *m_camera;

    utility::mutex m_lockOctree;
    utility::Octree<t_leaf> m_octree;
    Ogre::Vector3 m_posCamera; // for async, gets locked by m_lockOctree

    graphics::Material *m_material;
    double *m_lodDistances;

#ifdef BLUB_DEBUG
    double m_timeCalcVisible;
#endif
};

}

#endif // GRAPHICVOXELTERRAIN_H
