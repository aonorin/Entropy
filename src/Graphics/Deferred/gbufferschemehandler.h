#ifndef GBUFFERSCHEMEHANDLER_H
#define GBUFFERSCHEMEHANDLER_H

#include "OGRE/OgreMaterialManager.h"
#include "Graphics/Deferred/gbuffermaterialgenerator.h"

namespace graphics {

namespace deferred {

/** Class for handling materials who did not specify techniques for rendering
 *  themselves into the GBuffer. This class allows deferred shading to be used,
 *  without having to specify new techniques for all the objects in the scene.
 *  @note This does not support all the possible rendering techniques out there.
 *  in order to support more, either expand this class or specify the techniques
 *  in the materials.
 */
class GBufferSchemeHandler : public Ogre::MaterialManager::Listener
{
public:
    /** @copydoc MaterialManager::Listener::handleSchemeNotFound */
    virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex,
        const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex,
        const Ogre::Renderable* rend);

protected:
    //The material generator
    GBufferMaterialGenerator mMaterialGenerator;

    //The string that will be checked in textures to determine whether they are normal maps
    static const Ogre::String NORMAL_MAP_PATTERN;
    static const Ogre::String SPECULAR_MAP_PATTERN;
    static const Ogre::String DISPLACEMENT_MAP_PATTERN;
    static const Ogre::String OCCLUSION_MAP_PATTERN;
    static const Ogre::String CONE_MAP_PATTERN;

    //A structure for containing the properties of a material, relevant to GBuffer rendering
    //You might need to expand this class to support more options
    struct PassProperties
    {
        PassProperties() :
            isDeferred(true),
            normalMap(0),
            specularMap(0),
            displacementMap(0),
            occlusionMap(0),
            coneMap(0),
            specMul(1),
            isSkinned(false)
        {}

        bool isDeferred;
        Ogre::vector<Ogre::TextureUnitState*>::type regularTextures;
        Ogre::TextureUnitState* normalMap;
        Ogre::TextureUnitState* specularMap;
        Ogre::TextureUnitState* displacementMap;
        Ogre::TextureUnitState* occlusionMap;
        Ogre::TextureUnitState* coneMap;
        float specMul;
        bool isSkinned;
        bool hasDiffuseColour;

        //Example of possible extension : vertex colours
        //Ogre::TrackVertexColourType vertexColourType;
    };

    //Inspect a technique and return its relevant properties
    PassProperties inspectPass(Ogre::Pass* pass,
        unsigned short lodIndex, const Ogre::Renderable* rend);

    //Get the permutation of material flags that fit a certain property sheet
    MaterialGenerator::Perm getPermutation(const PassProperties& props);

    //Fill a pass with the specific data from the pass it is based on
    void fillPass(Ogre::Pass* gBufferPass, Ogre::Pass* originalPass, const PassProperties& props);

    //Check if a texture is a normal map, and fill property sheet accordingly
    bool checkNormalMap(Ogre::TextureUnitState* tus, PassProperties& props);

    //Check if a texture is a specular map, and fill property sheet accordingly
    bool checkSpecularMap(Ogre::TextureUnitState* tus, PassProperties& props);

    //Check if a texture is a displacement map, and fill property sheet accordingly
    bool checkDisplacementMap(Ogre::TextureUnitState* tus, PassProperties& props);

    //Check if a texture is a occlusion map, and fill property sheet accordingly
    bool checkOcclusionMap(Ogre::TextureUnitState* tus, PassProperties& props);

    //Check if a texture is a occlusion map, and fill property sheet accordingly
    bool checkConeMap(Ogre::TextureUnitState* tus, PassProperties& props);
};

} // deferred namespace

} // graphics namespace

#endif // GBUFFERSCHEMEHANDLER_H
