#ifndef MEGABLOB_H
#define MEGABLOB_H

#include "blobedit.h"

namespace voxel
{

class MegaBlob
{
public:
    MegaBlob(int width, int height, int depth, float scalingFactor);

    std::vector<BlobEdit> blobs;
    void insertPoint(Ogre::Vector3 point, float value);

private:
    float width,height,depth,scalingFactor;
};

}

#endif // MEGABLOB_H
