#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "scene.h"
#include "gameplay/updateable.h"

namespace neuro
{

class SceneManager : public Updateable
{
public:
    typedef boost::shared_ptr<SceneManager> Ptr;
    SceneManager();

    void init();
    void continuous_update();
    void update_turn(double turn_number);

    void update_action() {}
    void complete_action() {}

    bool isInitialized();
    Scene::Ptr getCurrentScene();

private:
    int current_scene;
    int total_scenes;
    bool initialized;
    Scene::PtrArray scenes;
};

}

#endif // SCENEMANAGER_H
