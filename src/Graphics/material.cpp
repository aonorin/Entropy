#include "material.h"
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreTechnique.h>
#include <OGRE/OgrePass.h>

using namespace graphics;

unsigned int Material::g_idCounter = 0;

Material::Material()
{
}

Material::Material(std::string loadFromFile)
{
    m_material = Ogre::MaterialManager::getSingletonPtr()->getByName(loadFromFile);
}

Material::Material(const Material& mat)
{
    m_material = mat._material()->clone(Ogre::String("material__material").append(Ogre::StringConverter::toString((int) g_idCounter++)));
}

Ogre::MaterialPtr Material::_material() const
{
    return m_material;
}

void Material::ambient(const Ogre::ColourValue &col)
{
    m_material->getTechnique(0)->getPass(0)->setAmbient(col);
}
