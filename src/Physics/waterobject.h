#ifndef WATEROBJECT_H
#define WATEROBJECT_H

#include <vector>
#include "src/Physics/physicstypes.h"
#include "src/Physics/physicsobject.h"
#include "src/gameplay/CollisionAction.h"

namespace physics {

// WaterObject
class WaterObject : public GhostObject
{
public:

    WaterObject(btGhostObject* object, collision_group_id_t collisionGroup = MAIN_GROUP_ID, CursorType cursorType = NoCursor);
    WaterObject(const Vec3& origin, const Vec3& boxHalfExtents, const Quaternion& rotation = Quaternion(0, 1, 0, 0), bool addToWorld = true,
                collision_group_id_t collisionGroup = MAIN_GROUP_ID, CursorType cursorType = NoCursor);

protected:

    virtual void objectCollisionStarted(btCollisionObject* object); // begin collision callback
    virtual void objectCollisionEnded(btCollisionObject* object); // end collision callback
};

} // physics namespace

#endif // WATEROBJECT_H
