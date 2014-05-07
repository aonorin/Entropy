#include "GOOFShadows.h"
#include "GOOFDebugTextureManager.h"

using namespace Ogre;

namespace GOOF
{
	void Shadows::setupShadows()
	{
        SceneManager* sceneMgr = getSceneManager();		
        
		sceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED); // check SHADOWTYPE_TEXTURE_ADDITIVE
        
        
		sceneMgr->setShadowTexturePixelFormat(Ogre::PF_FLOAT32_R);
		sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 1);
		//sceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 1);
        sceneMgr->setShadowTextureConfig(0, 512, 512, PF_FLOAT32_R);
        
        sceneMgr->setShadowTextureCasterMaterial("ESMShadowCaster"); 
		sceneMgr->setShadowTextureSelfShadow(true);
        //sceneMgr->setShowDebugShadows(true);
        
        
        LiSPSMShadowCameraSetup* camSetup = new Ogre::LiSPSMShadowCameraSetup();
        camSetup->setCameraLightDirectionThreshold(Ogre::Degree(20));
        getShadowCameraSetupMap()["LiSPSM"] = ShadowCameraSetupPtr(camSetup);
        sceneMgr->setShadowCameraSetup( getShadowCameraSetupMap()["LiSPSM"] );
        
        sceneMgr->setShadowCasterRenderBackFaces(false);

        //DebugTextureManager::getSingleton().addShadowTextures(sceneMgr, 1);
        createTestScene();
        
        
        /*
		const unsigned numShadowRTTs = sceneMgr->getShadowTextureCount();
		
	    for (unsigned i = 0; i < numShadowRTTs; ++i)
		{
			printf("shadow tex %d\n", i);
	        Ogre::TexturePtr tex = sceneMgr->getShadowTexture(i);
	        Ogre::Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport(0);
            
			//Ogre::Real d = getCamera()->getFarClipDistance();
			//vp->setBackgroundColour(Ogre::ColourValue(d, d*d, d, d));
			vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1, 1));
	        vp->setClearEveryFrame(true);
		}
        */

        Ogre::Viewport* shadowMapViewport = sceneMgr->getShadowTexture(0)->getBuffer()->getRenderTarget()->getViewport(0);
        CompositorManager::getSingleton().addCompositor(shadowMapViewport, "ESM/BOXFILTER_5TAPS");
        CompositorManager::getSingleton().setCompositorEnabled(shadowMapViewport, "ESM/BOXFILTER_5TAPS", true);
		sceneMgr->addListener(this);
	}
	
	//-----------------------------------------------------------------------
	void Shadows::createRandomEntityClones(Entity* ent, size_t cloneCount, const Vector3& min, const Vector3& max)
    {
		SceneManager* sceneMgr = getSceneManager();		
        Entity *cloneEnt;
        for (size_t n = 0; n < cloneCount; ++n)
        {
            // Create a new node under the root
            SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode();
            // Random translate
            Vector3 nodePos;
            nodePos.x = Math::RangeRandom(min.x, max.x);
            nodePos.y = Math::RangeRandom(min.y, max.y);
            nodePos.z = Math::RangeRandom(min.z, max.z);
            node->setPosition(nodePos);
            cloneEnt = ent->clone(ent->getName() + "_clone" + StringConverter::toString(n));
            // Attach to new node
            node->attachObject(cloneEnt);
        }
    }
	
	//-----------------------------------------------------------------------
	void Shadows::createTestScene(bool useOgreMaterials)
	{		
		Ogre::String planeMatName, knotMatName;
		
		if(useOgreMaterials)
		{
			planeMatName = "PSSM/Plane";
			knotMatName  = "PSSM/Knot";
		}
		else 
		{
			planeMatName = "PSSMPlane";
			knotMatName  = "PSSMKnot";
		}		
		
        // temp 
        planeMatName = "ESM/Plane";
        knotMatName  = "ESM/Knot";
        
        SceneManager* sceneMgr = getSceneManager();		

        
        
        /*
		sceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
		Light* l = sceneMgr->createLight("Spot");
		l->setType(Light::LT_SPOTLIGHT);
		Vector3 dir(0.3, -1, 0.2);
		dir.normalise();
		l->setDirection(dir);
		l->setDiffuseColour(ColourValue(1.0, 1.0, 0.0, 1));
        l->setPosition(0, 25.0, 2.0);
        l->setSpotlightRange((Radian) 0.104, (Radian) 1.40, 1);
        l->setAttenuation(50, 1.0, 0.009, 0.0032);
        */
        


		// Create a basic plane to have something in the scene to look at
		Plane plane;
		plane.normal = Vector3::UNIT_Y;
		plane.d = 100;
		MeshPtr msh = MeshManager::getSingleton().createPlane("Myplane",
															  ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
															  4500,4500,100,100,true,1,40,40,Vector3::UNIT_Z);
		msh->buildTangentVectors(VES_TANGENT);
		Entity* pPlaneEnt;
		pPlaneEnt = sceneMgr->createEntity( "plane", "Myplane" );
		pPlaneEnt->setMaterialName(planeMatName);
		sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pPlaneEnt);


		Entity* ent = sceneMgr->createEntity("knot", "knot.mesh");
        ent->setMaterialName(knotMatName);
		createRandomEntityClones(ent, 20, Vector3(-100,0,-100), Vector3(100,0,100));

        SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode();
        node->attachObject(ent);
        //node->setPosition(Vector3(0, 0, 0));
        
        for(int i=0; i<sceneMgr->getRootSceneNode()->numChildren(); i++)
        {
            Node* node = sceneMgr->getRootSceneNode()->getChild(i);
            node->setScale(0.05f, 0.05f, 0.05f);
        }
	}
	//-----------------------------------------------------------------------
	void Shadows::shadowTextureCasterPreViewProj(Ogre::Light* light, Ogre::Camera* camera, size_t iteration)
	{
		// isn't going to work using an MaterialManager::setActiveScheme
		// see http://www.ogre3d.org/forums/viewtopic.php?f=4&t=50022
		// would need to do camera->getViewport()->setMaterialScheme()
		
		if(light->getType() == Light::LT_SPOTLIGHT)
		{
            float range = light->getAttenuationRange();
            camera->setNearClipDistance(range * 0.01);
            camera->setFarClipDistance(range);

			//camera->getViewport()->setMaterialScheme("Spotlight");
		}
		else if (light->getType() == Light::LT_DIRECTIONAL)
		{
			camera->getViewport()->setMaterialScheme("");
		}
	}
	//-----------------------------------------------------------------------
	void Shadows::shadowTextureReceiverPreViewProj(Ogre::Light* light, Ogre::Frustum* frustrum)
	{
		//MaterialManager::getSingleton().setActiveScheme("");
	}
}

