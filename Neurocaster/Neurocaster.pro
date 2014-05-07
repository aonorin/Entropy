#TEMPLATE = app
TARGET = Neurocaster

CONFIG -= qt

QT -= core
QT -= gui

#CONFIG -= x86_64
#CONFIG += x86

include( ../../QMakeVars.pri )

#DEFINES += BOOST_ALL_NO_LIB \
#    USE_RTSHADER_SYSTEM

# DEFINES += USE_RTSHADER_SYSTEM

#CHANGE THIS BACK FOR BEST PERFORMANCE
#QMAKE_CXXFLAGS_RELEASE += -IOgre -O3 -g
#QMAKE_CXXFLAGS_DEBUG += -IOgre -O3 -g

QMAKE_CXXFLAGS_RELEASE += -IOgre -O0 -g
QMAKE_CXXFLAGS_DEBUG += -IOgre -O0 -g

CXXFLAGS_RELEASE += -visibility=default
CXXFLAGS_DEBUG += -visibility=default

QMAKE_CFLAGS += -msse
QMAKE_CXXFLAGS += -msse
CXXFLAGS += -F$${OGREHOME}/lib/release \
    -Wl,-search_paths_first \
    -Wl,-headerpad_max_install_names

# Tiny XML Flag
QMAKE_CXXFLAGS += -DTIXML_USE_STL
QMAKE_CXXFLAGS += -v
CXXFLAGS += -DTIXML_USE_STL
QMAKE_CFLAGS += -DTIXML_USE_STL
CFLAGS += -DTIXML_USE_STL

CXXFLAGS += `sdl-config --cflags`

#INCLUDEPATH += $${ENTROPYINCLUDE}
#INCLUDEPATH += Hypnos \
#    Hypnos/src



macx* {
    QMAKE_CXXFLAGS += -F/Library/Frameworks
    QMAKE_LFLAGS += -F/Library/Frameworks

    QMAKE_CFLAGS += -D__APPLE__
    CFLAGS += -D__APPLE__

    INCLUDEPATH += \
        $${OGREHOME}/include \
        $${OGREHOME}/include/Ogre \
        $${OGREHOME}/include/OIS \
        $${OGREHOME}/include/Ogre/OSX \
        $${OGREHOME}/include/Ogre/Threading \
        -F/System/Library/Frameworks \
        $${BOOSTINCLUDE} \
#        $${BULLETINCLUDE} \
        $${SLBINCLUDE} \
        $${SLBINCLUDE}/../src \
        $${OISINCLUDE} \
#        $${SDLINCLUDE} \
        /System/Library/Frameworks/AppKit.framework/Headers \

    DEPENDPATH += \
        $${OGREHOME}/include \
        $${OGREHOME}/include/Ogre \
        $${OGREHOME}/include/Ogre/OSX \
        $${OGREHOME}/include/Ogre/Threading \
#        $${BULLETINCLUDE}

    RESOURCE_FILES.files += $${OGREHOME}/bin/plugins.cfg \
        $${OGREHOME}/bin/quakemap.cfg \
        $${OGREHOME}/bin/samples.cfg \
        neurocaster_resources.cfg \

    RESOURCE_FILES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += RESOURCE_FILES
    OBJECTIVE_HEADERS += src/core/NSRenderer.h
    OBJECTIVE_SOURCES += src/core/NSRenderer.mm

    FRAMEWORKS.files += $${OGREHOME}/lib/release/Ogre.framework
    FRAMEWORKS.path = Contents/Frameworks
    QMAKE_BUNDLE_DATA += FRAMEWORKS

    COMPONENTS.files += \
        $${OGREHOME}/lib/libOgrePaging.dylib \
        $${OGREHOME}/lib/libOgreProperty.dylib \
        $${OGREHOME}/lib/libOgreRTShaderSystem.dylib \
        $${OGREHOME}/lib/libOgreTerrain.dylib

    COMPONENTS.path = Contents/Components
    QMAKE_BUNDLE_DATA += COMPONENTS

    PLUGINS.files += $${OGREHOME}/lib/Plugin_BSPSceneManager.dylib \
        $${OGREHOME}/lib/Plugin_CgProgramManager.dylib \
        $${OGREHOME}/lib/Plugin_OctreeSceneManager.dylib \
        $${OGREHOME}/lib/Plugin_OctreeZone.dylib \
        $${OGREHOME}/lib/Plugin_ParticleFX.dylib \
        $${OGREHOME}/lib/Plugin_PCZSceneManager.dylib \
        $${OGREHOME}/lib/RenderSystem_GL.dylib \
        $${SDLLIBS}/libSDL2.dylib

    PLUGINS.path = Contents/Plugins
    QMAKE_BUNDLE_DATA += PLUGINS

    LIBS += \
        -framework Carbon \
        -framework Cocoa \
        -framework Foundation \
        -framework IOKit \
        -framework AppKit \
        -F/System/Library/Frameworks \
        -framework Ogre \
        $${OGREHOME}/lib/libOgreRTShaderSystem.dylib \
        $${OGREHOME}/lib/libOgreTerrain.dylib \
        $${OGREHOME}/lib/libOgrePaging.dylib \
        $${OGREHOME}/lib/libOgreProperty.dylib \
        $${OGREHOME}/lib/Plugin_BSPSceneManager.dylib \
        $${OGREHOME}/lib/RenderSystem_GL.dylib \
        $${OGREHOME}/lib/release/libOIS.a \
#        -framework $${OUT_PWD}/Hypnos.app/Contents/Frameworks/Ogre.framework \
        #Contents/Plugins/libSDL2.dylib \
#        $${SDLLIBS}/libSDL2.dylib \
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
        $${BULLETLIBS}/libMiniCL.a
#        $${ENTROPYLIB}/libEntropy.a

#        PRE_TARGETDEPS += $${ENTROPYLIB}/libEntropy.a
SOURCES += \
    src/core/main.mm
QMAKE_INFO_PLIST = /Users/curtismckinney/build/Neurocaster/Info.plist

#stuff for copying shaders
system(rm -r $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Media/DeferredShadingMedia)
system(rm -r $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Media/materials/scripts)
system(rm -r $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Media/materials/programs)
system(rm -r $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/LuaScripts)
system(rm -r $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Shaders)
system(rm $${OUT_PWD}/Neurocaster.app/Contents/Resources/neurocaster_resources.cfg)


system(cp -R ../resources/Media/DeferredShadingMedia $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Media/DeferredShadingMedia)
system(cp -R ../resources/Media/materials/scripts $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Media/materials/scripts)
system(cp -R ../resources/Media/materials/programs $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Media/materials/programs)
system(cp -R ../resources/LuaScripts $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/LuaScripts)
system(cp -R ../resources/Shaders $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Shaders)
system(cp neurocaster_resources.cfg $${OUT_PWD}/Neurocaster.app/Contents/Resources/neurocaster_resources.cfg)


}

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
        $${OGREINCLUDE}/include/OIS \
        $${OGREINCLUDE} \
        $${BOOSTINCLUDE} \
#        $${BULLETINCLUDE} \
        $${SLBINCLUDE} \
        $${SLBINCLUDE}/../src \
        $${OISINCLUDE} \
        $${SDLINCLUDE} \

    DEPENDPATH += $${BULLETLIBS}

    DEPENDPATH += $${OGREINCLUDE}/include/OGRE \
        $${OGREINCLUDE}/include/OGRE/RTShaderSystem \
        $${OGREINCLUDE}/include \
        $${OGREINCLUDE}/common/include \
        $${OGREINCLUDE}/include/OGRE/OSX \
        $${OGREINCLUDE}/include/OGRE/RTShaderSystem \
        $${OGREINCLUDE}/include/Terrain \
        $${OGREINCLUDE}/include/Paging \
        #$${OGREHOME}/include/OIS

#    LIBS += $${ENTROPYLIB}/libEntropy.a \
        #$${SDLLIBS}/libSDL2.a

    LIBS += $${OGRELIB}/libOgreMain.so \
        $${OGRELIB}/libOgrePaging.so \
        $${OGRELIB}/libOgreProperty.so \
        $${OGRELIB}/libOgreRTShaderSystem.so \
        $${OGRELIB}/libOgreTerrain.so \
        $${OGREPLUGINS}/Plugin_BSPSceneManager.so \
        $${OGREPLUGINS}/Plugin_CgProgramManager.so \
        $${OGREPLUGINS}/Plugin_OctreeSceneManager.so \
        $${OGREPLUGINS}/Plugin_OctreeZone.so \
        $${OGREPLUGINS}/Plugin_ParticleFX.so \
        $${OGREPLUGINS}/Plugin_PCZSceneManager.so \
        $${OGREPLUGINS}/RenderSystem_GL.so \
        /usr/lib/x86_64-linux-gnu/libOIS.a

#    PRE_TARGETDEPS += $${ENTROPYLIB}/libEntropy.a

    LIBS += \
        $${BOOSTLIBS}/libboost_date_time.a \
        $${BOOSTLIBS}/libboost_thread.a \
        $${BOOSTLIBS}/libboost_signals.a \
        $${BOOSTLIBS}/libboost_chrono.a \
        $${BOOSTLIBS}/libboost_system.a \
        $${BULLETLIBS}/libBulletSoftBody.a \
        $${BULLETLIBS}/libBulletDynamics.a \
        $${BULLETLIBS}/libBulletCollision.a \
        #$${BULLETLIBS}/libBulletMultiThreaded.a \
        $${BULLETLIBS}/libLinearMath.a \
        #$${BULLETLIBS}/libMiniCL.a \
        #$${SLBLIBS}/libSLB.a \
        /usr/lib/x86_64-linux-gnu/libX11.so \
        -lpthread

    #INCLUDEPATH += $${OGREHOME}/include/OGRE \
    #    $${OGREHOME}/include/OGRE/RTShaderSystem \
    #    $${OGREHOME}/include/OIS \
    #    $${OGREHOME}/boost_1_46_1

    #LIBS += $${OGREHOME}/lib/libOgreMain.so \
    #    $${OGREHOME}/lib/libOIS.so

    DESTDIR = $$OUT_PWD

    plugins.path = $$DESTDIR/Contents/LinuxOS
    other_files.path = $$DESTDIR

    resources.files = ../resources
    resources.path = $$DESTDIR/Contents/Resources

    emptyfolder.files = LinuxOS.txt
    emptyfolder.path = $$DESTDIR/Contents/LinuxOS

    ## Tell qmake to add the moving of them to the 'install' target
    INSTALLS += plugins
    INSTALLS += other_files
    INSTALLS += resources
    INSTALLS += emptyfolder

#MAKE FOLDER STRUCTURE
system(mkdir $$DESTDIR/NeurocasterContent)
system(mkdir $$DESTDIR/NeurocasterContent/Contents)
system(mkdir $$DESTDIR/NeurocasterContent/Contents/LinuxOS)
system(mkdir $$DESTDIR/NeurocasterContent/Contents/Components)
system(mkdir $$DESTDIR/NeurocasterContent/Contents/Resources)
system(mkdir $$DESTDIR/NeurocasterContent/Contents/Frameworks)
system(mkdir $$DESTDIR/NeurocasterContent/Contents/Plugins)

#OUT_PWD = $$DESTDIR/Contents/LinuxOS

#COPY PLUGINS
system(cp $${OGREPLUGINS}/Plugin_BSPSceneManager.so $$DESTDIR/NeurocasterContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_CgProgramManager.so $$DESTDIR/NeurocasterContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_OctreeSceneManager.so $$DESTDIR/NeurocasterContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_OctreeZone.so $$DESTDIR/NeurocasterContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_ParticleFX.so $$DESTDIR/NeurocasterContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_PCZSceneManager.so $$DESTDIR/NeurocasterContent/Contents/Components)
system(cp $${OGREPLUGINS}/RenderSystem_GL.so $$DESTDIR/NeurocasterContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_PCZSceneManager.so $$DESTDIR/NeurocasterContent/Contents/Components)

#COPY LIBS
system(cp $${OGRELIB}/libOgreMain.so $$DESTDIR/NeurocasterContent/Contents/Frameworks)
system(cp $${OGRELIB}/libOgrePaging.so $$DESTDIR/NeurocasterContent/Contents/Plugins)
system(cp $${OGRELIB}/libOgreProperty.so $$DESTDIR/NeurocasterContent/Contents/Plugins)
system(cp $${OGRELIB}/libOgreRTShaderSystem.so $$DESTDIR/NeurocasterContent/Contents/Plugins)
system(cp $${OGRELIB}/libOgreTerrain.so $$DESTDIR/NeurocasterContent/Contents/Plugins)
system(cp $${OGRECONFIGS}/resources.cfg $$DESTDIR/NeurocasterContent/Contents/Resources)
system(cp $${OGRECONFIGS}/quakemap.cfg $$DESTDIR/NeurocasterContent/Contents/Resources)
system(cp $${OGRECONFIGS}/samples.cfg $$DESTDIR/NeurocasterContent/Contents/Resources)
system(cp $${OGRECONFIGS}/tests.cfg $$DESTDIR/NeurocasterContent/Contents/Resources)
system(cp $${OGRECONFIGS}/plugins.cfg $$DESTDIR/NeurocasterContent/Contents/Resources)
system(cp neurocaster_resources.cfg $$DESTDIR/NeurocasterContent/Contents/Resources)
system(cp -R $${ENTROPYRESOURCES} $$DESTDIR/NeurocasterContent/Contents/Resources)

#stuff for copying shaders
system(rm -r $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Media/DeferredShadingMedia)
system(rm -r $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Media/materials/scripts)
#system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Media/materials/programs)
system(rm -r $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/LuaScripts)
system(rm -r $${OUT_PWD}/Neurocaster.app/Contents/Resources/resources/Shaders)

system(cp -R ../resources $${OUT_PWD}/NeurocasterContent/Contents/Resources)
system(cp -R ../resources/Media/DeferredShadingMedia $${OUT_PWD}/NeurocasterContent/Contents/Resources/resources/Media/DeferredShadingMedia)
system(cp -R ../resources/Media/materials/scripts $${OUT_PWD}/NeurocasterContent/Contents/Resources/resources/Media/materials/scripts)
#system(cp -R resources/Media/materials/programs $${OUT_PWD}/NeurocasterContent/Contents/Resources/resources/Media/materials/programs)
system(cp -R ../resources/LuaScripts $${OUT_PWD}/NeurocasterContent/Contents/Resources/resources/LuaScripts)
system(cp -R ../resources/Shaders $${OUT_PWD}/NeurocasterContent/Contents/Resources/resources/Shaders)
}

win32-msvc2010 {
    TEMPLATE = app
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

    LIBS += -L$${BOOSTLIBS}
    #Release:LIBS += -L$${OGREHOME}/lib/release
    #Debug:LIBS += -L$${OGREHOME}/lib/debug
    INCLUDEPATH += $${OGREHOME}/include
    INCLUDEPATH += $${OGREHOME}/include/OIS
    INCLUDEPATH += $${OGREHOME}/include/OGRE
    INCLUDEPATH += \
        $${BOOSTINCLUDES} \
        $${BULLETINCLUDES} \

#    INCLUDEPATH += $${ENTROPY_HOME}
#    DEPENDPATH += $${ENTROPY_HOME}

    #INCLUDEPATH += $${ENTROPY_HOME}/libraries/SLB-2.00
    #DEPENDPATH += $${ENTROPY_HOME}/libraries/SLB-2.00

    DEPENDPATH += $${OGREHOME}/include
    DEPENDPATH += $${OGREHOME}/include/OIS
    DEPENDPATH += $${OGREHOME}/include/OGRE
    DEPENDPATH += \
        $${BOOSTINCLUDES} \
        $${BULLETINCLUDES} \

#    LIBS += $${ENTROPYLIB}/Entropy.lib
    #LIBS += C:/Users/LiquidSquid/code/Entropy-build/Entropy.lib

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
        $${OIS_LIB}/OIS.lib \
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
        $${OIS_LIB}/OIS.lib \
    }

#    PRE_TARGETDEPS +=
#        $${ENTROPYLIB}/Entropy.lib \

    #CONFIG(debug, debug|release) {
    #    TARGET = $$join(TARGET,,,d)
    #    LIBS *= -lOgreMain_d -lOIS_d
    #}

    #CONFIG(release, debug|release):LIBS *= -lOgreMain -lOIS

    #MY_LIB_FILES += $${OGRE_DLLS}/OgreMain.dll
    #MY_LIB_FILES += $${OGRE_DLLS}/LIBGCC_S_DW2-1.DLL
    #MY_LIB_FILES += $${OGRE_DLLS}/QTcore4.DLL
    #MY_LIB_FILES += $${OGRE_DLLS}/QTGUI4.DLL

    ## Define what files are 'extra_libs' and where to put them
    plugins.files += \
        $${OGRE_DLLS}/Plugin_BSPSceneManager.dll \
        $${OGRE_DLLS}/Plugin_CgProgramManager.dll \
        $${OGRE_DLLS}/Plugin_OctreeSceneManager.dll \
        $${OGRE_DLLS}/Plugin_OctreeZone.dll \
        $${OGRE_DLLS}/Plugin_ParticleFX.dll \
        $${OGRE_DLLS}/Plugin_PCZSceneManger.dll \
        $${OGRE_DLLS}/RenderSystem_Direct3D9.dll \
        $${OGRE_DLLS}/RenderSystem_GL.dll \
        $${OGRE_DLLS}/Plugin_PCZSceneManager.dll

    other_files.files += \
        $${OGRE_DLLS}/OgreMain.dll \
        $${OGRE_DLLS}/OgrePaging.dll \
        $${OGRE_DLLS}/OgreProperty.dll \
        $${OGRE_DLLS}/OgreRTShaderSystem.dll \
        $${OGRE_DLLS}/OgreTerrain.dll \
        $${OGRE_DLLS}/resources.cfg \
        $${OGRE_DLLS}/quakemap.cfg \
        $${OGRE_DLLS}/samples.cfg \
        $${OGRE_DLLS}/test.cfg \
        $${OGRE_DLLS}/plugins.cfg \
        neurocaster_resources.cfg \
        $${FREEIMAGE_DLL_LOCATION}/FreeImage.dll \
        $${OIS_DLL_LOCATION}/OIS.dll

    #CONFIG(debug, debug|release): OUTDIR = debug
    #else: OUTDIR = release

    DESTDIR = $$OUT_PWD

    plugins.path = $$DESTDIR/Contents/WinOS
    other_files.path = $$DESTDIR

    resources.files = resources
    resources.path = $$DESTDIR/Contents/Resources
    QMAKE_BUNDLE_DATA += MEDIA

    emptyfolder.files = WinOS.txt
    emptyfolder.path = $$DESTDIR/Contents/WinOS

    ## Tell qmake to add the moving of them to the 'install' target
    INSTALLS += plugins
    INSTALLS += other_files
    INSTALLS += resources
    INSTALLS += emptyfolder
}

RESOURCE_FILES.files += $${OGREHOME}/bin/plugins.cfg \
        $${OGREHOME}/bin/quakemap.cfg \
        $${OGREHOME}/bin/samples.cfg \
       neurocaster_resources.cfg \

RESOURCE_FILES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += RESOURCE_FILES

MATERIALS.files += resources/OgreDebugDraw/Debug.material
MATERIALS.path = ../Media/materials/scripts
QMAKE_BUNDLE_DATA += MATERIALS

ENTROPY_RESOURCES.files = resources
ENTROPY_RESOURCES.path = Contents/Resources
QMAKE_BUNDLE_DATA += ENTROPY_RESOURCES

TEST_LEVEL.files = resources/scenes/TestLevel
TEST_LEVEL.path = Contents/Resources/resources/scenes
QMAKE_BUNDLE_DATA += TEST_LEVEL

INCLUDEPATH += src/libraries
INCLUDEPATH += ../libraries
INCLUDEPATH += src
DEPENDPATH += src

HEADERS += \
    src/core/neurocaster.h \
    src/core/baseapplication.h \
    src/Menu/gamemenu.h \
    src/core/update.h \
    src/gameplay/astar.h \
    src/core/Global.h \
    src/gameplay/neuron.h \
    src/gameplay/neuroai.h \
    src/gameplay/abilities/ability.h \
    src/gameplay/abilities/hackability.h \
    src/gameplay/square.h \
    src/gameplay/entity.h \
    src/gameplay/structures/hackable.h \
    src/gameplay/structures/port.h \
    src/gameplay/structures/node.h \
    src/gameplay/structures/subsystem.h \
    src/gameplay/tile.h \
    src/gameplay/abilities/enemyhack.h \
    src/gameplay/abilities/playerprogram.h \
    src/gameplay/map.h \
    src/gameplay/updateable.h \
    src/gameplay/maps/roguemapfactory.h \
    src/core/scene.h \
    src/core/scenemanager.h \
    src/gameplay/grid.h \
    src/gameplay/polyneuromanualobject.h \
    src/gameplay/structures/node_icons/nodeicon.h \
    src/gameplay/structures/port_icons/porticon.h \
    src/utility/util.h \
    src/gameplay/structures/hallway.h \
    src/gameplay/actor.h \
    src/Menu/menu.h \
    ../libraries/kdtree/kdtree.hsrc/gameplay/player.h \
    src/gameplay/enemies/sentry.h \
    src/Menu/playermenu.h \
    src/gameplay/player.h \
    src/gameplay/enemies/sentry.h \
    src/gameplay/tilehilight.h \
    src/Menu/playeractiongui.h \
    src/gameplay/abilities/playerportprogram.h \
    src/gameplay/abilities/playernodeprogram.h \
    src/gameplay/abilities/playerport/crack.h \
    src/graphics/abilityicongenerator.h

SOURCES += \
    src/core/neurocaster.cpp \
    src/core/baseapplication.cpp \
    src/Menu/gamemenu.cpp \
    src/core/update.cpp \
    src/gameplay/astar.cpp \
    src/gameplay/neuron.cpp \
    src/gameplay/neuroai.cpp \
    src/gameplay/abilities/ability.cpp \
    src/gameplay/abilities/hackability.cpp \
    src/gameplay/square.cpp \
    src/core/Global.cpp \
    src/gameplay/entity.cpp \
    src/gameplay/structures/hackable.cpp \
    src/gameplay/structures/port.cpp \
    src/gameplay/structures/node.cpp \
    src/gameplay/structures/subsystem.cpp \
    src/gameplay/tile.cpp \
    src/gameplay/abilities/enemyhack.cpp \
    src/gameplay/abilities/playerprogram.cpp \
    src/gameplay/map.cpp \
    src/gameplay/updateable.cpp \
    src/gameplay/maps/roguemapfactory.cpp \
    src/core/scene.cpp \
    src/core/scenemanager.cpp \
    src/gameplay/grid.cpp \
    src/gameplay/polyneuromanualobject.cpp \
    src/gameplay/structures/node_icons/nodeicon.cpp \
    src/gameplay/structures/port_icons/porticon.cpp \
    src/utility/util.cpp \
    src/gameplay/structures/hallway.cpp \
    src/gameplay/actor.cpp \
    src/Menu/menu.cpp \
    ../libraries/kdtree/kdtree.c \
    src/gameplay/player.cpp \
    src/gameplay/enemies/sentry.cpp \
    src/Menu/playermenu.cpp \
    src/gameplay/tilehilight.cpp \
    src/Menu/playeractiongui.cpp \
    src/gameplay/abilities/playerportprogram.cpp \
    src/gameplay/abilities/playernodeprogram.cpp \
    src/gameplay/abilities/playerport/crack.cpp \
    src/graphics/abilityicongenerator.cpp

linux-g++ {
    # UNCOMMENT FOR LINUX BUNDLE BUILDING
    #QMAKE_POST_LINK = ../Entropy/LinuxBundle.sh
}

OTHER_FILES +=
