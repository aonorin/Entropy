#include "ShadowListener.h"
#include "OGRE/OgreShadowCameraSetup.h"
#include "OGRE/OgreShadowCameraSetupPSSM.h"
#include "OGRE/OgreCompositorManager.h"
#include "OGRE/OgreHardwarePixelBuffer.h"
#include "OGRE/OgreMaterialManager.h"

namespace graphics {

ShadowListener::ShadowListener(Ogre::SceneManager *sceneMgr) :
    sceneMgr(sceneMgr)
{
    setupShadows();
}

ShadowListener::~ShadowListener()
{
    // sceneMgr->removeListener(this);
}

void ShadowListener::setupShadows()
{
    /*
    sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 3);
    sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
    sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 3);
    sceneMgr->setShadowTextureCount(9);
    sceneMgr->setShadowTextureConfig(0, 512, 512, Ogre::PF_FLOAT16_RGB);
    sceneMgr->setShadowTextureConfig(1, 1024, 1024, Ogre::PF_FLOAT16_RGB);
    sceneMgr->setShadowTextureConfig(2, 1024, 1024, Ogre::PF_FLOAT16_RGB);
    sceneMgr->setShadowTextureSelfShadow(true);
    sceneMgr->setShadowTextureCasterMaterial("pssm_vsm_caster");
    sceneMgr->setShadowCasterRenderBackFaces(false);*/

    /*
    const unsigned int numShadowRTTs = sceneMgr->getShadowTextureCount();

    for(unsigned int i = 0; i < numShadowRTTs; ++i)
    {
        Ogre::TexturePtr tex = sceneMgr->getShadowTexture(i);
        Ogre::Viewport* vp = tex->getBuffer()->getRenderTarget()->getViewport(0);
        vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1, 1));
        vp->setClearEveryFrame(true);
    }*/

    /*
    Ogre::PSSMShadowCameraSetup* pssm = new Ogre::PSSMShadowCameraSetup();

    // Should normally be Camera near/far clip
    pssm->calculateSplitPoints(3, 1, 10000);

    pssm->setSplitPadding(5);

    pssm->setOptimalAdjustFactor(0, 2);
    pssm->setOptimalAdjustFactor(1, 1);
    pssm->setOptimalAdjustFactor(2, 0.5);




    shadowCameraSetup.bind(pssm);

    sceneMgr->setShadowCameraSetup(shadowCameraSetup);


    //sceneMgr->setShadowTexturePixelFormat(Ogre::PF_FLOAT32_RGB);

    sceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));*/


    Ogre::LiSPSMShadowCameraSetup* lispsm = new Ogre::LiSPSMShadowCameraSetup();
    lispsm->setCameraLightDirectionThreshold(Ogre::Degree(20));
    shadowCameraSetup.bind(lispsm);
    sceneMgr->setShadowCameraSetup(shadowCameraSetup);
    // sceneMgr->setShadowTextureCount(3);
    // sceneMgr->setShadowTextureSize(512);
    sceneMgr->setShadowTextureSelfShadow(true);
    sceneMgr->setShadowTextureCasterMaterial("ESMShadowCaster");
    sceneMgr->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_R);
    sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 1);
    sceneMgr->setShadowTextureConfig(0, 512, 512, Ogre::PF_FLOAT16_R);
    sceneMgr->setShadowCasterRenderBackFaces(false);
    sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
    sceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));



    /*

    // once the camera setup is finialises comment this section out and set the param_named in
    // the .program script with the values of splitPoints
    Ogre::Vector4 splitPoints;
    const Ogre::PSSMShadowCameraSetup::SplitPointList& splitPointList = pssm->getSplitPoints();

    for (int i = 0; i <= 3; ++i)
    {
        splitPoints[i] = splitPointList[i];
    }*/

    /*
    Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("pssm_vsm_material");


    for(int i = 0; i < mat->getNumTechniques(); ++i)
    {
        mat->getTechnique(i)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("pssmSplitPoints", splitPoints);
    }*/

    /*
    Ogre::ResourceManager::ResourceMapIterator it = Ogre::MaterialManager::getSingleton().getResourceIterator();


    while (it.hasMoreElements())
    {
        Ogre::MaterialPtr mat = it.getNext();
        if (mat->getNumTechniques() > 0 &&
            mat->getTechnique(0)->getPass("EntropyPSVSMReceiverPass") != NULL)
        {
            std::cout << "Setting split points for " << mat->getName() << " to: " << "[" << splitPoints[0]
                      << ", " << splitPoints[1] << ", " << splitPoints[2] << ", " << splitPoints[3] << "]" << std::endl;
            //printf("set pssmSplitPoints %s\n", mat->getName().c_str());
            mat->getTechnique(0)->getPass("EntropyPSVSMReceiverPass")->getFragmentProgramParameters()->
            setNamedConstant("pssmSplitPoints", splitPoints);
        }
    }*/

    /*
    static const Ogre::String receiverPassName[2] =
    {
        "PSSMShadowReceiverDirectional",
        "PSSMShadowReceiverSpotlight",
    };

    //  &&
    // mat->getTechnique(0)->getPass(receiverPassName[i])->getFragmentProgramParameters()->
    // _findNamedConstantDefinition("pssmSplitPoints", false) != NULL


    while (it.hasMoreElements())
    {
        Ogre::MaterialPtr mat = it.getNext();
        for(int i=0; i < 2; i++)
        {
            if (mat->getNumTechniques() > 0 &&
                mat->getTechnique(0)->getPass(receiverPassName[i]) != NULL)
            {
                std::cout << "Setting split points for " << mat->getName() << " to: " << "[" << splitPoints[0]
                          << ", " << splitPoints[1] << ", " << splitPoints[2] << ", " << splitPoints[3] << "]" << std::endl;
                //printf("set pssmSplitPoints %s\n", mat->getName().c_str());
                mat->getTechnique(0)->getPass(receiverPassName[i])->getFragmentProgramParameters()->
                setNamedConstant("pssmSplitPoints", splitPoints);
            }
        }
    }*/

    Ogre::Viewport* shadowMapViewport = sceneMgr->getShadowTexture(0)->getBuffer()->getRenderTarget()->getViewport(0);
    Ogre::CompositorManager::getSingleton().addCompositor(shadowMapViewport, "ESM/BOXFILTER_5TAPS");
    Ogre::CompositorManager::getSingleton().setCompositorEnabled(shadowMapViewport, "ESM/BOXFILTER_5TAPS", true);

    // sceneMgr->addListener(this);
}

void ShadowListener::shadowTexturesUpdated(size_t numberOfShadowTextures)
{

}

void ShadowListener::shadowTextureCasterPreViewProj(Ogre::Light* light, Ogre::Camera* camera)
{

}

void ShadowListener::shadowTextureReceiverPreViewProj(Ogre::Light* light, Ogre::Frustum* frustum)
{

}

} // graphics namespace
