#ifndef NEURON_H
#define NEURON_H

#include <OGRE/Ogre.h>
#include "OGRE/OgreRectangle2D.h"
#include "square.h"

namespace neuro
{

/**
 * @brief The NeuroManualObject class
 * This represents a manual (graphical) object in Neurocaster.
 * Contains an Ogre::ManualObject and an Ogre::SceneNode.
 */
class NeuroManualObject
{
public:
//#ifdef __APPLE__
//    typedef std::tr1::shared_ptr<NeuroManualObject> Ptr;
//#else
    typedef boost::shared_ptr<NeuroManualObject> Ptr;
//#endif
    typedef std::vector<Ptr> PtrArray;

    /**
     * @brief NeuroManualObject This represents a manual (graphical) object in Neurocaster.
     * @param pos The position of the NeuroManual in squares.
     * @param name The name of the NeuroManual object.
     */
    NeuroManualObject(Square pos,std::string name);
    ~NeuroManualObject();

    /**
     * @brief init_as_rect This is a convenience method, when used it initializes the NeuroManualObject as a rect with a texture.
     * @param texture_name The name of the texture for the NeuroManualObject, if none is given, it is assumed to be white.
     * @param left The coordinate of the left side, ranges from -1 to 1.
     * @param top The coordinate of the top side, ranges from -1 to 1.
     * @param right The coordinate of the right side, ranges from -1 to 1.
     * @param bottom The coordinate of the bottom side, ranges from -1 to 1.
     */
    void init_as_rect(std::string texture_name, float left = -1.0, float top = 1.0, float right = 1.0, float bottom = -1.0);

    /**
     * @brief init_manual Initializes the manual object. Should be implemented by inheriting classes.
     */
    virtual void init_manual() = 0;
    void async_init_manual();

    /**
     * @brief getPos Get this position of this object in squares.
     * @return
     */
    Square getPos();

    /**
     * @brief setPos Set the position of this object in squares.
     * @param pos
     */
    void setPos(Square pos);

    void asyncSetPos();

    void manualSetPos(Square pos);

    void setVisible();
    void setInvisible();

protected:
    void asyncSetVisible();
    void asyncSetInvisible();

    /**
     * @brief pos The position of this object in squares.
     */
    Square pos;

    /**
     * @brief name The name of the NeuroManualObject
     */
    std::string name;

    /**
     * @brief scene_node The scene node this manual object is attached to.
     */
    Ogre::SceneNode* scene_node;

    /**
     * @brief manual The Ogre::ManualObject this NeuroManualObject contains.
     */
    Ogre::ManualObject* manual;
};

}

#endif // NEURON_H
