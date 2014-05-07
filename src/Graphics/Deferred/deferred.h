#ifndef DEFERRED_H
#define DEFERRED_H

namespace Ogre {

    class Viewport;
    class SceneManager;
    class Camera;

} // Ogre forward declaration

namespace graphics {

namespace deferred {

void initialize(Ogre::Viewport* vp, Ogre::SceneManager* sm,  Ogre::Camera* cam); // MUST BE CALLED ON START
void cleanup(); // MUST BE CALLED ON FINISH

} // deferred namespace

} // graphics namespace

#endif // DEFERRED_H
