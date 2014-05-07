#include "gbufferschemehandler.h"
#include "OGRE/OgreMaterialManager.h"
#include "OGRE/OgreTechnique.h"

using namespace Ogre;

namespace graphics {

namespace deferred {

const String GBufferSchemeHandler::NORMAL_MAP_PATTERN = "normal";
const String GBufferSchemeHandler::SPECULAR_MAP_PATTERN = "specular";
const String GBufferSchemeHandler::DISPLACEMENT_MAP_PATTERN = "displacement";
const String GBufferSchemeHandler::OCCLUSION_MAP_PATTERN = "occlusion";
const String GBufferSchemeHandler::CONE_MAP_PATTERN = "cone";

Technique* GBufferSchemeHandler::handleSchemeNotFound(unsigned short schemeIndex,
                                                      const String& schemeName, Material* originalMaterial, unsigned short lodIndex,
                                                      const Renderable* rend)
{
    Ogre::MaterialManager& matMgr = Ogre::MaterialManager::getSingleton();
    String curSchemeName = matMgr.getActiveScheme();
    matMgr.setActiveScheme(MaterialManager::DEFAULT_SCHEME_NAME);
    Technique* originalTechnique = originalMaterial->getBestTechnique(lodIndex, rend);
    matMgr.setActiveScheme(curSchemeName);

    Technique* gBufferTech = originalMaterial->createTechnique();
    gBufferTech->removeAllPasses();
    gBufferTech->setSchemeName(schemeName);

    Technique* noGBufferTech = originalMaterial->createTechnique();
    noGBufferTech->removeAllPasses();
    noGBufferTech->setSchemeName("NoGBuffer");

    for (unsigned short i=0; i<originalTechnique->getNumPasses(); i++)
    {
        Pass* originalPass = originalTechnique->getPass(i);
        PassProperties props = inspectPass(originalPass, lodIndex, rend);

        if (!props.isDeferred)
        {
            //Just copy the technique so it gets rendered regularly
            Pass* clonePass = noGBufferTech->createPass();
            *clonePass = *originalPass;
            continue;
        }

        Pass* newPass = gBufferTech->createPass();
        MaterialGenerator::Perm perm = getPermutation(props);

        const Ogre::MaterialPtr& templateMat = mMaterialGenerator.getMaterial(perm);

        //We assume that the GBuffer technique contains only one pass. But its true.
        *newPass = *(templateMat->getTechnique(0)->getPass(0));
        fillPass(newPass, originalPass, props);
    }

    return gBufferTech;
}

bool GBufferSchemeHandler::checkNormalMap(
        TextureUnitState* tus, GBufferSchemeHandler::PassProperties& props)
{
    bool isNormal = false;
    Ogre::String lowerCaseAlias = tus->getTextureNameAlias();
    Ogre::StringUtil::toLowerCase(lowerCaseAlias);

    if (lowerCaseAlias.find(NORMAL_MAP_PATTERN) != Ogre::String::npos)
    {
        isNormal = true;
    }

    else
    {
        Ogre::String lowerCaseName = tus->getTextureName();
        Ogre::StringUtil::toLowerCase(lowerCaseName);

        if (lowerCaseName.find(NORMAL_MAP_PATTERN) != Ogre::String::npos)
        {
            isNormal = true;
        }
    }

    if (isNormal)
    {
        if (props.normalMap == 0)
        {
            props.normalMap = tus;
        }

        else
        {
            OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
                        "Multiple normal map patterns matches",
                        "GBufferSchemeHandler::inspectPass");
        }
    }

    return isNormal;
}

bool GBufferSchemeHandler::checkSpecularMap(
        TextureUnitState* tus, GBufferSchemeHandler::PassProperties& props)
{
    bool isSpecular = false;
    Ogre::String lowerCaseAlias = tus->getTextureNameAlias();
    Ogre::StringUtil::toLowerCase(lowerCaseAlias);

    if (lowerCaseAlias.find(SPECULAR_MAP_PATTERN) != Ogre::String::npos)
    {
        isSpecular = true;
    }

    else
    {
        Ogre::String lowerCaseName = tus->getTextureName();
        Ogre::StringUtil::toLowerCase(lowerCaseName);

        if (lowerCaseName.find(SPECULAR_MAP_PATTERN) != Ogre::String::npos)
        {
            std::cout << "isSpecular = true;" << std::endl;
            isSpecular = true;
        }
    }

    if (isSpecular)
    {
        if (props.specularMap == 0)
        {
            props.specularMap = tus;
        }

        else
        {
            OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
                        "Multiple specular map patterns matches",
                        "GBufferSchemeHandler::inspectPass");
        }
    }

    return isSpecular;
}

bool GBufferSchemeHandler::checkDisplacementMap(
        TextureUnitState* tus, GBufferSchemeHandler::PassProperties& props)
{
    bool isDisplacement = false;
    Ogre::String lowerCaseAlias = tus->getTextureNameAlias();
    Ogre::StringUtil::toLowerCase(lowerCaseAlias);

    if (lowerCaseAlias.find(DISPLACEMENT_MAP_PATTERN) != Ogre::String::npos)
    {
        isDisplacement = true;
    }

    else
    {
        Ogre::String lowerCaseName = tus->getTextureName();
        Ogre::StringUtil::toLowerCase(lowerCaseName);

        if (lowerCaseName.find(DISPLACEMENT_MAP_PATTERN) != Ogre::String::npos)
        {
            isDisplacement = true;
        }
    }

    if (isDisplacement)
    {
        if (props.displacementMap == 0)
        {
            props.displacementMap = tus;
        }

        else
        {
            OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
                        "Multiple displacement map patterns matches",
                        "GBufferSchemeHandler::inspectPass");
        }
    }

    return isDisplacement;
}

bool GBufferSchemeHandler::checkOcclusionMap(
        TextureUnitState* tus, GBufferSchemeHandler::PassProperties& props)
{
    bool isOcclusion = false;
    Ogre::String lowerCaseAlias = tus->getTextureNameAlias();
    Ogre::StringUtil::toLowerCase(lowerCaseAlias);

    if (lowerCaseAlias.find(OCCLUSION_MAP_PATTERN) != Ogre::String::npos)
    {
        isOcclusion = true;
    }

    else
    {
        Ogre::String lowerCaseName = tus->getTextureName();
        Ogre::StringUtil::toLowerCase(lowerCaseName);

        if (lowerCaseName.find(OCCLUSION_MAP_PATTERN) != Ogre::String::npos)
        {
            isOcclusion = true;
        }
    }

    if (isOcclusion)
    {
        if (props.occlusionMap == 0)
        {
            props.occlusionMap = tus;
        }

        else
        {
            OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
                        "Multiple occlusion map patterns matches",
                        "GBufferSchemeHandler::inspectPass");
        }
    }

    return isOcclusion;
}

bool GBufferSchemeHandler::checkConeMap(
        TextureUnitState* tus, GBufferSchemeHandler::PassProperties& props)
{
    bool isCone = false;
    Ogre::String lowerCaseAlias = tus->getTextureNameAlias();
    Ogre::StringUtil::toLowerCase(lowerCaseAlias);

    if (lowerCaseAlias.find(CONE_MAP_PATTERN) != Ogre::String::npos)
    {
        isCone = true;
    }

    else
    {
        Ogre::String lowerCaseName = tus->getTextureName();
        Ogre::StringUtil::toLowerCase(lowerCaseName);

        if (lowerCaseName.find(CONE_MAP_PATTERN) != Ogre::String::npos)
        {
            isCone = true;
        }
    }

    if (isCone)
    {
        if (props.coneMap == 0)
        {
            props.coneMap = tus;
        }

        else
        {
            OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
                        "Multiple cone map patterns matches",
                        "GBufferSchemeHandler::inspectPass");
        }
    }

    return isCone;
}

GBufferSchemeHandler::PassProperties GBufferSchemeHandler::inspectPass(
        Pass* pass, unsigned short lodIndex, const Renderable* rend)
{
    PassProperties props;
//    pass->getUserObjectBindings();
//    Ogre::UserObjectBindings::getUserAny();
//    Ogre::Any::
    //TODO : Use renderable to indicate wether this has skinning.
    //Probably use same const cast that renderSingleObject uses.
    if (pass->hasVertexProgram())
    {
        props.isSkinned = pass->getVertexProgram()->isSkeletalAnimationIncluded();
    }

    else
    {
        props.isSkinned = false;
    }

    for (unsigned short i=0; i<pass->getNumTextureUnitStates(); i++)
    {
        TextureUnitState* tus = pass->getTextureUnitState(i);
        if (!checkNormalMap(tus, props))
        {
            if(!checkSpecularMap(tus,props))
            {
                if(!checkDisplacementMap(tus,props))
                {
                    if(!checkOcclusionMap(tus,props))
                    {
                        if(!checkConeMap(tus,props))
                        {
                            props.regularTextures.push_back(tus);
                        }
                    }
                }
            }
        }

        if (tus->getEffects().size() > 0)
        {
            props.isDeferred = false;
        }

    }

    if (pass->getDiffuse() != ColourValue::White)
    {
        props.hasDiffuseColour = true;
    }

    //Check transparency
    if (pass->getDestBlendFactor() != Ogre::SBF_ZERO)
    {
        //TODO : Better ways to do this
        props.isDeferred = false;
    }

    return props;
}

MaterialGenerator::Perm GBufferSchemeHandler::getPermutation(const PassProperties& props)
{
    MaterialGenerator::Perm perm = 0;
    switch (props.regularTextures.size())
    {
    case 0:
        perm |= GBufferMaterialGenerator::GBP_NO_TEXTURES;

        if (props.normalMap !=0)
        {
            perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
        }
        else if (props.specularMap !=0)
        {
            perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
        }
        else if (props.displacementMap !=0)
        {
            perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
        }
        else if (props.occlusionMap !=0)
        {
            perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
        }
        else if (props.coneMap !=0)
        {
            perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
        }
        else
        {
            perm |= GBufferMaterialGenerator::GBP_NO_TEXCOORDS;
        }
        break;

    case 1:
        perm |= GBufferMaterialGenerator::GBP_ONE_TEXTURE;
        perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
        break;

    case 2:
        perm |= GBufferMaterialGenerator::GBP_TWO_TEXTURES;
        //TODO : When do we use two texcoords?
        perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
        break;

    case 3:
        perm |= GBufferMaterialGenerator::GBP_THREE_TEXTURES;
        perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
        break;
    case 4:
        perm |= GBufferMaterialGenerator::GBP_FOUR_TEXTURES;
        perm |= GBufferMaterialGenerator::GBP_ONE_TEXCOORD;
        break;
    default:
        OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
                    "Can not generate G-Buffer materials for '>3 regular-texture' objects",
                    "GBufferSchemeHandler::inspectPass");
    }

    if (props.isSkinned)
    {
        perm |= GBufferMaterialGenerator::GBP_SKINNED;
    }

    if (props.normalMap != 0)
    {
        perm |= GBufferMaterialGenerator::GBP_NORMAL_MAP;
    }

    if (props.specularMap != 0)
    {
        perm |= GBufferMaterialGenerator::GBP_SPECULAR_MAP;
    }

    if (props.displacementMap != 0)
    {
        perm |= GBufferMaterialGenerator::GBP_DISPLACEMENT_MAP;
    }

    if (props.hasDiffuseColour)
    {
        perm |= GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR;
    }

    if (props.occlusionMap != 0)
    {
        perm |= GBufferMaterialGenerator::GBP_OCCLUSION_MAP;
    }

    if (props.coneMap != 0)
    {
        perm |= GBufferMaterialGenerator::GBP_CONE_MAP;
    }

    return perm;
}

void GBufferSchemeHandler::fillPass(
        Pass* gBufferPass, Pass* originalPass, const PassProperties& props)
{
    //Reference the correct textures. Normal map first!
    int texUnitIndex = 0;
    if (props.normalMap != 0)
    {
        *(gBufferPass->getTextureUnitState(texUnitIndex)) = *(props.normalMap);
        texUnitIndex++;
    }

    if (props.specularMap != 0)
    {
        *(gBufferPass->getTextureUnitState(texUnitIndex)) = *(props.specularMap);
        texUnitIndex++;
    }

    if (props.displacementMap != 0)
    {
        *(gBufferPass->getTextureUnitState(texUnitIndex)) = *(props.displacementMap);
        texUnitIndex++;
    }

    if (props.occlusionMap != 0)
    {
        *(gBufferPass->getTextureUnitState(texUnitIndex)) = *(props.occlusionMap);
        texUnitIndex++;
    }

    if (props.coneMap != 0)
    {
        *(gBufferPass->getTextureUnitState(texUnitIndex)) = *(props.coneMap);
        texUnitIndex++;
    }

    for (size_t i=0; i<props.regularTextures.size(); i++)
    {
        *(gBufferPass->getTextureUnitState(texUnitIndex)) = *(props.regularTextures[i]);
        texUnitIndex++;
    }

    gBufferPass->setAmbient(originalPass->getAmbient());
    gBufferPass->setDiffuse(originalPass->getDiffuse());
    gBufferPass->setSpecular(originalPass->getSpecular());
    gBufferPass->setShininess(originalPass->getShininess());
    gBufferPass->setCullingMode(originalPass->getCullingMode());
    gBufferPass->setLightingEnabled(false);
}

} // deferred namespace

} // graphics namespace
