#include "neuron.h"
#include "core/neurocaster.h"

using namespace neuro;

NeuroManualObject::NeuroManualObject(Square pos, std::string name) :
    pos(pos),
    name(name),
    scene_node(0),
    manual(0)
{

}

NeuroManualObject::~NeuroManualObject()
{
    //Clean up scene node and manual object

}

Square NeuroManualObject::getPos()
{
    return pos;
}

void NeuroManualObject::setPos(Square pos)
{
    this->pos = pos;
    boost::function<void ()> func = boost::bind(&NeuroManualObject::asyncSetPos,this);
    Neurocaster::getSingletonPtr()->register_async_func(func);
}

void NeuroManualObject::asyncSetPos()
{
    if(scene_node!=0)
    {
        scene_node->setPosition(pos.convert());
    }
}

void NeuroManualObject::manualSetPos(Square pos)
{
    this->pos = pos;
    if(scene_node!=0)
    {
        scene_node->setPosition(pos.convert());
    }
}

void NeuroManualObject::init_as_rect(std::string texture_name,float left, float top, float right, float bottom)
{
    if(!texture_name.empty())
    {
        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(name,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        material->getTechnique(0)->getPass(0)->createTextureUnitState(texture_name);

        material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
        material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
        material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    }

    manual = Neurocaster::getSingletonPtr()->mSceneMgr->createManualObject(name);
    //    manual->setUseIdentityProjection(true);
    manual->setUseIdentityView(true);

    if(!texture_name.empty())
    {
        manual->begin(name, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
    }
    else
    {
        manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
    }

    manual->position(left, top, 0.0);
    manual->textureCoord(0,1);
    manual->colour(1,1,1);

    manual->position(left, bottom, 0.0);
    manual->textureCoord(0,0);
    manual->colour(1,1,1);

    manual->position(right,  top, 0.0);
    manual->textureCoord(1,1);
    manual->colour(1,1,1);

    manual->position(right, bottom, 0.0);
    manual->textureCoord(1,0);
    manual->colour(1,1,1);

    manual->end();

    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    manual->setBoundingBox(aabInf);

    manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY -1);

    scene_node = Neurocaster::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    scene_node->attachObject(manual);
    std::cout << pos.convert().x << "," << pos.convert().y << std::endl;
    scene_node->setPosition(pos.convert());

}


void NeuroManualObject::async_init_manual()
{
    boost::function<void ()> func = boost::bind(&NeuroManualObject::init_manual,this);
    Neurocaster::getSingletonPtr()->register_async_func(func);
}

void NeuroManualObject::asyncSetVisible()
{
    boost::function<void ()> func = boost::bind(&NeuroManualObject::asyncSetVisible,this);
    Neurocaster::getSingletonPtr()->register_async_func(func);
}

void NeuroManualObject::asyncSetInvisible()
{
    boost::function<void ()> func = boost::bind(&NeuroManualObject::asyncSetInvisible,this);
    Neurocaster::getSingletonPtr()->register_async_func(func);
}

void NeuroManualObject::setVisible()
{
    manual->setVisible(true);
}

void NeuroManualObject::setInvisible()
{
    manual->setVisible(false);
}
