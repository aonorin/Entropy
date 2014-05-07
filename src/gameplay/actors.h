#ifndef ACTORS_H
#define ACTORS_H

// System includes
#include <string>
#include <vector>

#include "Script/scripttypes.h"

namespace physics {
    class PhysicsActor;
}

namespace gameplay {

    namespace actors {

        void update();
        void updateTransform(); // Different to fix crashing
        void setActors(std::vector<physics::PhysicsActor*> actorVector);
        void addActor(physics::PhysicsActor* physicsActor);
        void removeActor(std::string name, bool deleteActor = true);
        void createActor(std::string name, std::string modelName, std::string materialName,
                            int x, int y, int z);
        physics::PhysicsActor* getActor(std::string actorName);
        void clearActors();

        namespace detail {
            extern std::vector<physics::PhysicsActor*> actors;
        } // detail namespace

    } // actors namespace

} // gameplay namespace

#endif // ACTORS_H
