QT       += core gui

TARGET = Degenerator
TEMPLATE = app

include( ../QMakeVars.pri )

DEFINES += BOOST_ALL_NO_LIB \
    USE_RTSHADER_SYSTEM

QMAKE_CXXFLAGS_RELEASE += -IOgre -O0 -g
QMAKE_CXXFLAGS_DEBUG += -IOgre -O0 -g

QMAKE_CXXFLAGS_RELEASE += -visibility=default
QMAKE_CXXFLAGS_DEBUG += -visibility=default

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

mac {
    QMAKE_CFLAGS += -D__APPLE__
    CFLAGS += -D__APPLE__

    INCLUDEPATH += $${OGREHOME}/include/OGRE \
        $${OGREHOME}/include/OGRE/RTShaderSystem \
        $${OGREHOME}/include \
        $${OGREHOME}/common/include \
        $${OGREHOME}/include/OGRE/OSX \
        $${OGREHOME}/include/OGRE/RTShaderSystem \
        $${OGREHOME}/include/Terrain \
        $${OGREHOME}/include/Paging \
        $${OGREHOME}/include/OIS \
        $${BOOSTINCLUDE} \
        $${BULLETINCLUDE} \
        $${SLBINCLUDE} \
        $${SLBINCLUDE}/../src \
        $${OISINCLUDE} \
        $${SDLINCLUDE} \
        Degenerator \
        Degenerator/src \
        #/usr/include/pcl-1.5

    DEPENDPATH += $${BULLETLIBS}

    QMAKE_LFLAGS += -F$${OGREHOME}/lib/release/Ogre.framework
    #QMAKE_CFLAGS += I$${OGREHOME}/include/OGRE/RTShaderSystem -pthread

    LIBS += -framework Carbon \
    -framework Cocoa \
    -framework Foundation \
    -framework IOKit \
    -framework AppKit \
    -framework Ogre \
    $${BOOSTLIBS}/libboost_date_time.a \
    $${BOOSTLIBS}/libboost_thread.a \
    $${BOOSTLIBS}/libboost_signals.a \
    $${BOOSTLIBS}/libboost_system.a \
    $${OGREHOME}/lib/libOgreRTShaderSystem.dylib \
    $${OGREHOME}/lib/libOgreTerrain.dylib \
    $${OGREHOME}/lib/libOgrePaging.dylib \
    $${OGREHOME}/lib/release/libOIS.a \
    $${BULLETLIBS}/BulletCollision/libBulletCollision.a \
    $${BULLETLIBS}/BulletDynamics/libBulletDynamics.a \
    $${BULLETLIBS}/BulletMultiThreaded/libBulletMultiThreaded.a \
    $${BULLETLIBS}/BulletSoftBody/libBulletSoftBody.a \
    $${BULLETLIBS}/LinearMath/libLinearMath.a \
    $${BULLETLIBS}/MiniCL/libMiniCL.a \
    $${SLBLIBS}/libSLB.a \
    $${SDLLIBS}/libSDL2.dylib \
    #/usr/lib/libpcl_common.1.5.1.dylib \
    #/usr/lib/libpcl_octree.1.5.1.dylib \
    #/usr/lib/libpcl_io.1.5.1.dylib \
    #/usr/lib/libpcl_sample_consensus.1.5.1.dylib \
    #/usr/lib/libpcl_kdtree.1.5.1.dylib \
    #/usr/lib/libpcl_search.1.5.1.dylib \
    #/usr/lib/libpcl_filters.1.5.1.dylib \
    #/usr/lib/libpcl_segmentation.1.5.1.dylib \
    #/usr/lib/libpcl_visualization.1.5.1.dylib \
    #/usr/lib/libpcl_features.1.5.1.dylib \
    #/usr/lib/libpcl_surface.1.5.1.dylib \
    #/usr/lib/libpcl_registration.1.5.1.dylib \
    #/usr/lib/libpcl_keypoints.1.5.1.dylib \
    #/usr/lib/libpcl_tracking.1.5.1.dylib \
    #/usr/lib/libpcl_apps.1.5.1.dylib

    RESOURCE_FILES.files += $${OGREHOME}/bin/plugins.cfg \
        $${OGREHOME}/bin/quakemap.cfg \
        $${OGREHOME}/bin/samples.cfg \
        entropy_resources.cfg \

    RESOURCE_FILES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += RESOURCE_FILES

    COMPONENTS.files += $${OGREHOME}/lib/libOgrePaging.dylib \
        $${OGREHOME}/lib/libOgreProperty.dylib \
        $${OGREHOME}/lib/libOgreRTShaderSystem.dylib \
        $${OGREHOME}/lib/libOgreTerrain.dylib

    COMPONENTS.path = Contents/Components
    QMAKE_BUNDLE_DATA += COMPONENTS

    FRAMEWORKS.files = $${OGREHOME}/lib/release/Ogre.framework
    FRAMEWORKS.path = Contents/Frameworks
    QMAKE_BUNDLE_DATA += FRAMEWORKS



    PLUGINS.files += $${OGREHOME}/lib/Plugin_BSPSceneManager.dylib \
        $${OGREHOME}/lib/Plugin_CgProgramManager.dylib \
        $${OGREHOME}/lib/Plugin_OctreeSceneManager.dylib \
        $${OGREHOME}/lib/Plugin_OctreeZone.dylib \
        $${OGREHOME}/lib/Plugin_ParticleFX.dylib \
        $${OGREHOME}/lib/Plugin_PCZSceneManager.dylib \
        $${OGREHOME}/lib/RenderSystem_GL.dylib

    PLUGINS.path = Contents/Plugins
    QMAKE_BUNDLE_DATA += PLUGINS

    MEDIA.files = $${OGREHOME}/Media/
    MEDIA.path = ../Media
    QMAKE_BUNDLE_DATA += MEDIA

    MATERIALS.files += resources/OgreDebugDraw/Debug.material
    MATERIALS.path = ../Media/materials/scripts
    QMAKE_BUNDLE_DATA += MATERIALS

    #OBJECT_FILES.files = objectData/dummy
    #OBJECT_FILES.path = ../ObjectData
    #QMAKE_BUNDLE_DATA += OBJECT_FILES

    ERESOURCES.files = resources
    ERESOURCES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += ERESOURCES

    TEST_LEVEL.files = resources/scenes/TestLevel
    TEST_LEVEL.path = Contents/Resources/resources/scenes
    QMAKE_BUNDLE_DATA += TEST_LEVEL

    # Entropy lib
    LIBS += $${ENTROPYLIB}/libEntropy.a
    INCLUDEPATH += $${ENTROPYINCLUDE}

} else:unix {

    INCLUDEPATH += $${OGREHOME}/include/OGRE \
        $${OGREHOME}/include/OGRE/RTShaderSystem \
        $${OGREHOME}/include/OIS \
        $${OGREHOME}/boost_1_46_1

    LIBS += $${OGREHOME}/lib/libOgreMain.so \
        $${OGREHOME}/lib/libOIS.so
}

win32 {
    QMAKE_CFLAGS += -D__WINDOWS__
    CFLAGS += -D__WINDOWS__

    LIBS += -L$$OGREHOME\\boost_1_42\\lib
    Release:LIBS += -L$$OGREHOME\\lib\\release
    Debug:LIBS += -L$$OGREHOME\\lib\\debug
    INCLUDEPATH += $$OGREHOME\\include
    INCLUDEPATH += $$OGREHOME\\include\\OIS
    INCLUDEPATH += $$OGREHOME\\include\\OGRE
    INCLUDEPATH += $$OGREHOME\\boost_1_42

    CONFIG(debug, debug|release) {
        TARGET = $$join(TARGET,,,d)
        LIBS *= -lOgreMain_d -lOIS_d
    }

    CONFIG(release, debug|release):LIBS *= -lOgreMain -lOIS
}


SOURCES += Degenerator/main.cpp\
        Degenerator/mainwindow.cpp \
    Degenerator/src/ogrewidget.cpp \
    Degenerator/src/update.cpp \
    Degenerator/src/meshgenerator.cpp \
    Degenerator/src/algorithm.cpp \
    Degenerator/src/algorithms/testalgorithm.cpp \
    Degenerator/src/algorithms/corruptedsphere.cpp \
    Degenerator/src/algorithms/flockingmesh.cpp \
    Degenerator/src/algorithms/strangemesh.cpp \
    Degenerator/src/greedysurface.cpp \
    Degenerator/src/algorithms/lsystemmesh.cpp \
    Degenerator/src/meshfunctions.cpp \
    Degenerator/src/algorithms/wolframcamesh.cpp \
    Degenerator/src/algorithms/gameoflifemesh.cpp

HEADERS  += Degenerator/mainwindow.h \
    Degenerator/src/ogrewidget.h \
    Degenerator/src/ColouredRectangle.h \
    Degenerator/src/CameraMan.h \
    Degenerator/src/update.h \
    Degenerator/src/meshgenerator.h \
    Degenerator/src/algorithm.h \
    Degenerator/src/algorithms/testalgorithm.h \
    Degenerator/src/algorithms/corruptedsphere.h \
    Degenerator/src/algorithms/flockingmesh.h \
    Degenerator/src/algorithms/strangemesh.h \
    Degenerator/src/greedysurface.h \
    Degenerator/src/algorithms/lsystemmesh.h \
    Degenerator/src/meshfunctions.h \
    Degenerator/src/algorithms/wolframcamesh.h \
    Degenerator/src/algorithms/gameoflifemesh.h

FORMS    += Degenerator/mainwindow.ui

OTHER_FILES += \
    Degenerator/entropy_resources.cfg \
    resources/DegenTestMaterial.material
