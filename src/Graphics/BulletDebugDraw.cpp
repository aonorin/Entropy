// Entropy includes
#include "BulletDebugDraw.h"
#include "src/Graphics/OgreDebugDraw/DebugDrawer.h"

namespace graphics {

BulletDebugDraw::BulletDebugDraw() :
    btIDebugDraw(),
    enabled(false)
{

}

void BulletDebugDraw::drawLine (const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
    DebugDrawer::getSingleton().drawLine(
                Ogre::Vector3(from.x(), from.y(), from.z()),
                Ogre::Vector3(to.x(), to.y(), to.z()),
                Ogre::ColourValue(color.x(), color.y(), color.z(), 0.75)
    );
}

void BulletDebugDraw::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
                       const btVector3 &color)
{
    DebugDrawer::getSingleton().drawSphere(
                Ogre::Vector3(PointOnB.x(), PointOnB.y(), PointOnB.z()),
                distance,
                Ogre::ColourValue(lifeTime, lifeTime, lifeTime)
    );
}

void BulletDebugDraw::reportErrorWarning(const char* warningString)
{
    std::cout << warningString << std::endl;
}

void BulletDebugDraw::draw3dText(const btVector3 &location, const char* textString)
{

}

void BulletDebugDraw::setDebugMode(int debugMode)
{
    this->debugMode = debugMode;
}

int BulletDebugDraw::getDebugMode() const
{
    return debugMode;
}

} // graphics namespace
