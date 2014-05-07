#! /bin/bash
rm -r ../Hypnos-build-desktop/Media
cp /home/octopian/Documents/source/Ogre/Ogre-1.7/ogre/Samples/Media -r ../Hypnos-build-desktop

# Create the app bundle
rm -r ../Hypnos-build-desktop/Hypnos.app
mkdir ../Hypnos-build-desktop/Hypnos.app
mkdir ../Hypnos-build-desktop/Hypnos.app/Contents
mkdir ../Hypnos-build-desktop/Hypnos.app/Contents/Resources
mkdir ../Hypnos-build-desktop/Hypnos.app/Contents/Frameworks
mkdir ../Hypnos-build-desktop/Hypnos.app/Contents/Components
mkdir ../Hypnos-build-desktop/Hypnos.app/Contents/Plugins
mkdir ../Hypnos-build-desktop/Hypnos.app/Contents/LinuxOS

#RESOURCE_FILES.path = Contents/Resources
#    QMAKE_BUNDLE_DATA += RESOURCE_FILES

cp ../Entropy/resources -r ../Hypnos-build-desktop/Hypnos.app/Contents/Resources

#FRAMEWORKS.files = $${OGREHOME}/lib/release/Ogre.framework
#    FRAMEWORKS.path = Contents/Frameworks
#    QMAKE_BUNDLE_DATA += FRAMEWORKS

#cp /usr/local/lib/libOgrePaging.so.1.7.4 \
#	/usr/local/lib/libOgreProperty.so.1.7.4 \
#	/usr/local/lib/libOgreRTShaderSystem.so.1.7.4 \
#	/usr/local/lib/libOgreTerrain.so.1.7.4 ../Hypnos-build-desktop/Hypnos.app/Contents/Components

#cp /usr/local/lib/libOgreMain.so.1.7.4 ../Hypnos-build-desktop/Hypnos.app/Contents/LinuxOS

#cp /usr/local/lib/OGRE/Plugin_BSPSceneManager.so \
#	/usr/local/lib/OGRE/Plugin_CgProgramManager.so \
#	/usr/local/lib/OGRE/Plugin_OctreeZone.so \
#	/usr/local/lib/OGRE/Plugin_ParticleFX.so \
#	/usr/local/lib/OGRE/Plugin_PCZSceneManager.so \
#	/usr/local/lib/OGRE/RenderSystem_GL.so ../Hypnos-build-desktop/Hypnos.app/Contents/Plugins

cp ../Entropy/entropy_resources.cfg ../Hypnos-build-desktop/Hypnos.app/Contents/Resources
cp /home/octopian/Documents/source/Ogre/ogre/build/bin/plugins.cfg \
	/home/octopian/Documents/source/Ogre/ogre/build/bin/quakemap.cfg \
    /home/octopian/Documents/source/Ogre/ogre/build/bin/samples.cfg \
    ../Hypnos-build-desktop/Hypnos.app/Contents/Resources

cp ../Entropy/resources/OgreDebugDraw/Debug.material ../Hypnos-build-desktop/Media/materials/scripts
mv ../Hypnos-build-desktop/Hypnos ../Hypnos-build-desktop/Hypnos.app/Contents/LinuxOS
rm ../Hypnos-build-desktop/HypnosApp
ln -s ../Hypnos-build-desktop/Hypnos.app/Contents/LinuxOS/Hypnos ../Hypnos-build-desktop/HypnosApp

# TO DO: CREATE FRAMEWORKS BUNDLE OF LIBS THAT LINKS EVERYTHING RELATIVELY!

#MATERIALS.files += resources/OgreDebugDraw/Debug.material
#MATERIALS.path = ../Media/materials/scripts
#QMAKE_BUNDLE_DATA += MATERIALS

#ENTROPY_RESOURCES.files = resources
#ENTROPY_RESOURCES.path = Contents/Resources
#QMAKE_BUNDLE_DATA += ENTROPY_RESOURCES

#TEST_LEVEL.files = resources/scenes/TestLevel
#TEST_LEVEL.path = Contents/Resources/resources/scenes
#QMAKE_BUNDLE_DATA += TEST_LEVEL