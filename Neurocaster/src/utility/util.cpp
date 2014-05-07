#include <cmath>
#include <ctime>
#include <cstdlib>

#include "util.h"
#include "core/neurocaster.h"

namespace neuro
{

namespace util
{

Ogre::Matrix4 camera_projection;

void seedRandomness()
{
    srand(time(NULL));
}

double rand_double(double min, double max)
{
    return (max - min) * ((double) rand() / (double) RAND_MAX) + min;
}

int rand_int(int min, int max)
{
    return (rand() % (max - min)) + min;
}

Ogre::Matrix4 BuildScaledOrthoMatrix(float left, float right, float bottom, float top, float near, float far)
{
    float invw = 1 / (right - left);
    float invh = 1 / (top - bottom);
    float invd = 1 / (far - near);

    Ogre::Matrix4 proj = Ogre::Matrix4::ZERO;
    proj[0][0] = 2 * invw;
    proj[0][3] = -(right + left) * invw;
    proj[1][1] = 2 * invh;
    proj[1][3] = -(top + bottom) * invh;
    proj[2][2] = -2 * invd;
    proj[2][3] = -(far + near) * invd;
    proj[3][3] = 1;
    return proj;
}

Ogre::Matrix4 moveCamera(float zoom, float pan_x, float pan_y)
{
    double width = Neurocaster::getSingletonPtr()->mCamera->getViewport()->getActualWidth();
    double height = Neurocaster::getSingletonPtr()->mCamera->getViewport()->getActualHeight();
    camera_projection = BuildScaledOrthoMatrix(
                1  / zoom / -2.0f + pan_x,
                1  / zoom /  2.0f + pan_x,
                1 / zoom / -2.0f + pan_y,
                1 / zoom /  2.0f + pan_y,
                0,
                1000
                );
    Neurocaster::getSingletonPtr()->mCamera->setCustomProjectionMatrix(true,camera_projection);
}

Ogre::Vector3 unproject(Ogre::Vector3 pos, Ogre::Matrix4 proj)
{
    return Neurocaster::getSingletonPtr()->mCamera->getProjectionMatrix().inverse() * pos;
}

Ogre::Vector2 linear_interpolate_pos(Ogre::Vector2 pos1, Ogre::Vector2 pos2, double mu)
{
    return Ogre::Vector2(
                pos1.x*(1-mu) + pos2.x*mu,
                pos1.y*(1-mu) + pos2.y*mu
                );
}

Square mouseToTile(int x, int y)
{
    Ogre::Vector3 pos(x,y,1);

    double width = Neurocaster::getSingletonPtr()->mCamera->getViewport()->getActualWidth();
    double height = Neurocaster::getSingletonPtr()->mCamera->getViewport()->getActualHeight();

    pos.x = pos.x / width;
    pos.y = pos.y / height;

    Ogre::Ray mouseRay = Neurocaster::getSingletonPtr()->mCamera->getCameraToViewportRay(pos.x, pos.y);

    Ogre::Vector3 zeroZero = Square(0,0).convert();
    Ogre::Vector3 oneOne = Square(59,59).convert();

    Ogre::AxisAlignedBox aabb(-10,-10,0,10,10,0);
    double distance = mouseRay.intersects(aabb).second;

    pos = mouseRay.getPoint( distance );

    pos.x = (pos.x - zeroZero.x) / (oneOne.x - zeroZero.x);
    pos.y = (pos.y - zeroZero.y) / (oneOne.y - zeroZero.y);

    Square s = Square::inverseConvert(pos);
    s.x = (int) round(s.x + 1);
    s.y = (int) round(s.y);

    return s;
}

} // util namespace

} // neuro namespace

