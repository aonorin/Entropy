#TEMPLATE = app
TARGET = Entropy

CONFIG -= qt

#=============================================
#Lib stuff
#=============================================
TEMPLATE = lib
CONFIG = staticlib warn_on $$(QMAKESPEC)

message($$QMAKESPEC)
#=============================================
#/Lib stuff
#=============================================

QT -= core
QT -= gui

#CONFIG -= x86_64
#CONFIG += x86

include( ../QMakeVars.pri )

#DEFINES += BOOST_ALL_NO_LIB \
#    USE_RTSHADER_SYSTEM

# DEFINES += USE_RTSHADER_SYSTEM

QMAKE_CXXFLAGS_RELEASE += -IOgre -O3 -g
QMAKE_CXXFLAGS_DEBUG += -IOgre -O3 -g
CXXFLAGS_RELEASE += -visibility=default
CXXFLAGS_DEBUG += -visibility=default

QMAKE_CFLAGS += -msse
QMAKE_CXXFLAGS += -msse
CXXFLAGS += -F$${OGREHOME}/lib/Release \
    -Wl,-search_paths_first \
    -Wl,-headerpad_max_install_names

# Tiny XML Flag
QMAKE_CXXFLAGS += -DTIXML_USE_STL
CXXFLAGS += -DTIXML_USE_STL
QMAKE_CFLAGS += -DTIXML_USE_STL
CFLAGS += -DTIXML_USE_STL

CXXFLAGS += `sdl-config --cflags`

macx* {

    QMAKE_CFLAGS += -D__APPLE__
    CFLAGS += -D__APPLE__

    INCLUDEPATH += \
        $${OGREHOME}/include \
        $${OGREHOME}/include/Ogre \
        $${OGREHOME}/include/OIS \
        $${OGREHOME}/include/Ogre/OSX \
        $${OGREHOME}/include/Ogre/Threading \
        $${BOOSTINCLUDE} \
        $${BULLETINCLUDE} \
        $${SLBINCLUDE} \
        $${SLBINCLUDE}/../src \
        $${OISINCLUDE} \
        $${SDLINCLUDE} \
/System/Library/Frameworks/AppKit.framework/Headers \

    DEPENDPATH += \
        $${OGREHOME}/include \
        $${OGREHOME}/include/Ogre \
        $${OGREHOME}/include/Ogre/OSX \
        $${OGREHOME}/include/Ogre/Threading \
        $${BULLETINCLUDE}


    LIBS += -framework Carbon \
        -framework Cocoa \
        -framework Foundation \
        -framework IOKit \
        -framework AppKit \
        -framework Ogre \
        $${OGREHOME}/lib/RelWithDebInfo/libOgreRTShaderSystem.dylib \
        $${OGREHOME}/lib/RelWithDebInfo/libOgreTerrain.dylib \
        $${OGREHOME}/lib/RelWithDebInfo/libOgrePaging.dylib \
        Contents/Plugins/libSDL2.dylib \
        $${BOOSTLIBS}/libboost_date_time.a \
        $${BOOSTLIBS}/libboost_thread.a \
        $${BOOSTLIBS}/libboost_signals.a \
        $${BOOSTLIBS}/libboost_chrono.a \
        $${BOOSTLIBS}/libboost_system.a \
        $${SLBLIBS}/libSLB.a \
        $${BULLETLIBS}/libBulletMultiThreaded.a \
        $${BULLETLIBS}/libBulletDynamics.a \
        $${BULLETLIBS}/libBulletCollision.a \
        $${BULLETLIBS}/libBulletSoftBody.a \
        $${BULLETLIBS}/libLinearMath.a \
        $${BULLETLIBS}/libMiniCL.a \
        $${OISLIB}/libOIS.a \

PRE_TARGETDEPS += \
        $${BULLETLIBS}/libBulletMultiThreaded.a \
        $${BULLETLIBS}/libBulletDynamics.a \
        $${BULLETLIBS}/libBulletCollision.a \
        $${BULLETLIBS}/libBulletSoftBody.a \
        $${BULLETLIBS}/libLinearMath.a \
        $${BULLETLIBS}/libMiniCL.a \


    RESOURCE_FILES.files += $${OGREHOME}/bin/plugins.cfg \
        $${OGREHOME}/bin/quakemap.cfg \
        $${OGREHOME}/bin/samples.cfg \
        entropy_resources.cfg \

    RESOURCE_FILES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += RESOURCE_FILES
    OBJECTIVE_HEADERS += src/Core/NSRenderer.h
    OBJECTIVE_SOURCES += src/Core/NSRenderer.mm
}

INCLUDEPATH += /usr/local/include/bullet
INCLUDEPATH += /usr/local/include/

linux-g++* {
    QMAKE_CFLAGS += -D__LINUX__
    CFLAGS += -D__LINUX__
    QMAKE_CXXFLAGS += -D__LINUX__
    CXXFLAGS += -D__LINUX__
    DEFINES += __LINUX__
    QMAKE_CXXFLAGS += -std=c++11
    INCLUDEPATH += $${OGREINCLUDE}/include/OGRE \
        $${OGREINCLUDE}/include/OGRE/RTShaderSystem \
        $${OGREINCLUDE}/include \
        $${OGREINCLUDE}/common/include \
        $${OGREINCLUDE}/include/OGRE/OSX \
        $${OGREINCLUDE}/include/OGRE/RTShaderSystem \
        $${OGREINCLUDE}/include/Terrain \
        $${OGREINCLUDE}/include/Paging \
        $${OGREINCLUDE} \
        $${BOOSTINCLUDE} \
        $${BULLETINCLUDE}/BulletCollision \
        $${BULLETINCLUDE}/BulletSoftBody \
        $${BULLETINCLUDE}/vectormath \
        $${BULLETINCLUDE}/BulletDynamics \
        $${BULLETINCLUDE}/LinearMath \
        $${BULLETINCLUDE}/BulletMultiThreaded \
        $${BULLETINCLUDE}/MiniCL \
        $${BULLETINCLUDE} \
        /usr/local/include/bullet \
        $${SLBINCLUDE} \
        $${SLBINCLUDE}/../src \
        $${OISINCLUDE}
        #$${SDLINCLUDE} \

    INCLUDEPATH += /usr/include/OIS

    DEPENDPATH += $${BULLETLIBS} \
        $${BOOSTINCLUDE}

    DEPENDPATH += $${OGREINCLUDE}/include/OGRE \
        $${OGREINCLUDE}/include/OGRE/RTShaderSystem \
        $${OGREINCLUDE}/include \
        $${OGREINCLUDE}/common/include \
        $${OGREINCLUDE}/include/OGRE/OSX \
        $${OGREINCLUDE}/include/OGRE/RTShaderSystem \
        $${OGREINCLUDE}/include/Terrain \
        $${OGREINCLUDE}/include/Paging \
        $${OGREINCLUDE}/include/OIS \
        $${BULLETINCLUDE}/BulletCollision \
        $${BULLETINCLUDE}/BulletSoftBody \
        $${BULLETINCLUDE}/vectormath \
        $${BULLETINCLUDE}/BulletDynamics \
        $${BULLETINCLUDE}/LinearMath \
        $${BULLETINCLUDE}/BulletMultiThreaded \
        $${BULLETINCLUDE}/MiniCL \
        $${BULLETINCLUDE} \

    #LIBS += $${SDLLIBS}/libSDL2.a

    LIBS += $${OGREHOME}/lib/libOgreMain.so \
        $${OGREHOME}/lib/libOgrePaging.so \
        $${OGREHOME}/lib/libOgreProperty.so \
        $${OGREHOME}/lib/libOgreRTShaderSystem.so \
        $${OGREHOME}/lib/libOgreTerrain.so \
        $${OGREHOME}/lib/OGRE/Plugin_BSPSceneManager.so \
        $${OGREHOME}/lib/OGRE/Plugin_CgProgramManager.so \
        $${OGREHOME}/lib/OGRE/Plugin_OctreeSceneManager.so \
        $${OGREHOME}/lib/OGRE/Plugin_OctreeZone.so \
        $${OGREHOME}/lib/OGRE/Plugin_ParticleFX.so \
        $${OGREHOME}/lib/OGRE/Plugin_PCZSceneManager.so \
        $${OGREHOME}/lib/OGRE/RenderSystem_GL.so \
        /usr/lib/x86_64-linux-gnu/libOIS.a

    #INCLUDEPATH += $${OGREHOME}/include/OGRE \
    #    $${OGREHOME}/include/OGRE/RTShaderSystem \
    #    $${OGREHOME}/include/OIS \
    #    $${OGREHOME}/boost_1_46_1

    #LIBS += $${OGREHOME}/lib/libOgreMain.so \
    #    $${OGREHOME}/lib/libOIS.so
}

win32-msvc2010 {
    CONFIG += x86_64

    CONFIG+=exceptions
    QMAKE_CXXFLAGS += -fexceptions

    #DEBUGGING ENABLED
    QMAKE_CXXFLAGS += -IDL=0

    message("windows build")

    QMAKE_CFLAGS += -DWIN32
    CFLAGS += -DWIN32
    QMAKE_CXXFLAGS += -DWIN32
    CXXFLAGS += -DWIN32
    DEFINES += WIN32
    DEFINES += SLB_LIBRARY

    #LIBS += -L$${BOOSTLIBS}
    #Release:LIBS += -L$${OGREHOME}/lib/release
    #Debug:LIBS += -L$${OGREHOME}/lib/debug
    INCLUDEPATH += $${OGREHOME}/include
    INCLUDEPATH += $${OGREHOME}/include/OIS
    INCLUDEPATH += $${OGREHOME}/include/OGRE
    INCLUDEPATH += $${BOOSTINCLUDES} \
        $${BULLETINCLUDES} \

    INCLUDEPATH += $${ENTROPY_HOME}
    DEPENDPATH += $${ENTROPY_HOME}

    #INCLUDEPATH += $${ENTROPY_HOME}/libraries/SLB-2.00
    #DEPENDPATH += $${ENTROPY_HOME}/libraries/SLB-2.00

    DEPENDPATH += $${OGREHOME}/include
    DEPENDPATH += $${OGREHOME}/include/OIS
    DEPENDPATH += $${OGREHOME}/include/OGRE
    DEPENDPATH += $${BOOSTINCLUDES} \
        $${BULLETINCLUDES} \

    Release{
    LIBS += -static \
        $${OGREHOME}/lib/release/OgreMain.lib \
        $${OGREHOME}/lib/release/OgrePaging.lib \
        $${OGREHOME}/lib/release/OgreProperty.lib \
        $${OGREHOME}/lib/release/OgreRTShaderSystem.lib \
        $${OGREHOME}/lib/release/OgreTerrain.lib \
        $${OGREHOME}/lib/release/opt/Plugin_BSPSceneManager.lib \
        $${OGREHOME}/lib/release/opt/Plugin_CgProgramManager.lib \
        $${OGREHOME}/lib/release/opt/Plugin_OctreeSceneManager.lib \
        $${OGREHOME}/lib/release/opt/Plugin_OctreeZone.lib \
        $${OGREHOME}/lib/release/opt/Plugin_ParticleFX.lib \
        $${OGREHOME}/lib/release/opt/Plugin_PCZSceneManager.lib \
        $${OGREHOME}/lib/release/opt/RenderSystem_GL.lib \
        $${BULLETLIBS}/BulletCollision_vs2010_x64_release.lib \
        $${BULLETLIBS}/BulletDynamics_vs2010_x64_release.lib \
        #$${BULLETLIBS}/BulletMultiThreaded_vs2010_debug.lib \
        $${BULLETLIBS}/BulletSoftBody_vs2010_x64_release.lib \
        $${BULLETLIBS}/LinearMath_vs2010_x64_release.lib \
        #$${BULLETLIBS}/MiniCL/libMiniCL_vs_debug \
        $${BOOSTLIBS}/libboost_chrono-vc100-mt-1_49.lib \
        $${BOOSTLIBS}/libboost_thread-vc100-mt-1_49.lib \
        $${BOOSTLIBS}/libboost_system-vc100-mt-1_49.lib \
        $${BOOSTLIBS}/libboost_signals-vc100-mt-1_49.lib \
        $${BOOSTLIBS}/libboost_date_time-vc100-mt-1_49.lib \
        $${BOOSTLIBS}/libboost_regex-vc100-mt-1_49.lib \
        $${OGREDEPS}/lib/Release/OIS.lib \
    }

    Debug{
    LIBS += $${OGREHOME}/lib/debug/OgreMain_d.lib \
        $${OGREHOME}/lib/debug/OgrePaging_d.lib \
        $${OGREHOME}/lib/debug/OgreProperty_d.lib \
        $${OGREHOME}/lib/debug/OgreRTShaderSystem_d.lib \
        $${OGREHOME}/lib/debug/OgreTerrain_d.lib \
        $${OGREHOME}/lib/debug/opt/Plugin_BSPSceneManager_d.lib \
        $${OGREHOME}/lib/debug/opt/Plugin_CgProgramManager_d.lib \
        $${OGREHOME}/lib/debug/opt/Plugin_OctreeSceneManager_d.lib \
        $${OGREHOME}/lib/debug/opt/Plugin_OctreeZone_d.lib \
        $${OGREHOME}/lib/debug/opt/Plugin_ParticleFX_d.lib \
        $${OGREHOME}/lib/debug/opt/Plugin_PCZSceneManager_d.lib \
        $${OGREHOME}/lib/debug/opt/RenderSystem_GL_d.lib \
        $${BULLETLIBS}/BulletCollision_vs2010_debug.lib \
        $${BULLETLIBS}/BulletDynamics_vs2010_debug.lib \
        $${BULLETLIBS}/BulletSoftBody_vs2010_debug.lib \
        $${BULLETLIBS}/LinearMath_vs2010_debug.lib \
        $${BOOSTLIBS}/libboost_chrono-vc100-mt-gd-1_49.lib \
        $${BOOSTLIBS}/libboost_thread-vc100-mt-gd-1_49.lib \
        $${BOOSTLIBS}/libboost_system-vc100-mt-gd-1_49.lib \
        $${BOOSTLIBS}/libboost_signals-vc100-mt-gd-1_49.lib \
        $${BOOSTLIBS}/libboost_date_time-vc100-mt-gd-1_49.lib \
        $${BOOSTLIBS}/libboost_regex-vc100-mt-gd-1_49.lib \
        $${OGREDEPS}/lib/Release/OIS.lib \
    }

    #CONFIG(debug, debug|release) {
    #    TARGET = $$join(TARGET,,,d)
    #    LIBS *= -lOgreMain_d -lOIS_d
    #}

    #CONFIG(release, debug|release):LIBS *= -lOgreMain -lOIS

    MEDIA.files = $${OGREHOME}/Media
    MEDIA.path = ../Media
    QMAKE_BUNDLE_DATA += MEDIA
}

linux-g++* {

    COMPONENTS.files += $${OGREHOME}/lib/libOgrePaging.so \
        $${OGREHOME}/lib/libOgreProperty.so \
        $${OGREHOME}/lib/libOgreRTShaderSystem.so \
        $${OGREHOME}/lib/libOgreTerrain.so

    INCLUDEPATH += /libraries/ChatScript/src
    DEPENDPATH += /libraries/ChatScript/src
    LIBS += $${CHATSCRIPTHOME}/build/libChatScript.a
    INCLUDEPATH += ${OGREINCLUDE} \
    $${ENTROPYINCLUDE}

    COMPONENTS.path = Contents/Components
    QMAKE_BUNDLE_DATA += COMPONENTS



    #MEDIA.files = /usr/share/OGRE-1.8.0/media
    #MEDIA.path = ../Media
    #QMAKE_BUNDLE_DATA += MEDIA

LIBS += $${BOOSTLIBS}/libboost_date_time.a \
    $${BOOSTLIBS}/libboost_thread.a \
    $${BOOSTLIBS}/libboost_signals.a \
    $${BOOSTLIBS}/libboost_chrono.a \
    $${BOOSTLIBS}/libboost_system.a \
    $${BULLETLIBS}/BulletCollision/libBulletCollision.a \
    $${BULLETLIBS}/BulletDynamics/libBulletDynamics.a \
    $${BULLETLIBS}/BulletMultiThreaded/libBulletMultiThreaded.a \
    $${BULLETLIBS}/BulletSoftBody/libBulletSoftBody.a \
    $${BULLETLIBS}/LinearMath/libLinearMath.a \
    $${BULLETLIBS}/MiniCL/libMiniCL.a \
    #$${SLBLIBS}/libSLB.a \

    RESOURCE_FILES.files += $${OGREHOME}/bin/plugins.cfg \
        $${OGREHOME}/bin/quakemap.cfg \
        $${OGREHOME}/bin/samples.cfg \
        entropy_resources.cfg \

    RESOURCE_FILES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += RESOURCE_FILES

}

MATERIALS.files += resources/OgreDebugDraw/Debug.material
MATERIALS.path = ../Media/materials/scripts
QMAKE_BUNDLE_DATA += MATERIALS

ENTROPY_RESOURCES.files = resources
ENTROPY_RESOURCES.path = Contents/Resources
QMAKE_BUNDLE_DATA += ENTROPY_RESOURCES

TEST_LEVEL.files = resources/scenes/TestLevel
TEST_LEVEL.path = Contents/Resources/resources/scenes
QMAKE_BUNDLE_DATA += TEST_LEVEL

INCLUDEPATH += src
DEPENDPATH += src

HEADERS += \
    src/Core/BaseApplication.h \
    src/Core/terminal.h \
    src/neuro/virus.h \
    src/neuro/square.h \
    src/neuro/room.h \
    src/neuro/map.h \
    src/neuro/hero.h \
    src/neuro/Global.h \
    src/neuro/astar.h \
    src/Core/update.h \
    src/Physics/physics.h \
    src/Physics/world.h \
    src/Graphics/OgreDebugDraw/DebugDrawer.h \
    src/Graphics/graphics.h \
    src/Physics/physicsobject.h \
    src/Physics/physicstypes.h \
    src/Core/collisioncamera.h \
    src/neuro/port.h \
    src/neuro/program.h \
    src/Core/timedevent.h \
    src/neuro/node.h \
    src/neuro/projectile.h \
    src/Utility/Math.h \
    src/neuro/neuroai.h \
    src/neuro/phantom.h \
    src/Utility/Vector.h \
    src/Utility/geometry.h \
    src/Utility/Array.h \
    src/ArtificialLife/wolframca.h \
    src/ArtificialLife/gameoflife.h \
    src/ArtificialLife/Flocking.h \
    src/ArtificialLife/CellularAutomata.h \
    src/ArtificialLife/ArtificialLife.h \
    src/ArtificialLife/abstractca.h \
    src/Fractal/Strange.h \
    src/Fractal/LSystem.h \
    src/Fractal/FractalTypes.h \
    src/Fractal/Fractal.h \
    libraries/tinyxml/tinyxml.h \
    libraries/tinyxml/tinystr.h \
    src/Physics/scene.h \
    src/neuro/backtracer.h \
    src/Core/dotscene.h \
    src/Physics/eventdispatcher.h \
    src/Menu/gamemenu.h \
    src/Core/Entropy.h \
    src/Menu/entropylog.h \
    src/Menu/logfile.h \
    src/Menu/logfileserializer.h \
    src/Menu/logfilemanager.h \
    src/Menu/inventory.h \
    src/Menu/inventoryitem.h \
    src/Menu/questmenu.h \
    src/Menu/chatscript.h \
    src/Menu/chatmenu.h \
    src/Menu/menufuncs.h \
    libraries/ChatScript/src/variableSystem.h \
    libraries/ChatScript/src/userSystem.h \
    libraries/ChatScript/src/topicSystem.h \
    libraries/ChatScript/src/tokenSystem.h \
    libraries/ChatScript/src/textUtilities.h \
    libraries/ChatScript/src/testing.h \
    libraries/ChatScript/src/tagger.h \
    libraries/ChatScript/src/systemVariables.h \
    libraries/ChatScript/src/spellcheck.h \
    libraries/ChatScript/src/scriptCompile.h \
    libraries/ChatScript/src/readrawdata.h \
    libraries/ChatScript/src/patternSystem.h \
    libraries/ChatScript/src/outputSystem.h \
    libraries/ChatScript/src/markSystem.h \
    libraries/ChatScript/src/mainSystem.h \
    libraries/ChatScript/src/infer.h \
    libraries/ChatScript/src/functionExecute.h \
    libraries/ChatScript/src/factSystem.h \
    libraries/ChatScript/src/dictionarySystem.h \
    libraries/ChatScript/src/dictionaryMore.h \
    libraries/ChatScript/src/csocket.h \
    libraries/ChatScript/src/constructCode.h \
    libraries/ChatScript/src/common.h \
    src/Utility/WeakFunc.h \
    src/Menu/entropycursor.h \
    src/Script/script.h \
    src/Script/EntropyScript.hpp \
    src/Script/wrappers.h \
    src/gameplay/quest.h \
    src/gameplay/gameplay.h \
    src/Menu/console.h \
    src/gameplay/actors.h \
    src/gameplay/actorupdateactions.h \
    src/Menu/questentry.h \
    src/Menu/consolefilemanager.h \
    src/Menu/consolefile.h \
    src/Physics/collisiongroupmanager.h \
    src/Script/scripttypes.h \
    src/Script/luafunctor.h \
    src/Utility/OgreEuler.h \
    src/Graphics/DynamicsLines/DynamicsLines.h \
    src/Graphics/DynamicsLines/DynamicRenderable.h \
    src/Graphics/BulletDebugDraw.h \
    src/Utility/Triangle.h \
    src/gameplay/EventAction.h \
    src/gameplay/CollisionAction.h \
    src/Utility/ToDoListener.h \
    src/VoxelTerrain/map3.h \
    src/VoxelTerrain/voxel.h \
    src/VoxelTerrain/triangle.h \
    src/VoxelTerrain/mutex.h \
    src/VoxelTerrain/part3.h \
    src/VoxelTerrain/edit.h \
    src/VoxelTerrain/transvoxel.h \
    src/VoxelTerrain/editaxisalignedbox.h \
    src/VoxelTerrain/editsphere.h \
    src/VoxelTerrain/voxelmap.h \
    src/Utility/mutexlocker.h \
    src/VoxelTerrain/trianglevector.h \
    src/VoxelTerrain/Line.h \
    src/VoxelTerrain/quadVector3.h \
    src/VoxelTerrain/voxelterrain.h \
    src/Utility/framelistener.h \
    src/VoxelTerrain/graphicvoxelterrain.h \
    src/Utility/octree.h \
    src/Graphics/material.h \
    src/Graphics/manualentity.h \
    src/Graphics/manualobject.h \
    src/Graphics/entity.h \
    src/Graphics/Moveable.h \
    src/VoxelTerrain/terraingenerator.h \
    src/Graphics/ShadowListener.h \
    src/VoxelTerrain/blobedit.h \
    src/VoxelTerrain/chunkedit.h \
    src/ArtificialLife/diamondsquared.h \
    src/VoxelTerrain/caveedit.h \
    src/Graphics/Deferred/gbufferschemehandler.h \
    src/Graphics/Deferred/materialgenerator.h \
    src/Graphics/Deferred/gbuffermaterialgenerator.h \
    src/Graphics/Deferred/AmbientLight.h \
    src/Graphics/Deferred/SSAOLogic.h \
    src/Graphics/Deferred/NullSchemeHandler.h \
    src/Graphics/Deferred/ListenerFactoryLogic.h \
    src/Graphics/Deferred/LightMaterialGenerator.h \
    src/Graphics/Deferred/GeomUtils.h \
    src/Graphics/Deferred/DLight.h \
    src/Graphics/Deferred/DeferredLightCP.h \
    src/Graphics/Deferred/DeferredShading.h \
    src/Graphics/Deferred/deferred.h \
    src/VoxelTerrain/voxelphysics.h \
    src/VoxelTerrain/rigidentity.h \
    src/VoxelTerrain/megachunk.h \
    src/VoxelTerrain/Shape.h \
    src/VoxelTerrain/Mesh.h \
    src/VoxelTerrain/MultiShape.h \
    src/VoxelTerrain/sphere.h \
    src/VoxelTerrain/MultiSphere.h \
    src/Utility/ois2sdl.h \
    src/Utility/astar3d.h \
    src/Core/global.h \
    src/gameplay/actor/enemyactor.h \
    src/gameplay/actor/doombot.h \
    libraries/SLB2/SLB.hpp \
    libraries/SLB2/lua/lualib.h \
    libraries/SLB2/lua/luaconf.h \
    libraries/SLB2/lua/lua.h \
    libraries/SLB2/lua/lauxlib.h \
    src/Core/NSRenderer.h \
    src/Core/nsrendererimpl.h \
    src/ArtificialLife/simplexnoise1234.h \
    src/VoxelTerrain/fresnelwater.h \
    src/gameplay/playerinputaction.h \
    resources/Media/DeferredShadingMedia/DeferredShading/post/LightMaterial_ps.cg \
    src/VoxelTerrain/megablob.h \
    src/VoxelTerrain/terrainmultiverse.h \
    src/VoxelTerrain/voxelalgorithm.h \
    src/VoxelTerrain/algoedit.h \
    src/VoxelTerrain/simplexedit.h \
    src/Physics/waterobject.h

SOURCES += \
    src/Core/BaseApplication.cpp \
    src/Core/terminal.cpp \
    src/neuro/virus.cpp \
    src/neuro/square.cpp \
    src/neuro/room.cpp \
    src/neuro/map.cpp \
    src/neuro/hero.cpp \
    src/neuro/astar.cpp \
    src/Core/update.cpp \
    src/Physics/physics.cpp \
    src/Physics/world.cpp \
    src/Graphics/OgreDebugDraw/DebugDrawer.cpp \
    src/Graphics/graphics.cpp \
    src/Physics/physicsobject.cpp \
    src/Physics/physicstypes.cpp \
    src/Core/collisioncamera.cpp \
    src/neuro/port.cpp \
    src/neuro/program.cpp \
    src/neuro/node.cpp \
    src/neuro/projectile.cpp \
    src/neuro/neuroai.cpp \
    src/neuro/phantom.cpp \
    src/Utility/Array.cpp \
    src/ArtificialLife/wolframca.cpp \
    src/ArtificialLife/gameoflife.cpp \
    src/ArtificialLife/Flocking.cpp \
    src/ArtificialLife/abstractca.cpp \
    src/Fractal/Strange.cpp \
    src/Fractal/LSystem.cpp \
    src/Fractal/FractalTypes.cpp \
    src/Fractal/Fractal.cpp \
    libraries/tinyxml/tinyxmlparser.cpp \
    libraries/tinyxml/tinyxmlerror.cpp \
    libraries/tinyxml/tinyxml.cpp \
    libraries/tinyxml/tinystr.cpp \
    src/Physics/scene.cpp \
    src/neuro/backtracer.cpp \
    src/Core/dotscene.cpp \
    src/Physics/eventdispatcher.cpp \
    src/Menu/gamemenu.cpp \
    src/Core/timedevent.cpp \
    src/Core/Entropy.cpp \
    src/Menu/entropylog.cpp \
    src/Menu/logfile.cpp \
    src/Menu/logfileserializer.cpp \
    src/Menu/logfilemanager.cpp \
    src/Menu/inventory.cpp \
    src/Menu/inventoryitem.cpp \
    src/Menu/questmenu.cpp \
    src/Menu/chatscript.cpp \
    src/Menu/chatmenu.cpp \
    src/Menu/entropycursor.cpp \
    src/Script/script.cpp \
    src/Script/EntropyScript.cpp \
    src/Script/wrappers.cpp \
    src/gameplay/quest.cpp \
    src/Menu/console.cpp \
    src/gameplay/actors.cpp \
    src/gameplay/actorupdateactions.cpp \
    src/Menu/questentry.cpp \
    src/Menu/consolefilemanager.cpp \
    src/Menu/consolefile.cpp \
    src/Physics/collisiongroupmanager.cpp \
    src/Script/luafunctor.cpp \
    src/Graphics/DynamicsLines/DynamicsLines.cpp \
    src/Graphics/DynamicsLines/DynamicRenderable.cpp \
    src/Graphics/BulletDebugDraw.cpp \
    src/gameplay/EventAction.cpp \
    src/gameplay/CollisionAction.cpp \
    src/VoxelTerrain/map3.cpp \
    src/VoxelTerrain/part3.cpp \
    src/VoxelTerrain/edit.cpp \
    src/VoxelTerrain/editaxisalignedbox.cpp \
    src/VoxelTerrain/editsphere.cpp \
    src/VoxelTerrain/voxelmap.cpp \
    src/VoxelTerrain/voxelterrain.cpp \
    src/VoxelTerrain/graphicvoxelterrain.cpp \
    src/Graphics/material.cpp \
    src/Graphics/manualentity.cpp \
    src/Graphics/manualobject.cpp \
    src/Graphics/entity.cpp \
    src/Utility/Math.cpp \
    src/VoxelTerrain/terraingenerator.cpp \
    src/Graphics/ShadowListener.cpp \
    src/VoxelTerrain/blobedit.cpp \
    src/VoxelTerrain/chunkedit.cpp \
    src/ArtificialLife/diamondsquared.cpp \
    src/VoxelTerrain/caveedit.cpp \
    src/Graphics/Deferred/gbufferschemehandler.cpp \
    src/Graphics/Deferred/materialgenerator.cpp \
    src/Graphics/Deferred/gbuffermaterialgenerator.cpp \
    src/Graphics/Deferred/AmbientLight.cpp \
    src/Graphics/Deferred/SSAOLogic.cpp \
    src/Graphics/Deferred/LightMaterialGenerator.cpp \
    src/Graphics/Deferred/GeomUtils.cpp \
    src/Graphics/Deferred/DLight.cpp \
    src/Graphics/Deferred/DeferredLightCP.cpp \
    src/Graphics/Deferred/DeferredShading.cpp \
    src/Graphics/Deferred/deferred.cpp \
    src/VoxelTerrain/voxelphysics.cpp \
    src/VoxelTerrain/rigidentity.cpp \
    src/VoxelTerrain/megachunk.cpp \
    src/Utility/ois2sdl.cpp \
    src/Utility/astar3d.cpp \
    src/gameplay/actor/enemyactor.cpp \
    src/gameplay/actor/doombot.cpp \
    libraries/SLB2/SLB.cpp \
    libraries/SLB2/lua.c \
    src/Core/global.cpp \
    src/Core/nsrendererimpl.cpp \
    src/Core/nsrendererimpl.cpp \
    src/ArtificialLife/simplexnoise1234.cpp \
    src/VoxelTerrain/Mesh.cpp \
    src/VoxelTerrain/fresnelwater.cpp \
    src/gameplay/playerinputaction.cpp \
    src/VoxelTerrain/terrainmultiverse.cpp \
    src/VoxelTerrain/voxelalgorithm.cpp \
    src/VoxelTerrain/algoedit.cpp \
    src/VoxelTerrain/simplexedit.cpp \
    src/Physics/waterobject.cpp

OTHER_FILES += \
    entropy_resources.cfg \
    resources/WetwareIcon.png \
    resources/virus_icon.jpg \
    resources/SpawnerIcon.jpg \
    resources/ScannerIcon.png \
    resources/NetworkIcon.jpg \
    resources/Nanowires.jpg \
    resources/IOIcon.png \
    resources/DataIcon.jpg \
    resources/ControlIcon.png \
    QMakeVars.pri \
    resources/NeuroResources/WetwareIcon.png \
    resources/NeuroResources/WaterWindow.jpg \
    resources/NeuroResources/virus_icon.pnm \
    resources/NeuroResources/virus_icon.png \
    resources/NeuroResources/Vamp.png \
    resources/NeuroResources/TrojanIcon.png \
    resources/NeuroResources/SpawnerIcon.jpg \
    resources/NeuroResources/ScannerIcon.png \
    resources/NeuroResources/NetworkIcon.jpg \
    resources/NeuroResources/Nanowires.jpg \
    resources/NeuroResources/LockIcon.png \
    resources/NeuroResources/IOIcon.png \
    resources/NeuroResources/InfectionIcon.png \
    resources/NeuroResources/EWetwareIcon.png \
    resources/NeuroResources/Evirus_icon.png \
    resources/NeuroResources/ESpawnerIcon.jpg \
    resources/NeuroResources/EScannerIcon.png \
    resources/NeuroResources/EDataIcon.png \
    resources/NeuroResources/EControlIcon.png \
    resources/NeuroResources/DataIcon.png \
    resources/NeuroResources/ControlIcon.png \
    resources/NeuroResources/BombIcon.png \
    resources/NeuroResources/AlarmIcon.png \
    resources/NeuroResources/ProjectileSprites.png \
    resources/NeuroResources/HoneyPotIcon.png \
    resources/NeuroResources/CrashIcon.png \
    resources/NeuroResources/BlackIceIcon.png \
    Game ideas.rtf \
    resources/NeuroResources/ScanIcon.png \
    resources/NeuroResources/PhantomIcon.png \
    resources/NeuroResources/InvisibleHero.png \
    resources/white_cursor.png \
    resources/glossy_button.png \
    resources/LogFiles/textideas.txt \
    resources/LogFiles/LoremIpsum.txt \
    resources/hi_arrow_button.png \
    resources/arrow_button.png \
    resources/scenes/TestLevel/WaterWindow.jpg \
    resources/scenes/TestLevel/untitled.scene \
    resources/scenes/TestLevel/untitled.blend2 \
    resources/scenes/TestLevel/untitled.blend1 \
    resources/scenes/TestLevel/untitled.blend \
    resources/scenes/TestLevel/TestLevel.scene \
    resources/scenes/TestLevel/TestLevel.blend2 \
    resources/scenes/TestLevel/TestLevel.blend1 \
    resources/scenes/TestLevel/TestLevel.blend \
    resources/scenes/TestLevel/Material.material \
    resources/scenes/TestLevel/Material.002.material \
    resources/scenes/TestLevel/Material.001.material \
    resources/scenes/TestLevel/Log.mesh.xml \
    resources/scenes/TestLevel/Log.mesh \
    resources/scenes/TestLevel/Icosphere.mesh.xml \
    resources/scenes/TestLevel/Icosphere.mesh \
    resources/scenes/TestLevel/DefaultTexture.xcf \
    resources/scenes/TestLevel/DefaultTexture.png \
    resources/scenes/TestLevel/Cube.mesh.xml \
    resources/scenes/TestLevel/Cube.mesh \
    resources/scenes/TestLevel/Cube.001.mesh.xml \
    resources/scenes/TestLevel/Cube.001.mesh \
    resources/InventoryIcons/VirgilComm.png \
    resources/InventoryIcons/StrangeRock.png \
    resources/InventoryIcons/SonicPathfinder.png \
    resources/InventoryIcons/Flashlight.png \
    resources/InventoryIcons/InventoryBlackout.png \
    resources/InventoryIcons/InventoryBackground.png \
    resources/InventoryIcons/questIcon.png \
    libraries/ChatScript/libChatLib.a \
    resources/ChatScript/version.txt \
    resources/ChatScript/serverlog1024.txt \
    resources/ChatScript/err.txt \
    resources/ChatScript/DICT/z.txt \
    resources/ChatScript/DICT/y.txt \
    resources/ChatScript/DICT/x.txt \
    resources/ChatScript/DICT/w.txt \
    resources/ChatScript/DICT/v.txt \
    resources/ChatScript/DICT/u.txt \
    resources/ChatScript/DICT/t.txt \
    resources/ChatScript/DICT/s.txt \
    resources/ChatScript/DICT/r.txt \
    resources/ChatScript/DICT/q.txt \
    resources/ChatScript/DICT/p.txt \
    resources/ChatScript/DICT/other.txt \
    resources/ChatScript/DICT/o.txt \
    resources/ChatScript/DICT/n.txt \
    resources/ChatScript/DICT/m.txt \
    resources/ChatScript/DICT/label.txt \
    resources/ChatScript/DICT/l.txt \
    resources/ChatScript/DICT/k.txt \
    resources/ChatScript/DICT/j.txt \
    resources/ChatScript/DICT/i.txt \
    resources/ChatScript/DICT/h.txt \
    resources/ChatScript/DICT/g.txt \
    resources/ChatScript/DICT/facts.txt \
    resources/ChatScript/DICT/f.txt \
    resources/ChatScript/DICT/e.txt \
    resources/ChatScript/DICT/dict.bin \
    resources/ChatScript/DICT/d.txt \
    resources/ChatScript/DICT/c.txt \
    resources/ChatScript/DICT/b.txt \
    resources/ChatScript/DICT/a.txt \
    resources/ChatScript/DICT/9.txt \
    resources/ChatScript/DICT/8.txt \
    resources/ChatScript/DICT/7.txt \
    resources/ChatScript/DICT/6.txt \
    resources/ChatScript/DICT/5.txt \
    resources/ChatScript/DICT/4.txt \
    resources/ChatScript/DICT/3.txt \
    resources/ChatScript/DICT/2.txt \
    resources/ChatScript/DICT/1.txt \
    resources/ChatScript/DICT/0.txt \
    resources/ChatScript/LIVEDATA/systemfacts.txt \
    resources/ChatScript/LIVEDATA/substitutes.txt \
    resources/ChatScript/LIVEDATA/spellfix.txt \
    resources/ChatScript/LIVEDATA/queries.txt \
    resources/ChatScript/LIVEDATA/posrules.txt \
    resources/ChatScript/LIVEDATA/lowercaseTitles.txt \
    resources/ChatScript/LIVEDATA/canonical.txt \
    resources/ChatScript/LIVEDATA/british.txt \
    resources/ChatScript/LIVEDATA/abbreviations.txt \
    resources/ChatScript/LIVEDATA/COMPILE/readme.txt \
    resources/ChatScript/LIVEDATA/COMPILE/generalizer.txt \
    resources/ChatScript/LIVEDATA/COMPILE/allownonwords.txt \
    resources/ChatScript/LOGS/serverlog1024.txt \
    resources/ChatScript/LOGS/bugs.txt \
    resources/ChatScript/RAWDATA/ONTOLOGY/verbhierarchy.top \
    resources/ChatScript/RAWDATA/ONTOLOGY/sexednouns.tbl \
    resources/ChatScript/RAWDATA/ONTOLOGY/pureverbhierarchy.top \
    resources/ChatScript/RAWDATA/ONTOLOGY/prepositionhierarchy.top \
    resources/ChatScript/RAWDATA/ONTOLOGY/opposite.tbl \
    resources/ChatScript/RAWDATA/ONTOLOGY/nounhierarchy.top \
    resources/ChatScript/RAWDATA/ONTOLOGY/concepts.top \
    resources/ChatScript/RAWDATA/ONTOLOGY/affect.top \
    resources/ChatScript/RAWDATA/ONTOLOGY/adverbhierarchy.top \
    resources/ChatScript/RAWDATA/ONTOLOGY/adjectivehierarchy.top \
    resources/ChatScript/RAWDATA/skeleton.top \
    resources/ChatScript/RAWDATA/simpletopic.top \
    resources/ChatScript/RAWDATA/simplecontrol.top \
    resources/ChatScript/RAWDATA/WORLDDATA/videogames.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/tools.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/things.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/speed.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/peoplegroups.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/musicbasic.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/music.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/movies_tv.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/malepeople.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/literature.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/lifespan.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/geography.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/femalepeople.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/container.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/color.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/cars.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/basicgeography.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/basicbooks.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/basic.tbl \
    resources/ChatScript/RAWDATA/WORLDDATA/animals.tbl \
    resources/ChatScript/REGRESS/tokens.txt \
    resources/ChatScript/REGRESS/questions.txt \
    resources/ChatScript/REGRESS/postest.txt \
    resources/ChatScript/REGRESS/particletest.txt \
    resources/ChatScript/REGRESS/LoebnerQuestions.txt \
    resources/ChatScript/REGRESS/junk.txt \
    resources/ChatScript/REGRESS/failedpos.txt \
    resources/ChatScript/REGRESS/bigregress.txt \
    resources/ChatScript/TMP/topics.tmp \
    resources/ChatScript/TMP/prebuild1 \
    resources/ChatScript/TMP/prebuild0 \
    resources/ChatScript/TMP/facts.tmp \
    resources/ChatScript/TOPIC/script1.txt \
    resources/ChatScript/TOPIC/script0.txt \
    resources/ChatScript/TOPIC/patternWords1.txt \
    resources/ChatScript/TOPIC/patternWords0.txt \
    resources/ChatScript/TOPIC/macros1.txt \
    resources/ChatScript/TOPIC/keywords1.txt \
    resources/ChatScript/TOPIC/keywords0.txt \
    resources/ChatScript/TOPIC/facts1.txt \
    resources/ChatScript/TOPIC/facts0.txt \
    resources/ChatScript/TOPIC/extraKeys1.txt \
    resources/ChatScript/TOPIC/extraKeys0.txt \
    resources/ChatScript/TOPIC/dict1.txt \
    resources/ChatScript/TOPIC/dict0.txt \
    resources/ChatScript/VERIFY/violence.txt \
    resources/ChatScript/VERIFY/research.txt \
    resources/ChatScript/VERIFY/preferences.txt \
    resources/ChatScript/VERIFY/lab.txt \
    resources/ChatScript/VERIFY/introductions.txt \
    resources/ChatScript/VERIFY/info.txt \
    resources/ChatScript/VERIFY/control.txt \
    resources/ChatScript/VERIFY/childhood.txt \
    resources/ChatScript/VERIFY/alternate_control.txt \
    resources/ChatScript/VERIFY/access.txt \
    resources/ChatScript/VERIFY/aboutsecurebot.txt \
    resources/ChatScript/USERS/null.txt \
    resources/ChatScript/err.txt \
    resources/ChatScript/err.txt \
    DesignDocs/LivingAreaFloor1.svg \
    DesignDocs/LivingArea.jpg \
    DesignDocs/Act I, Chapter II.rtf \
    DesignDocs/Game ideas.rtf \
    DesignDocs/Backstory.rtf \
    DesignDocs/Opening Sequence.rtf \
    DesignDocs/Opening Sequence.rtf \
    DesignDocs/Michael McGill.rtfd \
    DesignDocs/Game ideas.rtf \
    DesignDocs/Enemies.txt \
    DesignDocs/Dixon Rutledge.rtfd \
    DesignDocs/Act I, Chapter II.rtf \
    DesignDocs/Act I, Chapter II.rtf \
    resources/InteractionIcons/LookIcon.png \
    resources/InteractionIcons/HackIcon.png \
    resources/InteractionIcons/GrabIcon.png \
    resources/InteractionIcons/ActivateIcon.png \
    DesignDocs/MapEditorDesign.txt \
    src/Graphics/Shadows/ESMBoxFilters.material \
    dylibScript.sh \
    Info.plist \
    ../media/materials/scripts/Examples-Water.material \
    resources/Media/DeferredShadingMedia/deferred.compositor \
    resources/LuaScripts/DefaultLibrary/StartUp.lua \
    resources/Media/materials/programs/Example_Fresnel.cg \
    resources/Media/materials/scripts/Examples-Advanced.material \
    resources/Media/DeferredShadingMedia/DeferredShading/post/LightMaterial_vs.cg \
    resources/Shaders/Water/EntropyUnderwater.material \
    resources/Shaders/Water/EntropyUnderwater.cg \
    resources/Shaders/Water/UnderwaterQuad.material \
    resources/Shaders/Water/UnderwaterQuad.compositor \
    resources/Media/DeferredShadingMedia/deferred_post.program \
    resources/Media/DeferredShadingMedia/deferred_post.material \
    resources/Media/DeferredShadingMedia/ssao.compositor \
    resources/Media/DeferredShadingMedia/ssao.material \
    DesignDocs/Game ideas.rtf \
    resources/Shaders/Water/UnderwaterQuadVP.cg \
    resources/Shaders/Water/UnderwaterQuadFP.cg \
    resources/SimplexMaterial.material \
    ../QMakeVars.pri \
    resources/Shaders/ASCII/asciiShader.compositor
