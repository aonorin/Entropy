#ifndef MATERIAL_H
#define MATERIAL_H

#include <OGRE/OgreMaterial.h>

namespace graphics {

class Material
{
public:
    Material();
    Material(std::string loadFromFile);
    Material(const Material &mat);

    void ambient(const Ogre::ColourValue &col);
//protected:
    friend class manualObject;
    Ogre::MaterialPtr _material() const;
private:
    Ogre::MaterialPtr m_material;

    static unsigned int g_idCounter; // for coping materials
};


}

#endif // MATERIAL_H
