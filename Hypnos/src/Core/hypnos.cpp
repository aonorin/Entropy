#include "hypnos.h"
#include "Core/update.h"
#include "Hypnos/src/script/luafunctions.h"

Hypnos::Hypnos()
{
#ifdef __APPLE__
//        thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Hypnos::purgeMemory, this)));
#endif

//    startNewGame();
}

Hypnos::~Hypnos()
{

}

void Hypnos::purgeMemory()
{
//    system("purge");
//    boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
//    startNewGame();
    thread->join();
}

void Hypnos::createScene()
{
    Entropy::createScene();
}

void Hypnos::startNewGame()
{
    Entropy::startNewGame();
    hypnos::LuaFunctions func();
    std::cout << "Hypnos::createScene()" << std::endl;
    // tgen = new hypnos::TerrainGenerator();
    //    terrainGenerator = boost::shared_ptr<voxel::TerrainGenerator>(new hypnos::MapGen(50,50,50));
//    terrainGenerator = boost::shared_ptr<voxel::TerrainGenerator>(new voxel::TerrainGenerator(MAPSIZE_X_NONSCALED,MAPSIZE_Y_NONSCALED,MAPSIZE_Z_NONSCALED,MAPSIZE_SCALE));
//    terrainGenerator->genSimplex(2,5,2,0.115,0.115,0.2);
    //    Int8ArrayPtr_t tempMap(new Int8Array_t(33000000,-1));
    //    gameplay::astar.setMap(tempMap);
    //    Entropy::getSingletonPtr()->mCameraMan->setPosition(btVector3(160,160,160));
    /*
        Entropy::getSingletonPtr()->mCameraMan->setCameraPosition(0,0,0);
    gameplay::astar.setPos(Entropy::getSingletonPtr()->mCameraMan->getPathID(),
                           Ogre::Vector3(
                               Entropy::getSingletonPtr()->mCameraMan->getPosition().x(),
                               Entropy::getSingletonPtr()->mCameraMan->getPosition().y(),
                               Entropy::getSingletonPtr()->mCameraMan->getPosition().z()
                               )
                           );
                           */
    //    hypnos::DoomBot::constructDoomBot();
}

#ifndef __APPLE__
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
    // Create application object
    Hypnos app;

    try {
        app.go();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        //        MessageBox( NULL, CT2CW(e.getFullDescription().c_str()), L"An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#else
        std::cerr << "An exception has occured: " <<
                     e.getFullDescription().c_str() << std::endl;
#endif
    } catch (std::exception& e) {
        std::cerr << "An exception has occured: " << e.what() << std::endl;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
#endif
