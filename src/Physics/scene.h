#ifndef IMPORTSCENE_H
#define IMPORTSCENE_H

// System includes
#include <string>
#include <vector>

// Local includes
#include "src/Physics/physicstypes.h"
#include "src/Physics/collisiongroupmanager.h"

class TiXmlDocument;

namespace physics {

class SceneObject {

public:
    SceneObject(std::string name = "", const Vec3 &position = Vec3(0, 0, 0), const Vec3 &scale = Vec3(0, 0, 0),
                double mass = 0.0, const Quaternion &rotation = Quaternion(0, 0, 0, 0), BodyType bodyType = Rigid,
                ShapeType shapeType = Box, collision_group_id_t collisionGroup = MAIN_GROUP_ID,
                CursorType cursorType = NoCursor, bool physicsEnabled = false, bool hollow = false);

    std::string getName();
    void setName(std::string name);
    Vec3 getPosition();
    void setPosition(const Vec3 &position);
    Vec3 getScale();
    void setScale(const Vec3 &scale);
    double getMass();
    void setMass(double mass);
    Quaternion getRotation();
    void setRotation(const Quaternion &rotation);
    BodyType getBodyType();
    void setBodyType(BodyType bodyType);
    ShapeType getShapeType();
    void setShapeType(ShapeType shapeType);
    collision_group_id_t getCollisionGroup();
    void setCollisionGroup(collision_group_id_t collisionGroup);
    CursorType getCursorType();
    void setCursorType(CursorType cursorType);
    bool getPhysicsEnabled();
    void setPhysicsEnabled(bool physicsEnabled);
    bool getHollow();
    void setHollow(bool hollow);

private:

    std::string name;
    Vec3 position, scale;
    double mass;
    Quaternion rotation;
    BodyType bodyType;
    collision_group_id_t collisionGroup;
    CursorType cursorType;
    ShapeType shapeType;
    bool physicsEnabled, hollow;
};

class Scene {

public:

    Scene(std::string name);
    ~Scene();

    std::string getName();
    void addObject(SceneObject* object);
    void addObject(std::string objectName, const Vec3 &position, const Vec3 &scale, double mass, const Quaternion &rotation);
    const std::vector<SceneObject*>& getObjects();
    SceneObject* lastObject();

private:

    std::string name;
    std::vector<SceneObject*> objects;
};

bool importScene(Scene* scene, std::string file);
bool importScene(Scene* scene, TiXmlDocument* document);

} // physics namespace

#endif // IMPORTSCENE_H
