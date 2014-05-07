#include "scenemanager.h"
#include "gameplay/maps/roguemapfactory.h"

using namespace neuro;

SceneManager::SceneManager() :
    current_scene(0),
    initialized(false)
{

}

bool SceneManager::isInitialized()
{
    return initialized;
}

void SceneManager::init()
{
    RogueMapFactory rmf;
    Map::Ptr map = rmf.createMap();
    scenes.push_back(map);
    map->init();
    initialized = true;
}

void SceneManager::continuous_update()
{
    scenes.at(current_scene)->continuous_update();
    if(scenes.at(current_scene)->isComplete())
    {
        if(current_scene<total_scenes-1)
        {
            current_scene++;
        }
    }
}

void SceneManager::update_turn(double turn_number)
{
    scenes.at(current_scene)->update_turn(turn_number);
}

Scene::Ptr SceneManager::getCurrentScene()
{
    return scenes.at(current_scene);
}
