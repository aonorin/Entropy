#include "gbuffermaterialgenerator.h"
#include "OGRE/OgreMaterialManager.h"
#include "OGRE/OgreGpuProgramManager.h"
#include "OGRE/OgreStringConverter.h"
#include "OGRE/OgreHighLevelGpuProgramManager.h"
#include "OGRE/OgreTechnique.h"

//Use this directive to control whether you are writing projective (regular) or linear depth.
#define WRITE_LINEAR_DEPTH

namespace graphics {

namespace deferred {

//This is the concrete implementation of the material generator.
class GBufferMaterialGeneratorImpl : public MaterialGenerator::Impl
{
public:
    GBufferMaterialGeneratorImpl(const Ogre::String& baseName) : mBaseName(baseName) {}

protected:
    Ogre::String mBaseName;
    virtual Ogre::GpuProgramPtr generateVertexShader(MaterialGenerator::Perm permutation);
    virtual Ogre::GpuProgramPtr generateFragmentShader(MaterialGenerator::Perm permutation);
    virtual Ogre::MaterialPtr generateTemplateMaterial(MaterialGenerator::Perm permutation);

};

GBufferMaterialGenerator::GBufferMaterialGenerator() {
    vsMask = VS_MASK;
    fsMask = FS_MASK;
    matMask = MAT_MASK;
    materialBaseName = "DeferredShading/GBuffer/";
    mImpl = new GBufferMaterialGeneratorImpl(materialBaseName);
}

Ogre::GpuProgramPtr GBufferMaterialGeneratorImpl::generateVertexShader(MaterialGenerator::Perm permutation)
{
    Ogre::StringStream ss;

    ss << "void ToGBufferVP(" << std::endl;
    ss << "	float4 iPosition : POSITION," << std::endl;
    ss << "	float3 iNormal   : NORMAL," << std::endl;

    Ogre::uint32 numTexCoords = (permutation & GBufferMaterialGenerator::GBP_TEXCOORD_MASK) >> 8;
    for (Ogre::uint32 i=0; i<numTexCoords; i++)
    {
        ss << "	float2 iUV" << i << " : TEXCOORD" << i << ',' << std::endl;
    }

    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        ss << "	float3 iTangent : TANGENT0," << std::endl;
    }

    //TODO : Skinning inputs
    ss << std::endl;



    ss << "	out float4 oPosition : POSITION," << std::endl;
#ifdef WRITE_LINEAR_DEPTH
    ss << "	out float3 oViewPos : TEXCOORD0," << std::endl;
#else
    ss << "	out float oDepth : TEXCOORD0," << std::endl;
#endif
    ss << "	out float3 oNormal : TEXCOORD1," << std::endl;
    int texCoordNum = 2;
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        ss << "	out float3 oTangent : TEXCOORD" << texCoordNum++ << ',' << std::endl;
        ss << "	out float3 oBiNormal : TEXCOORD" << texCoordNum++ << ',' << std::endl;
        ss << " out float3 oEyeDir : TEXCOORD" << texCoordNum++ << "," << std::endl;
    }

    for (Ogre::uint32 i=0; i<numTexCoords; i++)
    {
        ss << "	out float2 oUV" << i << " : TEXCOORD" << texCoordNum++ << ',' << std::endl;
    }
    ss << "out float4 oObjNormal : TEXCOORD" << texCoordNum++ << "," << std::endl;
    ss << "out float4 oWorldPos : TEXCOORD" << texCoordNum++ << "," << std::endl;
    ss << std::endl;
    ss << " uniform float4 eyePosition," << std::endl;
    ss << "	uniform float4x4 cWorldViewProj," << std::endl;
    ss << "	uniform float4x4 cWorldView," << std::endl;
    ss << " uniform float4x4 cWorld" << std::endl;
    ss << "	)" << std::endl;


    ss << "{" << std::endl;
    ss << "	oPosition = mul(cWorldViewProj, iPosition);" << std::endl;
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
//        ss << "	oTangent = mul(cWorldView, float4(iTangent,0)).xyz;" << std::endl;
        ss << "	oNormal = mul(cWorldView, float4(iNormal,0)).xyz;" << std::endl;
//        ss << " oNormal = iNormal.xyz;" << std::endl;
        ss << "	oTangent =iTangent.xyz;" << std::endl;
        ss << "	oBiNormal = cross(oNormal, oTangent);" << std::endl;
        ss << " float3 eyeDir = eyePosition.xyz - iPosition.xyz;" << std::endl;
        ss << " float3 binormal = cross(iTangent, iNormal);" << std::endl;
        ss << " float3x3 rotation = float3x3(iTangent, binormal, iNormal);" << std::endl;
        ss << " eyeDir = mul(rotation, eyeDir);" << std::endl;
        //ss << " oEyeDir = eyeDir;" << std::endl;
        ss << " oEyeDir = eyePosition.xyz - iPosition.xyz;" << std::endl;
    }
    else
    {
        ss << "	oNormal = mul(cWorldView, float4(iNormal,0)).xyz;" << std::endl;
        //ss << " oNormal = iNormal.xyz;" << std::endl;
    }
    ss << " oObjNormal.xyz = iNormal;" << std::endl;
    ss << " oWorldPos = mul(cWorld,iPosition);"<<std::endl;
#ifdef WRITE_LINEAR_DEPTH
    ss << "	oViewPos = mul(cWorldView, iPosition).xyz;" << std::endl;
#else
    ss << "	oDepth = oPosition.w;" << std::endl;
#endif

    for (Ogre::uint32 i=0; i<numTexCoords; i++) {
        ss << "	oUV" << i << " = iUV" << i << ';' << std::endl;
    }

    ss << "}" << std::endl;

    Ogre::String programSource = ss.str();
    Ogre::String programName = mBaseName + "VP_" + Ogre::StringConverter::toString((int)permutation);

#if OGRE_DEBUG_MODE
    Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(programSource);
#endif

    // Create shader object
    Ogre::HighLevelGpuProgramPtr ptrProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                programName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                "cg", Ogre::GPT_VERTEX_PROGRAM);
    ptrProgram->setSource(programSource);
    ptrProgram->setParameter("entry_point","ToGBufferVP");
    ptrProgram->setParameter("profiles","vs_1_1 arbvp1");

    const Ogre::GpuProgramParametersSharedPtr& params = ptrProgram->getDefaultParameters();
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        params->setNamedAutoConstant("eyePosition",Ogre::GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);
    }
    params->setNamedAutoConstant("cWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
    params->setNamedAutoConstant("cWorldView", Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX);
    params->setNamedAutoConstant("cWorld", Ogre::GpuProgramParameters::ACT_WORLD_MATRIX);
    ptrProgram->load();

    return Ogre::GpuProgramPtr(ptrProgram);
}

Ogre::GpuProgramPtr GBufferMaterialGeneratorImpl::generateFragmentShader(MaterialGenerator::Perm permutation)
{
    Ogre::StringStream ss;

    ss << "void ToGBufferFP(" << std::endl;
#ifdef WRITE_LINEAR_DEPTH
    ss << "	float3 iViewPos : TEXCOORD0," << std::endl;
#else
    ss << "	float1 iDepth : TEXCOORD0," << std::endl;
#endif
    ss << "	float3 iNormal   : TEXCOORD1," << std::endl;

    int texCoordNum = 2;
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        ss << "	float3 iTangent : TEXCOORD" << texCoordNum++ << ',' << std::endl;
        ss << "	float3 iBiNormal : TEXCOORD" << texCoordNum++ << ',' << std::endl;
        ss << " float3 iEyeDir : TEXCOORD" << texCoordNum++ << "," << std::endl;
    }

    Ogre::uint32 numTexCoords = (permutation & GBufferMaterialGenerator::GBP_TEXCOORD_MASK) >> 8;
    for (Ogre::uint32 i=0; i<numTexCoords; i++)
    {
        ss << "	float2 iUV" << i << " : TEXCOORD" << texCoordNum++ << ',' << std::endl;
    }

    ss << " float4 iObjNormal : TEXCOORD" << texCoordNum++ << "," << std::endl;
    ss << " float4 iWorldPosition : TEXCOORD" << texCoordNum++ << "," << std::endl;
    //    ss << std::endl;

    ss << "	out float4 oColor0 : COLOR0," << std::endl;
    ss << "out float4 oColor1 : COLOR1," << std::endl;
    //FOR DISPLACEMENT MAP VALUES
    //ss << "	out float4 oColor2 : COLOR2," << std::endl;
    //ss << "	out float4 oColor3 : COLOR3," << std::endl;

    //    ss << std::endl;

    int samplerNum = 0;
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        ss << "	uniform sampler sNormalMap : register(s" << samplerNum++ << ")," << std::endl;
    }

    //SPECULAR MAP
    if (permutation & GBufferMaterialGenerator::GBP_SPECULAR_MAP)
    {
        ss << "	uniform sampler sSpecularMap : register(s" << samplerNum++ << ")," << std::endl;
    }
//    ss << "	uniform float cSpecularity," << std::endl;

    //DISPLACEMENT MAP
    if (permutation & GBufferMaterialGenerator::GBP_DISPLACEMENT_MAP)
    {
        std::cout << "if (permutation & GBufferMaterialGenerator::GBP_DISPLACEMENT_MAP)" << std::endl;
        ss << "	uniform sampler sDisplacementMap : register(s" << samplerNum++ << ")," << std::endl;
    }

    //OCCLUSION MAP
    if (permutation & GBufferMaterialGenerator::GBP_OCCLUSION_MAP)
    {
        ss << "	uniform sampler sOcclusionMap : register(s" << samplerNum++ << ")," << std::endl;
    }

    //Cone MAP
    if (permutation & GBufferMaterialGenerator::GBP_CONE_MAP)
    {
        ss << "	uniform sampler sConeMap : register(s" << samplerNum++ << ")," << std::endl;
    }

    Ogre::uint32 numTextures = permutation & GBufferMaterialGenerator::GBP_TEXTURE_MASK;
    for (Ogre::uint32 i=0; i<numTextures; i++) {
        ss << "	uniform sampler sTex" << i << " : register(s" << samplerNum++ << ")," << std::endl;
    }

    if (numTextures == 0 || permutation & GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR)
    {
        ss << "	uniform float4 cDiffuseColour," << std::endl;
    }
//    ss << " uniform float4x4 cWorldView," << std::endl;
#ifdef WRITE_LINEAR_DEPTH
    ss << "	uniform float cFarDistance" << std::endl;
#endif
    ss << "	)" << std::endl;

    ss << "{" << std::endl;
    if(numTexCoords==0)
    {
        ss << "float2 iUV0 = 0.0;" << std::endl;
    }

    ss << "iNormal = normalize(iNormal);" << std::endl;
    ss << "float tighten = 0.4679;" << std::endl;
    ss << "float mXY = (abs(iObjNormal.z) - tighten);" << std::endl;
    ss << "float mXZ = (abs(iObjNormal.y) - tighten);" << std::endl;
    ss << "float mYZ = (abs(iObjNormal.x) - tighten);" << std::endl;


    ss << "mXY = max(mXY,0);"<<std::endl;
    ss << "mXZ = max(mXZ,0);"<<std::endl;
    ss << "mYZ = max(mYZ,0);"<<std::endl;

    ss << "float total = mXY + mXZ + mYZ;" << std::endl;
    ss << "mXY = mXY/total;" << std::endl;
    ss << "mXZ = mXZ/total;" << std::endl;
    ss << "mYZ = mYZ/total;" << std::endl;

    ss << "float scale = 0.006;" << std::endl;
    ss << "float2 texCoord3 = float2(iWorldPosition.z,-iWorldPosition.y)*scale;" << std::endl;
    ss << "float2 texCoord2 = float2(iWorldPosition.x,-iWorldPosition.z)*scale;" << std::endl;
    ss << "float2 texCoord1 = float2(iWorldPosition.x,-iWorldPosition.y)*scale;" << std::endl;
    ss << "float3 flip = sign(iObjNormal.xyz);" << std::endl;
    ss << "texCoord1 *= flip.x;" << std::endl;
    ss << "texCoord2 *= flip.y;" << std::endl;
    ss << "texCoord3 *= -flip.z;" << std::endl;
    // - Tangent1: wrapped around Y (Valid for XY and ZY projected textures)
    // - Tangent2: wrapped around Z (Valid for XZ projected textures)
    ss << "float3 tangent1 = normalize(float3(-1 * iNormal.z,0,iNormal.x));" << std::endl;
    ss << "float3 tangent2 = normalize(float3(iNormal.y,-1 * iNormal.x,0));" << std::endl;
    ss << "float3 tangent1Obj = normalize(float3(-1 * iObjNormal.z,0,iObjNormal.x));" << std::endl;
    ss << "float3 tangent2Obj = normalize(float3(iObjNormal.y,-1 * iObjNormal.x,0));" << std::endl;
    //CONE MAP
    if (permutation & GBufferMaterialGenerator::GBP_CONE_MAP)
    {
        std::cout << "if (permutation & GBufferMaterialGenerator::GBP_CONE_MAP)" << std::endl;
        ss << "float3 p,p2,p20,p3,p30,v,v2,v3;" << std::endl;
        ss << "const int cone_steps = 5;" << std::endl;
        ss << "const int binary_steps = 7;" << std::endl;
        ss << "float db,db2,db3;" << std::endl;
        ss << "float3 p0;"<< std::endl;
        ss << "float dist,dist2,dist3;"<< std::endl;
        ss << "float4 tex,tex2,tex3;"<< std::endl;
        ss << "float height;"<< std::endl;
        ss << "float cone_ratio;"<< std::endl;
        //ss << "v = normalize(iEyeDir.xyz);"<< std::endl;
        ss << "float3x3 tbn1 = float3x3(tangent1,cross(tangent1Obj,iObjNormal),iObjNormal);" << std::endl;
        ss << "float3x3 tbn2 = float3x3(tangent2,cross(tangent2Obj,iObjNormal),iObjNormal);" << std::endl;
        ss << "iEyeDir = normalize(iEyeDir);" << std::endl;
        ss << "v = normalize(mul(tbn1,iEyeDir.xyz));" << std::endl;
        ss << "v2 = mul(tbn2,iEyeDir.xyz);" << std::endl;
        ss << "v3 = v;" << std::endl;

        //1
        ss << "v.z = abs(v.z);"<< std::endl;
        ss << "db = 1.0-v.z; db*=db; db*=db; db=1.0-db*db;"<< std::endl;
        ss << "v.xy *= db;"<< std::endl;
        //"v.xy *= parallaxDepth;"
        ss << "v.xy *= 0.15;"<< std::endl;
        ss << "v /= v.z;"<< std::endl;
        ss << "dist = length(v.xy);"<< std::endl;

        //2
        ss << "v2.z = abs(v2.z);"<< std::endl;
        ss << "db2 = 1.0-v2.z; db2*=db2; db2*=db2; db2=1.0-db2*db2;"<< std::endl;
        ss << "v2.xy *= db2;"<< std::endl;
        //"v.xy *= parallaxDepth;"
        ss << "v2.xy *= 0.15;"<< std::endl;
        ss << "v2 /= v2.z;"<< std::endl;
        ss << "dist2 = length(v2.xy);"<< std::endl;

        //3
        ss << "v3.z = abs(v3.z);"<< std::endl;
        ss << "db3 = 1.0-v3.z; db3*=db3; db3*=db3; db3=1.0-db3*db3;"<< std::endl;
        ss << "v3.xy *= db3;"<< std::endl;
        //"v.xy *= parallaxDepth;"
        ss << "v3.xy *= 0.15;"<< std::endl;
        ss << "v3 /= v3.z;"<< std::endl;
        ss << "dist3 = length(v3.xy);"<< std::endl;

        //ss << "float3 revertV = v;"<< std::endl;
        //ss << "float3 revertV2 = v2;"<< std::endl;
        //ss << "float3 revertV3 = v3;"<< std::endl;
        ss << "p = float3(texCoord1, 0);"<< std::endl;
        ss << "p0 = p;"<< std::endl;
        ss << "p2 = float3(texCoord2, 0);"<< std::endl;
        ss << "p20 = p2;"<< std::endl;
        ss << "p3 = float3(texCoord3, 0);"<< std::endl;
        ss << "p30 = p3;"<< std::endl;

        ss << "for( int i=0;i<cone_steps;i++ )"<< std::endl;
        ss << "{"<< std::endl;
        //1
        ss << "     tex.z = tex2D(sConeMap, p.xy).z;"<< std::endl;
        ss << "     tex.w = tex2D(sDisplacementMap, p.xy).r;"<< std::endl;
        ss << "     height = saturate(tex.w - p.z);"<< std::endl;
        ss << "     cone_ratio = tex.z;"<< std::endl;
        ss << "     p += v * (cone_ratio * height / (dist + cone_ratio));"<< std::endl;
        //2
        ss << "     tex2.z = tex2D(sConeMap, p2.xy).z;"<< std::endl;
        ss << "     tex2.w = tex2D(sDisplacementMap, p2.xy).r;"<< std::endl;
        ss << "     height = saturate(tex2.w - p2.z);"<< std::endl;
        ss << "     cone_ratio = tex2.z;"<< std::endl;
        ss << "     p2 += v2 * (cone_ratio * height / (dist2 + cone_ratio));"<< std::endl;
        //3
        ss << "     tex3.z = tex2D(sConeMap, p3.xy).z;"<< std::endl;
        ss << "     tex3.w = tex2D(sDisplacementMap, p3.xy).r;"<< std::endl;
        ss << "     height = saturate(tex3.w - p3.z);"<< std::endl;
        ss << "     cone_ratio = tex3.z;"<< std::endl;
        ss << "     p3 += v3 * (cone_ratio * height / (dist3 + cone_ratio));"<< std::endl;
        ss << "}"<< std::endl;

        ss << "v2 *= p2.z*0.5;"<< std::endl;
        ss << "p2 = p20 + v2;"<< std::endl;

        ss << "v3 *= p3.z*0.5;"<< std::endl;
        ss << "p3 = p30 + v3;"<< std::endl;

        ss << "v *= p.z*0.5;"<< std::endl;
        ss << "p = p0 + v;"<< std::endl;

        ss << "for( int i=0;i<binary_steps;i++ )"<< std::endl;
        ss << "{"<< std::endl;
        //1
        ss << "     tex.z = tex2D(sConeMap, p.xy).z;"<< std::endl;
        ss << "     tex.w = tex2D(sDisplacementMap, p.xy).r;"<< std::endl;
        ss << "     v *= 0.5;"<< std::endl;
        ss << "     if (p.z<tex.w){"<< std::endl;
        ss << "         p+=v;"<< std::endl;
        ss << "     }else{"<< std::endl;
        ss << "         p-=v;"<< std::endl;
        ss << "     }"<< std::endl;
        //2
        ss << "     tex2.z = tex2D(sConeMap, p2.xy).z;"<< std::endl;
        ss << "     tex2.w = tex2D(sDisplacementMap, p2.xy).r;"<< std::endl;
        ss << "     v2 *= 0.5;"<< std::endl;
        ss << "     if (p2.z<tex2.w){"<< std::endl;
        ss << "         p2+=v;"<< std::endl;
        ss << "     }else{"<< std::endl;
        ss << "         p2-=v;"<< std::endl;
        ss << "     }"<< std::endl;
        //3
        ss << "     tex3.z = tex2D(sConeMap, p3.xy).z;"<< std::endl;
        ss << "     tex3.w = tex2D(sDisplacementMap, p3.xy).r;"<< std::endl;
        ss << "     v *= 0.5;"<< std::endl;
        ss << "     if (p3.z<tex3.w){"<< std::endl;
        ss << "         p3+=v;"<< std::endl;
        ss << "     }else{"<< std::endl;
        ss << "         p3-=v;"<< std::endl;
        ss << "     }"<< std::endl;
        ss << "}"<< std::endl;
        ss << "texCoord1 = p.xy;" << std::endl;
        ss << "texCoord2 = p2.xy;" << std::endl;
        ss << "texCoord3 = p3.xy;" << std::endl;
        //ss << "float2 newTexCoord = p.xy*mXY+p2.xy*mXZ+p3.xy*mYZ;" << std::endl;
        //        ss << " float2 newTexCoord = ((normalize(iEyeDir.xyz) * displacement) + iUV0).xy;" << std::endl;
    }
    else
        //DISPLACEMENT MAP
        if (permutation & GBufferMaterialGenerator::GBP_DISPLACEMENT_MAP)
        {
            ss << "iEyeDir = normalize(iEyeDir);" << std::endl;
            ss << "float3x3 tbn1 = float3x3(tangent1,cross(tangent1Obj,iObjNormal),iObjNormal);" << std::endl;
            ss << "float3x3 tbn2 = float3x3(tangent2,cross(tangent2Obj,iObjNormal),iObjNormal);" << std::endl;
            ss << "float3 v = normalize(mul(tbn1,iEyeDir.xyz));" << std::endl;
            ss << "float3 v2 = normalize(mul(tbn2,iEyeDir.xyz));" << std::endl;
            ss << "float3 v3 = v;" << std::endl;
            ss << "	float displacement = tex2D(sDisplacementMap, texCoord1).r;" << std::endl;
            ss << " displacement = (displacement * 0.07) + -0.035;" << std::endl;
            ss << " texCoord1 = ((v.xyz * displacement) + texCoord1).xy;" << std::endl;

            ss << "	displacement = tex2D(sDisplacementMap, texCoord2).r;" << std::endl;
            ss << " displacement = (displacement * 0.07) + -0.035;" << std::endl;
            ss << " texCoord2 = ((v2.xyz * displacement) + texCoord2).xy;" << std::endl;

            ss << "	displacement = tex2D(sDisplacementMap, texCoord3).r;" << std::endl;
            ss << " displacement = (displacement * 0.07) + -0.035;" << std::endl;
            ss << " texCoord3 = ((v3.xyz * displacement) + texCoord3).xy;" << std::endl;
        }
        else
        {
            ss << " float2 newTexCoord = iUV0;" << std::endl;
            //ss << "	oColor2.a = 1.0;" << std::endl;
        }

    if ( numTextures > 0)
    {
        std::cout << "if (numTexCoords > 0 && numTextures > 0)" << std::endl;
        if(numTextures==3)
        {
//        ss << "	oColor0.rgb = tex2D(sTex0, newTexCoord);" << std::endl;
            ss << "float3 cXY = tex2D(sTex0,texCoord1).rgb;" << std::endl;
            ss << "float3 cXZ = tex2D(sTex1,texCoord2).rgb;" << std::endl;
            ss << "float3 cYZ = tex2D(sTex2,texCoord3).rgb;" << std::endl;
            ss << "oColor0.rgb = cXY*mXY + cXZ*mXZ + cYZ*mYZ;" << std::endl;
        }
        else if(numTextures==4)
        {
//        ss << "	oColor0.rgb = tex2D(sTex0, newTexCoord);" << std::endl;
            ss << "float3 cXY = tex2D(sTex0,texCoord1).rgb;" << std::endl;
            ss << "float3 cXZ;" << std::endl;
            ss << "if(iObjNormal.y>0) {" << std::endl;
            ss << "     cXZ = tex2D(sTex1,texCoord2).rgb;" << std::endl;
            ss << "} else {" << std::endl;
            ss << "     cXZ = tex2D(sTex3,texCoord2).rgb;" << std::endl;
            ss << "}" << std::endl;
            //ss << "float3 cXZ = tex2D(sTex1,texCoord2).rgb;" << std::endl;
            ss << "float3 cYZ = tex2D(sTex2,texCoord3).rgb;" << std::endl;
            ss << "oColor0.rgb = cXY*mXY + cXZ*mXZ + cYZ*mYZ;" << std::endl;
        }
        else
        {
            ss << "oColor0.rgb = tex2D(sTex0,iUV0).rgb;" << std::endl;
        }
        if (permutation & GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR)
        {
            ss << "	oColor0.rgb *= cDiffuseColour.rgb;" << std::endl;
        }
    }
    else
    {
        ss << "	oColor0.rgb = cDiffuseColour.rgb;" << std::endl;
    }

    //SPECULAR MAP
    if (permutation & GBufferMaterialGenerator::GBP_SPECULAR_MAP)
    {
        std::cout << "if (permutation & GBufferMaterialGenerator::GBP_SPECULAR_MAP)" << std::endl;
//        ss << "	oColor0.a = tex2D(sSpecularMap, newTexCoord).r * 8;" << std::endl;
        ss<< "float sXY = tex2D(sSpecularMap, texCoord1).r;" << std::endl;
        ss<< "float sXZ = tex2D(sSpecularMap, texCoord2).r;" << std::endl;
        ss<< "float sYZ = tex2D(sSpecularMap, texCoord3).r;" << std::endl;
        ss<< "oColor0.a = (sXY*mXY + sXZ*mXZ + sYZ*mYZ)*1;" << std::endl;
    }
    else
    {
        ss << "	oColor0.a = 1;" << std::endl;
    }
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {

//        ss << "oColor1.rgb = (tex2D(sNormalMap, newTexCoord).xyz-0.5)*2;" << std::endl;
        ss << "float3 nXY = (tex2D(sNormalMap,texCoord1).xyz-0.5)*2;" << std::endl;
        ss << "float3 nXZ = (tex2D(sNormalMap,texCoord2).xyz-0.5)*2;" << std::endl;
        ss << "float3 nYZ = (tex2D(sNormalMap,texCoord3).xyz-0.5)*2;" << std::endl;
        //ss << "	float3 texNormal = (tex2D(sNormalMap, newTexCoord)-0.5)*2;" << std::endl;
        ss << "float3 binormal1 = cross(tangent1,iNormal);"<< std::endl;
        ss << "float3 binormal2 = cross(tangent2,iNormal);"<< std::endl;
//        ss << "	float3x3 rotation1 = float3x3(tangent1*mXY+tangent2*mXZ+tangent1*mYZ, binormal1*mXY+binormal2*mXZ+binormal1*mYZ, iNormal);" << std::endl;
        ss << "	float3x3 rotation1 = float3x3(tangent1, binormal1, iNormal);" << std::endl;
        ss << "	float3x3 rotation2 = float3x3(tangent2, binormal2, iNormal);" << std::endl;
//        ss << "	float3x3 rotation3 = float3x3(tangent3, iBiNormal, iNormal);" << std::endl;
        ss << " nXY = normalize(mul(nXY,rotation1));" << std::endl;
        ss << " nXZ = normalize(mul(nXZ,rotation2));" << std::endl;
        ss << " nYZ = normalize(mul(nYZ,rotation1));" << std::endl;
        ss << "float3 normTotal = ((nXY*mXY + nXZ*mXZ + nYZ*mYZ));" << std::endl;
//        ss << "normTotal = mul(normTotal,rotation1);" << std::endl;
        ss << "normTotal = normalize(normTotal);" << std::endl;
        //ss << "oColor1.rgb = normalize(mul(cWorldView,float4(oColor1.rgb,0)));" << std::endl;
        ss << "oColor1.rgb = normTotal;" << std::endl;
        //ss << "	oColor2.rgb = iEyeDir.xyz;" << std::endl; //Tangent Data
        //ss << " oColor3.rgb = tangent1*mXY+tangent2*mXZ+tangent1*mYZ;" << std::endl;
    }
    else
    {
        //ss << " oColor3.rgb = float3(0.0,0.0,0.0);" << std::endl;
        ss << "	oColor1.rgb = iNormal;" << std::endl;
    }
    //ss << "	oColor2.x = iNormal.x;" << std::endl;
    //ss << " oColor2.y = iNormal.y;" << std::endl;
    //ss << " oColor3.a = iNormal.z;" << std::endl;
    if (permutation & GBufferMaterialGenerator::GBP_CONE_MAP)
    {
        //ss << " oColor2.z = tex2D(sConeMap, iUV0.xy).z;" << std::endl;
        //ss << " oColor2.w = tex2D(sDisplacementMap, iUV0.xy).r;" << std::endl;

        //ss << " iViewPos.xyz = (iViewPos)+float3(0,0,tex.w);" << std::endl;
    }
    else
    {
        //ss << "	oColor3.a = -1.0;" << std::endl;
    }

    if (permutation & GBufferMaterialGenerator::GBP_OCCLUSION_MAP)
    {
        //ss << "float3 occ1 = oColor0.rgb*tex2D(sOcclusionMap, texCoord1).rgb;" << std::endl;
        //ss << "float3 occ2 = oColor0.rgb*tex2D(sOcclusionMap, texCoord2).rgb;" << std::endl;
        //ss << "float3 occ3 = oColor0.rgb*tex2D(sOcclusionMap, texCoord3).rgb;" << std::endl;
        //ss << "oColor0.rgb = oColor0.rgb *0.5 + (oColor0.rgb*0.5 * (occ1*mXY+occ2*mXZ+occ3*mYZ));" << std::endl;
        //ss << "oColor0.rgb = oColor0.rgb * (occ1*mXY+occ2*mXZ+occ3*mYZ);" << std::endl;
    }
#ifdef WRITE_LINEAR_DEPTH
    ss << "oColor1.a = length(iViewPos) / cFarDistance;" << std::endl;
#else
    ss << "	oColor1.a = iDepth;" << std::endl;
#endif

    ss << "}" << std::endl;

    Ogre::String programSource = ss.str();
    Ogre::String programName = mBaseName + "FP_" + Ogre::StringConverter::toString((int)permutation);

#if OGRE_DEBUG_MODE
    Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(programSource);
#endif

    // Create shader object
    Ogre::HighLevelGpuProgramPtr ptrProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                programName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                "cg", Ogre::GPT_FRAGMENT_PROGRAM);
    ptrProgram->setSource(programSource);
    ptrProgram->setParameter("entry_point","ToGBufferFP");
    ptrProgram->setParameter("profiles","ps_2_0 arbfp1");

    const Ogre::GpuProgramParametersSharedPtr& params = ptrProgram->getDefaultParameters();

    //SPECULAR MAP
    //if (!(permutation & GBufferMaterialGenerator::GBP_SPECULAR_MAP))
    //{
    //    params->setNamedAutoConstant("cSpecularity", Ogre::GpuProgramParameters::ACT_SURFACE_SHININESS);
    //}
    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
//        params->setNamedAutoConstant("cWorldView",Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX);
    }
    if (numTextures == 0 || permutation & GBufferMaterialGenerator::GBP_HAS_DIFFUSE_COLOUR)
    {
        params->setNamedAutoConstant("cDiffuseColour", Ogre::GpuProgramParameters::ACT_SURFACE_DIFFUSE_COLOUR);
    }
    //params->setNamedAutoConstant("specMul");

#ifdef WRITE_LINEAR_DEPTH
    //TODO : Should this be the distance to the far corner, not the far clip distance?
    params->setNamedAutoConstant("cFarDistance", Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE);
#endif

    ptrProgram->load();
    return Ogre::GpuProgramPtr(ptrProgram);
}

Ogre::MaterialPtr GBufferMaterialGeneratorImpl::generateTemplateMaterial(MaterialGenerator::Perm permutation)
{
    Ogre::String matName = mBaseName + "Mat_" + Ogre::StringConverter::toString((int)permutation);

    Ogre::MaterialPtr matPtr = Ogre::MaterialManager::getSingleton().create
            (matName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::Pass* pass = matPtr->getTechnique(0)->getPass(0);
    pass->setName(mBaseName + "Pass_" + Ogre::StringConverter::toString((int)permutation));
    pass->setLightingEnabled(false);


    if (permutation & GBufferMaterialGenerator::GBP_NORMAL_MAP)
    {
        pass->createTextureUnitState();
    }

    if (permutation & GBufferMaterialGenerator::GBP_SPECULAR_MAP)
    {
        pass->createTextureUnitState();
    }

    if (permutation & GBufferMaterialGenerator::GBP_DISPLACEMENT_MAP)
    {
        pass->createTextureUnitState();
    }

    if (permutation & GBufferMaterialGenerator::GBP_OCCLUSION_MAP)
    {
        pass->createTextureUnitState();
    }

    if (permutation & GBufferMaterialGenerator::GBP_CONE_MAP)
    {
        pass->createTextureUnitState();
    }
    Ogre::uint32 numTextures = permutation & GBufferMaterialGenerator::GBP_TEXTURE_MASK;
    for (Ogre::uint32 i=0; i<numTextures; i++)
    {
        pass->createTextureUnitState();
    }
    std::cout << "numTextures: " << numTextures << std::endl;

    return matPtr;
}

} // deferred namespace

} // graphics namespace
