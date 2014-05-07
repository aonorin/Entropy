#ifndef HYPNOS_H
#define HYPNOS_H

#include "src/Core/Entropy.h"
#include "src/VoxelTerrain/terraingenerator.h"

class Hypnos : public Entropy
{
public:
    Hypnos();
    ~Hypnos();
    void createScene();
    void startNewGame();
    void purgeMemory();
    boost::shared_ptr<boost::thread> thread;
//    boost::shared_ptr<voxel::TerrainGenerator> tgen;
};

#endif // HYPNOS_H
