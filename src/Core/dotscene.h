#ifndef DOTSCENE_H
#define DOTSCENE_H

// System includes
#include <vector>

// Ogre includes
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreLight.h>

// TinyXML
#include "libraries/tinyxml/tinyxml.h"

// Local includes
#include "src/Physics/physicsobject.h"

namespace dotscene {

struct NodeProperty
{
    std::string nodeName, propertyName, valueName;

    NodeProperty(std::string nodeName, std::string propertyName, std::string valueName) : nodeName(nodeName),
        propertyName(propertyName), valueName(valueName) { }
};

extern std::vector<NodeProperty> nodeProperties;

Ogre::Light* loadLight(TiXmlElement* xmlLight, Ogre::SceneManager* sceneManager);
std::vector<physics::PhysicsActor*> parseDotScene(const std::string& sceneName, const std::string& groupName,
                                         Ogre::SceneManager* sceneManager);
bool getProperty(std::string nodeName, std::string propertyName, std::string& value);

} // dotscene namespace


#endif // DOTSCENE_H
