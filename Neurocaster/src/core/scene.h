#ifndef SCENE_H
#define SCENE_H

#include "boost/shared_ptr.hpp"

#include <vector>

#include "gameplay/updateable.h"

class Scene : public Updateable
{
public:
    typedef boost::shared_ptr<Scene> Ptr;
    typedef std::vector<Ptr> PtrArray;

    Scene();

    virtual void continuous_update() = 0;
    virtual void update_turn(double turn_number) = 0;
    virtual void new_round() = 0;

    bool isComplete();

    virtual void init() {}

private:
    bool complete;
};

#endif // SCENE_H
