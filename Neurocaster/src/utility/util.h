#ifndef UTIL_H
#define UTIL_H

#include "gameplay/square.h"
#include "OGRE/OgreMatrix4.h"

namespace neuro
{

namespace util
{

void seedRandomness(); // generates a new random seed
double rand_double(double min = 0, double max = 1);
int rand_int(int min, int max);

Ogre::Vector2 linear_interpolate_pos(Ogre::Vector2 pos1, Ogre::Vector2 pos2, double mu);


extern Ogre::Matrix4 camera_projection;
Ogre::Matrix4 moveCamera(float zoom, float pan_x = 0, float pan_y = 0);
Ogre::Vector3 unproject(Ogre::Vector3 pos,Ogre::Matrix4 proj);

Square mouseToTile(int x, int y);

} // util namespace

} // neuro namespace

#endif // UTIL_H
