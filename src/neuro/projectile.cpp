#include "projectile.h"
#include "boost/lexical_cast.hpp"
#include "src/Core/terminal.h"
#include "src/Core/Entropy.h"
#include <OGRE/Ogre.h>

namespace neuro {

Projectile::Projectile(Vec2f pos, int type, Direction direction, int pid) :
    pos(pos),
    direction(direction),
    pid(pid)
{
    Ogre::MaterialPtr proMat = Ogre::MaterialManager::getSingleton().create("Projectile", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    proMat->getTechnique(0)->getPass(0)->createTextureUnitState("NeuroResources/ProjectileSprites.png");
    proMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    proMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    proMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    proMat.getPointer()->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    projectileIcon = static_cast<Ogre::OverlayContainer*>(Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "projectile"+boost::lexical_cast<std::string>(this->pid)));
    projectileIcon->setMetricsMode(Ogre::GMM_PIXELS);
    projectileIcon->setPosition(pos.x*Terminal::getSingletonPtr()->pixelSize, pos.y*Terminal::getSingletonPtr()->pixelSize);
    projectileIcon->setWidth(Terminal::getSingletonPtr()->pixelSize*0.25);
    projectileIcon->setHeight(Terminal::getSingletonPtr()->pixelSize*0.25);
    projectileIcon->setParameter("uv_coords","0 0 0.25 0.25");
    projectileIcon->setMaterialName("Projectile");
    Terminal::getSingletonPtr()->_panel3->addChild(projectileIcon);

    speed = 0.1;
}

Projectile::~Projectile()
{
    Terminal::getSingletonPtr()->_panel3->removeChild("projectile"+boost::lexical_cast<std::string>(this->pid));
    Entropy::getSingletonPtr()->addQeueudDeleteOverlays(projectileIcon);
}

void Projectile::move()
{
    switch(direction) {
    case UP:
        pos.y+=speed;
        break;
    case DOWN:
        pos.y-=speed;
        break;
    case LEFT:
        pos.x-=speed;
        break;
    case RIGHT:
        pos.x+=speed;
        break;
    }
    if(checkMapBounds(pos)) {
        try {
            projectileIcon->setPosition(pos.x*Terminal::getSingletonPtr()->pixelSize, pos.y*Terminal::getSingletonPtr()->pixelSize);
        } catch(...) {
//            std::cout << "Projectile Error" << std::endl;
        }
    }
}

int Projectile::getID()
{
    return pid;
}

Vec2f Projectile::getPos()
{
    return pos;
}

}
