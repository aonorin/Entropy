#ifndef BULLETDEBUGDRAW_H
#define BULLETDEBUGDRAW_H

// System includes
#include <vector>

// Bullet includes
#include "LinearMath/btIDebugDraw.h"

// Ogre includes
#include "OGRE/OgreSceneManager.h"

// Entropy includes
#include "src/Utility/Triangle.h"

namespace graphics {

class BulletDebugDraw : public btIDebugDraw
{
public:
    BulletDebugDraw();

    // Inherited methods
    void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
    void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
                           const btVector3 &color);
    void reportErrorWarning(const char* warningString);
    void draw3dText(const btVector3 &location, const char* textString);
    void setDebugMode(int debugMode);
    int getDebugMode() const;

private:

//    void addPoint(const btVector3 &point);

    bool enabled;
    int debugMode;
};

} // graphics namespace

#endif // BULLETDEBUGDRAW_H
