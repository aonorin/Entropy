#TEMPLATE = app
TARGET = Hypnos

CONFIG -= qt

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

INCLUDEPATH += $${ENTROPYINCLUDE}
INCLUDEPATH += Hypnos \
    Hypnos/src



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

    RESOURCE_FILES.files += $${OGREHOME}/bin/plugins.cfg \
        $${OGREHOME}/bin/quakemap.cfg \
        $${OGREHOME}/bin/samples.cfg \
        entropy_resources.cfg \

    RESOURCE_FILES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += RESOURCE_FILES
    OBJECTIVE_HEADERS += src/Core/NSRenderer.h
    OBJECTIVE_SOURCES += src/Core/NSRenderer.mm

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
        $${BULLETLIBS}/libMiniCL.a \
        $${ENTROPYLIB}/libEntropy.a

        PRE_TARGETDEPS += $${ENTROPYLIB}/libEntropy.a
SOURCES += \
    Hypnos/src/Core/main.mm
QMAKE_INFO_PLIST = /Users/curtismckinney/build/Entropy/Info.plist

#stuff for copying shaders
system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Media/DeferredShadingMedia)
system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Media/materials/scripts)
system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Media/materials/programs)
system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/LuaScripts)
system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Shaders)

system(cp -R resources/Media/DeferredShadingMedia $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Media/DeferredShadingMedia)
system(cp -R resources/Media/materials/scripts $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Media/materials/scripts)
system(cp -R resources/Media/materials/programs $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Media/materials/programs)
system(cp -R resources/LuaScripts $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/LuaScripts)
system(cp -R resources/Shaders $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Shaders)

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
        $${BULLETINCLUDE} \
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

    LIBS += $${ENTROPYLIB}/libEntropy.a \
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

    PRE_TARGETDEPS += $${ENTROPYLIB}/libEntropy.a

    LIBS += $${BOOSTLIBS}/libboost_date_time.a \
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

    resources.files = resources
    resources.path = $$DESTDIR/Contents/Resources

    emptyfolder.files = LinuxOS.txt
    emptyfolder.path = $$DESTDIR/Contents/LinuxOS

    ## Tell qmake to add the moving of them to the 'install' target
    INSTALLS += plugins
    INSTALLS += other_files
    INSTALLS += resources
    INSTALLS += emptyfolder

#MAKE FOLDER STRUCTURE
system(mkdir $$DESTDIR/HypnosContent)
system(mkdir $$DESTDIR/HypnosContent/Contents)
system(mkdir $$DESTDIR/HypnosContent/Contents/LinuxOS)
system(mkdir $$DESTDIR/HypnosContent/Contents/Components)
system(mkdir $$DESTDIR/HypnosContent/Contents/Resources)
system(mkdir $$DESTDIR/HypnosContent/Contents/Frameworks)
system(mkdir $$DESTDIR/HypnosContent/Contents/Plugins)

#OUT_PWD = $$DESTDIR/Contents/LinuxOS

#COPY PLUGINS
system(cp $${OGREPLUGINS}/Plugin_BSPSceneManager.so $$DESTDIR/HypnosContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_CgProgramManager.so $$DESTDIR/HypnosContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_OctreeSceneManager.so $$DESTDIR/HypnosContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_OctreeZone.so $$DESTDIR/HypnosContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_ParticleFX.so $$DESTDIR/HypnosContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_PCZSceneManager.so $$DESTDIR/HypnosContent/Contents/Components)
system(cp $${OGREPLUGINS}/RenderSystem_GL.so $$DESTDIR/HypnosContent/Contents/Components)
system(cp $${OGREPLUGINS}/Plugin_PCZSceneManager.so $$DESTDIR/HypnosContent/Contents/Components)

#COPY LIBS
system(cp $${OGRELIB}/libOgreMain.so $$DESTDIR/HypnosContent/Contents/Frameworks)
system(cp $${OGRELIB}/libOgrePaging.so $$DESTDIR/HypnosContent/Contents/Plugins)
system(cp $${OGRELIB}/libOgreProperty.so $$DESTDIR/HypnosContent/Contents/Plugins)
system(cp $${OGRELIB}/libOgreRTShaderSystem.so $$DESTDIR/HypnosContent/Contents/Plugins)
system(cp $${OGRELIB}/libOgreTerrain.so $$DESTDIR/HypnosContent/Contents/Plugins)
system(cp $${OGRECONFIGS}/resources.cfg $$DESTDIR/HypnosContent/Contents/Resources)
system(cp $${OGRECONFIGS}/quakemap.cfg $$DESTDIR/HypnosContent/Contents/Resources)
system(cp $${OGRECONFIGS}/samples.cfg $$DESTDIR/HypnosContent/Contents/Resources)
system(cp $${OGRECONFIGS}/tests.cfg $$DESTDIR/HypnosContent/Contents/Resources)
system(cp $${OGRECONFIGS}/plugins.cfg $$DESTDIR/HypnosContent/Contents/Resources)
system(cp entropy_resources.cfg $$DESTDIR/HypnosContent/Contents/Resources)
system(cp -R $${ENTROPYRESOURCES} $$DESTDIR/HypnosContent/Contents/Resources)

#stuff for copying shaders
system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Media/DeferredShadingMedia)
system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Media/materials/scripts)
#system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Media/materials/programs)
system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/LuaScripts)
system(rm -r $${OUT_PWD}/Hypnos.app/Contents/Resources/resources/Shaders)

system(cp -R resources $${OUT_PWD}/HypnosContent/Contents/Resources)
system(cp -R resources/Media/DeferredShadingMedia $${OUT_PWD}/HypnosContent/Contents/Resources/resources/Media/DeferredShadingMedia)
system(cp -R resources/Media/materials/scripts $${OUT_PWD}/HypnosContent/Contents/Resources/resources/Media/materials/scripts)
#system(cp -R resources/Media/materials/programs $${OUT_PWD}/HypnosContent/Contents/Resources/resources/Media/materials/programs)
system(cp -R resources/LuaScripts $${OUT_PWD}/HypnosContent/Contents/Resources/resources/LuaScripts)
system(cp -R resources/Shaders $${OUT_PWD}/HypnosContent/Contents/Resources/resources/Shaders)
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

    LIBS += $${ENTROPYLIB}/Entropy.lib
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

    PRE_TARGETDEPS +=
        $${ENTROPYLIB}/Entropy.lib \

    #CONFIG(debug, debug|release) {
    #    TARGET = $$join(TARGET,,,d)
    #    LIBS *= -lOgreMain_d -lOIS_d
    #}

    #CONFIG(release, debug|release):LIBS *= -lOgreMain -lOIS

    #MY_LIB_FILES += $${OGRE_DLLS}/OgreMain.dll
    #MY_LIB_FILES += $${OGRE_DLLS}/LIBGCC_S_DW2-1.DLL
    #MY_LIB_FILES += $${OGRE_DLLS}/QTCORE4.DLL
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
        entropy_resources.cfg \
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
        entropy_resources.cfg \

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

INCLUDEPATH += src
DEPENDPATH += src

HEADERS += \
    Hypnos/src/Core/hypnos.h \
    Hypnos/src/script/luafunctions.h

SOURCES += \
    Hypnos/src/script/luafunctions.cpp \
    Hypnos/src/Core/hypnos.cpp

linux-g++ {
    # UNCOMMENT FOR LINUX BUNDLE BUILDING
    #QMAKE_POST_LINK = ../Entropy/LinuxBundle.sh
}

OTHER_FILES +=
