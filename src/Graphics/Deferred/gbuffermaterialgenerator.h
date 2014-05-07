#ifndef GBUFFERMATERIALGENERATOR_H
#define GBUFFERMATERIALGENERATOR_H

#include "Graphics/Deferred/materialgenerator.h"

namespace graphics {

namespace deferred {

/** Class for generating materials for objects to render themselves to the GBuffer
 *  @note This does not support all the possible rendering techniques out there.
 *  in order to support more, either expand this class or make sure that objects
 *  that will not get treated correctly will not have materials generated for them.
 */
class GBufferMaterialGenerator : public MaterialGenerator
{
public:

    //Constructor
    GBufferMaterialGenerator();

    //The relevant options for objects that are rendered to the GBuffer
    enum GBufferPermutations
    {
        //(Regular) Textures
        GBP_NO_TEXTURES =			0x00000000,
        GBP_ONE_TEXTURE =			0x00000001,
        GBP_TWO_TEXTURES =			0x00000002,
        GBP_THREE_TEXTURES =		0x00000003,
        GBP_FOUR_TEXTURES =         0x00000004,
        GBP_TEXTURE_MASK =			0x0000000F,

        //Material properties
        GBP_HAS_DIFFUSE_COLOUR =    0x00000010,

        //The number of texture coordinate sets
        GBP_NO_TEXCOORDS =			0x00000000,
        GBP_ONE_TEXCOORD =			0x00000100,
        GBP_TWO_TEXCOORDS =			0x00000200,
        GBP_TEXCOORD_MASK =			0x00000700,

        //Do we have a normal map
        GBP_NORMAL_MAP =			0x00000800,
        //Do we have a specular map
        GBP_SPECULAR_MAP =			0x10000000,
        //Do we have a displacement map
        GBP_DISPLACEMENT_MAP =		0x20000000,
        //Do we have a occlusion map
        GBP_OCCLUSION_MAP =         0x40000000,
        //Do we have a cone map
        GBP_CONE_MAP =              0x80000000,
        //Are we skinned?
        GBP_SKINNED =				0x00010000
    };

    //The mask of the flags that matter for generating the fragment shader
    static const Ogre::uint32 FS_MASK =		0xF000FFFF;

    //The mask of the flags that matter for generating the vertex shader
    static const Ogre::uint32 VS_MASK =		0x00FFFF00;

    //The mask of the flags that matter for generating the material
    static const Ogre::uint32 MAT_MASK =	0xFF00FFFF;
};

} // deferred namespac

} // graphics namespace

#endif // GBUFFERMATERIALGENERATOR_H
