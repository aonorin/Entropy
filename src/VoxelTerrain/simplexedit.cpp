#include "simplexedit.h"
#include "ArtificialLife/simplexnoise1234.h"

using namespace voxel;


SimplexEdit::SimplexEdit(Ogre::Vector3 dimensions, Ogre::Vector3 pos, int seed) :
    AlgoEdit(dimensions, pos, seed),
    octaves(1),
    featureFreq(1.0/50.0),
    xMul(2),
    yMul(0.075),
    zMul(0.075)
{
    srand(seed);
    xRand = rand() % 10000;
    yRand = rand() % 10000;
    zRand = rand() % 10000;
}

double SimplexEdit::calcVoxel(const Ogre::Vector3 &pos, const double &scale)
{

    float finalValue = 0;
    float simplexValue = 0;

    //Create SimplexNoise
    //Iterate through Octaves
//    for(int i = 1; i<octaves;++i)
//    {
        //Find Feature
        float value = finalValue;
        float tempValue = SimplexNoise1234::noise(
                    (((((float)pos.x*featureFreq)*xMul)+(float)xRand)*2.0)*scale,
                    (((((float)pos.y*featureFreq)*yMul)+(float)yRand)*2.0)*scale,
                    (((((float)pos.z*featureFreq)*zMul)+(float)zRand)*2.0)*scale
                    );
        value += tempValue;
        simplexValue = value;
//    }
        return simplexValue;
}
