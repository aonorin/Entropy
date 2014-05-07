#ifndef __GOOFShadows_H
#define __GOOFShadows_H

#include "GOOFPrerequisites.h"
#include "GOOFSharedFrameworkData.h"
#include <Ogre.h>

namespace GOOF
{
	class Shadows :
		public SharedFrameworkData,
		public Ogre::SceneManager::Listener
	{
	public:
		virtual void setupShadows();

		virtual void shadowTextureCasterPreViewProj(Ogre::Light* light, Ogre::Camera* camera, size_t iteration);

		virtual void preFindVisibleObjects(Ogre::SceneManager* source,
								   Ogre::SceneManager::IlluminationRenderStage irs, Ogre::Viewport* v){}
		virtual void postFindVisibleObjects(Ogre::SceneManager* source,
									Ogre::SceneManager::IlluminationRenderStage irs, Ogre::Viewport* v){}
	    virtual void shadowTexturesUpdated(size_t) {}
		virtual void shadowTextureReceiverPreViewProj(Ogre::Light* light, Ogre::Frustum* frustrum);
		
	protected:
		void createTestScene(bool useOgreMaterials = false);
		
	private:
		void createRandomEntityClones(Ogre::Entity* ent, size_t cloneCount,
									  const Ogre::Vector3& min, const Ogre::Vector3& max);
	};
}
#endif
