// Library includes
#include "libraries/tinyxml/tinyxml.h"
#include "libraries/tinyxml/tinystr.h"

// Local includes
#include "scene.h"
#include "world.h"
#include "physicstypes.h"
#include "src/Physics/collisiongroupmanager.h"


namespace physics {

///////////////
// SceneObject
///////////////

SceneObject::SceneObject(std::string name, const Vec3 &position, const Vec3 &scale, double mass,
                         const Quaternion &rotation, BodyType bodyType, ShapeType shapeType,
                         collision_group_id_t collisionGroup, CursorType cursorType, bool physicsEnabled,
                         bool hollow) :
    name(name), position(position), scale(scale), mass(mass), rotation(rotation), bodyType(bodyType),
    shapeType(shapeType), collisionGroup(collisionGroup), cursorType(cursorType), physicsEnabled(physicsEnabled),
    hollow(hollow)
{

}

std::string SceneObject::getName()
{
    return name;
}

void SceneObject::setName(std::string name)
{
    this->name = name;
}

Vec3 SceneObject::getPosition()
{
    return position;
}

void SceneObject::setPosition(const Vec3 &position)
{
    this->position = position;
}

Vec3 SceneObject::getScale()
{
    return scale;
}

void SceneObject::setScale(const Vec3 &scale)
{
    this->scale = scale;
}

double SceneObject::getMass()
{
    return mass;
}

void SceneObject::setMass(double mass)
{
    this->mass = mass;
}

Quaternion SceneObject::getRotation()
{
    return rotation;
}

void SceneObject::setRotation(const Quaternion &rotation)
{
    this->rotation = rotation;
}

BodyType SceneObject::getBodyType()
{
    return bodyType;
}

void SceneObject::setBodyType(BodyType bodyType)
{
    this->bodyType = bodyType;
}

ShapeType SceneObject::getShapeType()
{
    return shapeType;
}

void SceneObject::setShapeType(ShapeType shapeType)
{
    this->shapeType = shapeType;
}

collision_group_id_t SceneObject::getCollisionGroup()
{
    return collisionGroup;
}

void SceneObject::setCollisionGroup(collision_group_id_t collisionGroup)
{
    this->collisionGroup = collisionGroup;
}

CursorType SceneObject::getCursorType()
{
    return cursorType;
}

void SceneObject::setCursorType(CursorType cursorType)
{
    this->cursorType = cursorType;
}

bool SceneObject::getPhysicsEnabled()
{
    return physicsEnabled;
}

void SceneObject::setPhysicsEnabled(bool physicsEnabled)
{
    this->physicsEnabled = physicsEnabled;
}

bool SceneObject::getHollow()
{
    return hollow;
}

void SceneObject::setHollow(bool hollow)
{
    this->hollow = hollow;
}

/////////
// Scene
/////////

Scene::Scene(std::string name) :
    name(name)
{

}

Scene::~Scene()
{
    for(int i = 0; i < objects.size(); ++i)
    {
        delete objects[i];
        objects[i] = 0;
    }

    objects.clear();
}

std::string Scene::getName()
{
    return name;
}

void Scene::addObject(SceneObject* object)
{
    objects.push_back(object);
}

void Scene::addObject(std::string objectName, const Vec3 &position, const Vec3 &scale, double mass, const Quaternion &rotation)
{
    objects.push_back(new SceneObject(objectName, position, scale, mass, rotation));
}

const std::vector<SceneObject*>& Scene::getObjects()
{
    return objects;
}

SceneObject* Scene::lastObject()
{
    return objects[objects.size() -1];
}


/////////////////
// Import Scene
/////////////////

void recursiveXmlIterate(const TiXmlElement* parent, Scene* scene)
{
    const TiXmlNode* child(0);
    while(child = parent->IterateChildren(child))
    {
        const char* value = child->ToElement()->Value();

        if(strcmp(value, "node") == 0)
        {
            std::string name(child->ToElement()->Attribute("name"));
            scene->addObject(new SceneObject(name));
        }

        else if(strcmp(value, "position") == 0)
        {
            double x, y, z;
            child->ToElement()->Attribute("x", &x);
            child->ToElement()->Attribute("y", &y);
            child->ToElement()->Attribute("z", &z);
            scene->lastObject()->setPosition(Vec3(x, y, z));
        }

        else if(strcmp(value, "scale") == 0)
        {
            double x, y, z;
            child->ToElement()->Attribute("x", &x);
            child->ToElement()->Attribute("y", &y);
            child->ToElement()->Attribute("z", &z);
            scene->lastObject()->setScale(Vec3(x, y, z));
        }

        else if(strcmp(value, "user_data") == 0)
        {
            const char* tagName = child->ToElement()->Attribute("name");

            if(strcmp(tagName, "PhysicsEnabled") == 0)
            {
                int physicsEnabled;
                child->ToElement()->Attribute("value", &physicsEnabled);
                scene->lastObject()->setPhysicsEnabled(physicsEnabled);
            }

            else if(strcmp(tagName, "Mass") == 0)
            {
                double mass;
                child->ToElement()->Attribute("value", &mass);
                scene->lastObject()->setMass(mass);
            }

            else if(strcmp(tagName, "BodyType") == 0)
            {
                const char* typeString = child->ToElement()->Attribute("value");
                BodyType bodyType = Rigid;

                if(strcmp(typeString, "Ghost") == 0)
                    bodyType = Ghost;
                else if(strcmp(typeString, "Rigid") == 0)
                    bodyType = Rigid;
                else if(strcmp(typeString, "Soft") == 0)
                    bodyType = Soft;

                scene->lastObject()->setBodyType(bodyType);
            }

            else if(strcmp(tagName, "ShapeType") == 0)
            {
                const char* typeString = child->ToElement()->Attribute("value");
                ShapeType shapeType = Box;

                if(strcmp(typeString, "Box") == 0)
                    shapeType = Box;
                else if(strcmp(typeString, "Sphere") == 0)
                    shapeType = Sphere;
                else if(strcmp(typeString, "Mesh") == 0)
                    shapeType = Mesh;

                scene->lastObject()->setShapeType(shapeType);
            }

            else if(strcmp(tagName, "Hollow") == 0)
            {
                int hollow;
                child->ToElement()->Attribute("value", &hollow);
                scene->lastObject()->setHollow(hollow);
            }

            else if(strcmp(tagName, "CollisionGroup") == 0)
            {
                const char* typeString = child->ToElement()->Attribute("value");

                collision_group_id_t groupID = physics::MAIN_GROUP_ID;

                if(!CollisionGroupManager::containsCollisionGroup(typeString, groupID))
                {
                    groupID = CollisionGroupManager::addCollisionGroup(typeString);
                }

                scene->lastObject()->setCollisionGroup(groupID);

                /*
                CollisionGroup collisionGroup = DefaultGroup;

                if(strcmp(typeString, "DefaultGroup") == 0)
                    collisionGroup = DefaultGroup;
                else if(strcmp(typeString, "HeroGroup") == 0)
                    collisionGroup = HeroGroup;
                else if(strcmp(typeString, "EnemyGroup") == 0)
                    collisionGroup = EnemyGroup;

                scene->lastObject()->setCollisionGroup(collisionGroup);*/
            }

            else if(strcmp(tagName, "CursorType") == 0)
            {
                const char* typeString = child->ToElement()->Attribute("value");
                CursorType cursorType = NoCursor;

                if(strcmp(typeString, "NoCursor") == 0)
                    cursorType = NoCursor;
                else if(strcmp(typeString, "PickUpCursor") == 0)
                    cursorType = PickUpCursor;
                else if(strcmp(typeString, "HackCursor") == 0)
                    cursorType = HackCursor;
                else if(strcmp(typeString, "LookCursor") == 0)
                    cursorType = LookCursor;
                else if(strcmp(typeString, "ActivateCursor") == 0)
                    cursorType = ActivateCursor;

                scene->lastObject()->setCursorType(cursorType);
            }
        }

        else if(strcmp(value, "rotation") == 0)
        {
            double x, y, z, w;
            child->ToElement()->Attribute("qx", &x);
            child->ToElement()->Attribute("qy", &y);
            child->ToElement()->Attribute("qz", &z);
            child->ToElement()->Attribute("qw", &w);
            scene->lastObject()->setRotation(Quaternion(x, y, z, w));
        }

        recursiveXmlIterate(child->ToElement(), scene);
    }
}

bool importScene(Scene *scene, std::string file)
{
    TiXmlDocument document(file.c_str());

    try
    {
        bool fileLoaded = document.LoadFile();

        if(!fileLoaded)
            return fileLoaded;
    }

    catch(...)
    {
        std::cerr << "Failed reading XML file " << std::endl;
        return false;
    }

    return importScene(scene, &document);
}

bool importScene(Scene *scene, TiXmlDocument* document)
{
    try
    {
        const TiXmlElement* root = document->FirstChildElement("scene");
        recursiveXmlIterate(root, scene);
    }

    catch(...)
    {
        std::cerr << "Failed reading XML file " << std::endl;
        return false;
    }

    return true;
}

} // physics namespace
