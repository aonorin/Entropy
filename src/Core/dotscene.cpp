// Ogre includes
#include <OGRE/OgreLight.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreBillboardSet.h>
#include <OGRE/OgreString.h>
#include <OGRE/OgreMaterialManager.h>

// Entropy includes
#include "dotscene.h"
#include "src/Physics/physics.h"
#include "Graphics/material.h"

using namespace Ogre;
using namespace std;

namespace dotscene {

std::vector<NodeProperty> nodeProperties;

Ogre::Light* loadLight(TiXmlElement* xmlLight, Ogre::SceneManager* sceneManager)
{
    TiXmlElement* xmlDiffuse;
    TiXmlElement* xmlSpecular;
    TiXmlElement* xmlAttenuation;
    TiXmlElement* xmlPosition;

    Light* light = sceneManager->createLight();

    if(!xmlLight->Attribute("type") || strcmp(xmlLight->Attribute("type"), "point") == 0)
        light->setType(Light::LT_POINT);
    else if(strcmp(xmlLight->Attribute("type"), "spot") == 0)
        light->setType(Light::LT_SPOTLIGHT);
    else if(strcmp(xmlLight->Attribute("type"), "directional") == 0)
        light->setType(Light::LT_DIRECTIONAL);
    else if(strcmp(xmlLight->Attribute("type"), "radPoint") == 0)
        light->setType(Light::LT_POINT);

    xmlDiffuse = xmlLight->FirstChildElement("colourDiffuse");

    if(xmlDiffuse)
    {
        double red, green, blue;
        xmlDiffuse->Attribute("r", &red);
        xmlDiffuse->Attribute("g", &green);
        xmlDiffuse->Attribute("b", &blue);
        ColourValue diffuse(red, green, blue);
        light->setDiffuseColour(diffuse);
    }

    xmlSpecular = xmlLight->FirstChildElement("colourSpecular");

    if(xmlSpecular)
    {
        double red, green, blue;
        xmlSpecular->Attribute("r", &red);
        xmlSpecular->Attribute("g", &green);
        xmlDiffuse->Attribute("b", &blue);
        ColourValue diffuse(red, green, blue);
        light->setSpecularColour(diffuse);
    }

    xmlAttenuation = xmlLight->FirstChildElement("lightAttenuation");

    if(xmlAttenuation)
    {
        double range, constant, linear, quadratic;
        range = light->getAttenuationRange();
        constant = light->getAttenuationConstant();
        linear = light->getAttenuationLinear();
        quadratic = light->getAttenuationQuadric();

        if(xmlAttenuation->Attribute("range"))
            xmlAttenuation->Attribute("range", &range);
        if(xmlAttenuation->Attribute("constant"))
            xmlAttenuation->Attribute("constant", &constant);
        if(xmlAttenuation->Attribute("linear"))
            xmlAttenuation->Attribute("linear", &linear);
        if(xmlAttenuation->Attribute("quadratic"))
            xmlAttenuation->Attribute("quadratic", &quadratic);

        light->setAttenuation(range, constant, linear, quadratic);
    }

    xmlPosition = xmlLight->FirstChildElement("position");

    if(xmlPosition)
    {
        double x, y, z;
        x = y = z = 0.0;

        if(xmlPosition->Attribute("x"))
            xmlPosition->Attribute("x", &x);
        if(xmlPosition->Attribute("y"))
            xmlPosition->Attribute("y", &y);
        if(xmlPosition->Attribute("z"))
            xmlPosition->Attribute("z", &z);

        Vector3 position(x, y, z);
        light->setPosition(position);
    }

    xmlPosition = xmlLight->FirstChildElement("direction");

    if(xmlPosition)
    {
        double x, y, z;
        x = y = z = 0.0;

        if(xmlPosition->Attribute("x"))
            xmlPosition->Attribute("x", &x);
        if(xmlPosition->Attribute("y"))
            xmlPosition->Attribute("y", &y);
        if(xmlPosition->Attribute("z"))
            xmlPosition->Attribute("z", &z);

        Vector3 direction(x, y, z);
        light->setDirection(direction);
    }

    xmlPosition = xmlLight->FirstChildElement("spotlightRange");

    if(xmlPosition)
    {
        double inner, outer, falloff;

        if(xmlPosition->Attribute("inner"))
            xmlPosition->Attribute("inner", &inner);
        if(xmlPosition->Attribute("outer"))
            xmlPosition->Attribute("outer", &outer);
        if(xmlPosition->Attribute("falloff"))
            xmlPosition->Attribute("falloff", &falloff);

        light->setSpotlightRange(Ogre::Radian(inner), Ogre::Radian(outer), falloff);
    }

    light->setCastShadows(true);

    if(xmlLight->Attribute("castShadows"))
    {
        int shadowBool;
        xmlLight->Attribute("castShadows", &shadowBool);
        light->setCastShadows(shadowBool);
    }

    light->setVisible(true);

    if(xmlLight->Attribute("visible"))
    {
        int visibleBool;
        xmlLight->Attribute("visible", &visibleBool);
        light->setVisible(visibleBool);
    }

    return light;
}

std::vector<physics::PhysicsActor*> parseDotScene(const std::string& sceneName, const std::string& groupName,
                                                  Ogre::SceneManager* sceneManager)
{
    std::vector<physics::PhysicsActor*> actors;
    TiXmlDocument* xmlDoc = 0;
    TiXmlElement* xmlRoot = 0;
    TiXmlElement* xmlNodes = 0;
    TiXmlElement* xmlEnvir = 0;

    try
    {
        DataStreamPtr pStream = ResourceGroupManager::getSingleton().openResource(sceneName, groupName);
        String data = pStream.getPointer()->getAsString();

        xmlDoc = new TiXmlDocument();
        xmlDoc->Parse(data.c_str());
        pStream.getPointer()->close();
        pStream.setNull();

        if(xmlDoc->Error())
        {
            LogManager::getSingleton().logMessage("[dotSceneLoader] The TiXmlDocument reported an error");

            if(xmlDoc)
                delete xmlDoc;

            return actors;
        }
    }

    catch(...)
    {
        LogManager::getSingleton().logMessage("[dotSceneLoader] Error creating TiXmlDocument");

        if(xmlDoc)
            delete xmlDoc;

        return actors;
    }

    xmlRoot = xmlDoc->RootElement();

    if(strcmp(xmlRoot->Value(), "scene") != 0)
    {
        LogManager::getSingleton().logMessage("[dotSceneLoader]Error: Invalid .scene File. Missing <scene>");

        if(xmlDoc)
            delete xmlDoc;

        return actors;
    }

    xmlNodes = xmlRoot->FirstChildElement("nodes");

    if(xmlNodes)
    {
        TiXmlElement *xmlNode, *xmlPosition, *xmlRotation, *xmlScale,  *xmlEntity, *xmlBillboardSet,  *xmlLight, *xmlUserData;
        xmlNode = xmlPosition = xmlRotation = xmlEntity = xmlBillboardSet = xmlLight = xmlUserData = 0;

        xmlNode = xmlNodes->FirstChildElement("node");

        while(xmlNode)
        {
            String nodeName = xmlNode->Attribute("name");
            SceneNode* newNode = sceneManager->getRootSceneNode()->createChildSceneNode(nodeName);

            xmlPosition = xmlNode->FirstChildElement("position");

            if(xmlPosition)
            {
                double x, y, z;
                xmlPosition->Attribute("x", &x);
                xmlPosition->Attribute("y", &y);
                xmlPosition->Attribute("z", &z);
                newNode->setPosition(x, y, z);
            }

            xmlRotation = xmlNode->FirstChildElement("rotation");

            if(xmlRotation)
            {
                double x, y, z, w;
                xmlRotation->Attribute("qx", &x);
                xmlRotation->Attribute("qy", &y);
                xmlRotation->Attribute("qz", &z);
                xmlRotation->Attribute("qw", &w);
                newNode->setOrientation(w, x, y, z);
            }

            xmlScale = xmlNode->FirstChildElement("scale");

            if(xmlScale)
            {
                double x, y, z;
                xmlScale->Attribute("x", &x);
                xmlScale->Attribute("y", &y);
                xmlScale->Attribute("z", &z);
                newNode->setScale(x, y, z);
            }

            xmlLight = xmlNode->FirstChildElement("light");

            if(xmlLight)
                newNode->attachObject(loadLight(xmlLight, sceneManager));

            xmlEntity = xmlNode->FirstChildElement("entity");

            if(xmlEntity)
            {
                String entityName, entityMeshFilename;
                entityName = xmlEntity->Attribute("name");
                entityMeshFilename = xmlEntity->Attribute("meshFile");

                Entity* newEntity = sceneManager->createEntity(entityName, entityMeshFilename);
                newEntity->setCastShadows(true);

                if(xmlEntity->Attribute("materialFile"))
                {
                    newEntity->setMaterial(
                                Ogre::MaterialPtr(
                                    MaterialManager::getSingletonPtr()->getByName(xmlEntity->Attribute("materialFile"))
                                )
                    );
                }

                if(xmlEntity->Attribute("castShadows"))
                {
                    int shadowBool;
                    xmlEntity->Attribute("castShadows", &shadowBool);
                    newEntity->setCastShadows(shadowBool);
                }

                newNode->attachObject(newEntity);
            }

            xmlBillboardSet = xmlNode->FirstChildElement("billboardSet");

            if(xmlBillboardSet)
            {
                BillboardSet* billboardSet = sceneManager->createBillboardSet(newNode->getName());
                BillboardType type;
                const char* valueString = xmlBillboardSet->Attribute("type");

                if(strcmp(valueString, "orientedCommon") == 0)
                    type = BBT_ORIENTED_COMMON;
                else if(strcmp(valueString, "orientedSelf") == 0)
                    type = BBT_ORIENTED_SELF;
                else
                    type = BBT_POINT;

                BillboardOrigin origin;
                valueString = xmlBillboardSet->Attribute("origin"); // SHOULD BE type?

                if(strcmp(valueString, "bottom_left") == 0)
                    origin = BBO_BOTTOM_LEFT;
                else if(strcmp(valueString, "bottom_center") == 0)
                    origin = BBO_BOTTOM_CENTER;
                if(strcmp(valueString, "bottom_right") == 0)
                    origin = BBO_BOTTOM_RIGHT;
                if(strcmp(valueString, "left") == 0)
                    origin = BBO_CENTER_LEFT;
                else if(strcmp(valueString, "right") == 0)
                    origin = BBO_CENTER_RIGHT;
                else if(strcmp(valueString, "topLeft") == 0)
                    origin = BBO_TOP_LEFT;
                if(strcmp(valueString, "topRight") == 0)
                    origin = BBO_TOP_RIGHT;
                else if(strcmp(valueString, "topCenter") == 0)
                    origin = BBO_TOP_CENTER;
                else
                    origin = BBO_CENTER;

                billboardSet->setBillboardType(type);
                billboardSet->setBillboardOrigin(origin);

                valueString = xmlBillboardSet->Attribute("name");
                billboardSet->setMaterialName(valueString);

                int width, height;
                xmlBillboardSet->Attribute("width", &width);
                xmlBillboardSet->Attribute("height", &height);
                billboardSet->setDefaultDimensions(width, height);
                billboardSet->setVisible(true);
                newNode->attachObject(billboardSet);

                TiXmlElement* xmlBillboard;
                xmlBillboard = xmlBillboardSet->FirstChildElement("billboard");

                while(xmlBillboard)
                {
                    Billboard* billboard;
                    double red, green, blue, x, y, z;
                    red = green = blue = x = y = z = 0;

                    xmlPosition = xmlBillboard->FirstChildElement("position");

                    if(xmlPosition)
                    {
                        xmlPosition->Attribute("x", &x);
                        xmlPosition->Attribute("y", &y);
                        xmlPosition->Attribute("z", &z);
                    }

                    TiXmlElement* xmlColor = xmlBillboard->FirstChildElement("colourDiffuse");

                    if(xmlColor)
                    {
                        xmlColor->Attribute("r", &red);
                        xmlColor->Attribute("g", &green);
                        xmlColor->Attribute("b", &blue);
                    }

                    billboard = billboardSet->createBillboard(x, y, z, ColourValue(red, green, blue, 1));
                    xmlBillboard = xmlBillboard->NextSiblingElement("billboard");
                }
            }

            xmlUserData = xmlNode->FirstChildElement("userData");

            if(xmlUserData)
            {
                TiXmlElement* xmlProperty;
                xmlProperty = xmlUserData->FirstChildElement("property");

                while(xmlProperty)
                {
                    const char* nodeName = newNode->getName().c_str();
                    const char* propertyName = xmlProperty->Attribute("name");
                    const char* data = xmlProperty->Attribute("data");
                    NodeProperty newProperty(nodeName, propertyName, data);
                    nodeProperties.push_back(newProperty);

                    xmlProperty = xmlProperty->NextSiblingElement("property");
                }
            }

            xmlNode = xmlNode->NextSiblingElement("node");
        }
    }

    xmlEnvir = xmlRoot->FirstChildElement("environment");

    if(xmlEnvir)
    {
        std::cout << "ENVIRONMENT!!!!!!!!!! ENVIRONMENT ENVIRONMENT ENVIRONMENT !!!!!!" << std::endl;
        TiXmlElement* xmlAmbient = xmlEnvir->FirstChildElement("colourAmbient");

        if(xmlAmbient)
        {
            double red, green, blue;
            xmlAmbient->Attribute("r", &red);
            xmlAmbient->Attribute("g", &green);
            xmlAmbient->Attribute("b", &blue);
            ColourValue ambient(red, green, blue);

            sceneManager->setAmbientLight(ambient);

            std::cout << "Ambient!!!!!!!!!! AMBIENT AMBIENT AMBIENT !!!!!!" << std::endl;
        }
    }


    actors = physics::loadMap(xmlDoc);

    for(int i = 0; i < actors.size(); ++i)
    {
        actors[i]->bindOgreNode(sceneManager);
    }

    if(xmlDoc)
        delete xmlDoc;

    return actors;
}

bool getProperty(std::string nodeName, std::string propertyName, std::string& value)
{
    for(int i = 0; i < nodeProperties.size(); ++i)
    {
        if(nodeProperties[i].nodeName.compare(nodeName) == 0 && nodeProperties[i].propertyName.compare(propertyName) == 0)
        {
            value =  nodeProperties[i].valueName;
            return true;
        }
    }

    return false;
}

} // dotscene namespace
