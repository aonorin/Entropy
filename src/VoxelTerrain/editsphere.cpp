#include "editsphere.h"
#include "map3.h"

using namespace voxel;

EditSphere::EditSphere(Ogre::Sphere sp, bool cut)
    : Edit(cut)
    , m_sp(sp)
{

}

Ogre::AxisAlignedBox EditSphere::aab(double scale) const
{
    Ogre::Sphere sp = scaleSphere(m_sp, scale);
    Ogre::AxisAlignedBox aabb(sp.getCenter().x-((int)sp.getRadius()+1), sp.getCenter().y-((int)sp.getRadius()+1), sp.getCenter().z-((int)sp.getRadius()+1),
                        sp.getCenter().x+((int)sp.getRadius()+1), sp.getCenter().y+((int)sp.getRadius()+1), sp.getCenter().z+((int)sp.getRadius()+1));
    return aabb;
}

double EditSphere::calcVoxel(const Ogre::Vector3& pos, const double &scale)
{
    Ogre::Sphere sp = scaleSphere(m_sp, scale);
    double dist = sp.getRadius() - sp.getCenter().distance(pos);
    return dist;
}


Ogre::Sphere EditSphere::scaleSphere(const Ogre::Sphere _sp, const double scale)
{
    Ogre::Sphere sp = _sp;
    sp.setCenter(sp.getCenter()/scale);
    sp.setRadius(sp.getRadius()/scale);
    return sp;
}
