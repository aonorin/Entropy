#ifndef MESHFUNCTIONS_H
#define MESHFUNCTIONS_H

#include <string>
#include <vector>
#include "OGRE/OgreVector3.h"
#include "OGRE/OgreVector2.h"
#include "algorithm.h"

namespace Degenerator {

extern float cubeVertices[24];
extern int cubeIndices[36];

double stringToDouble(std::string& string);
void cube(Ogre::Vector3 position, double size, AStruct& astruct);
Ogre::Vector3 calcNormal(Ogre::Vector3 v0, Ogre::Vector3 v1, Ogre::Vector3 v2);

} // Degenerator namespace

#endif // MESHFUNCTIONS_H
