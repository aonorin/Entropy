#include "tilehilight.h"
#include "core/Global.h"
#include "core/neurocaster.h"

using namespace neuro;

void TileHilight::init_manual()
{

    manual = Neurocaster::getSingletonPtr()->mSceneMgr->createManualObject(name);

    manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

    double left = -0.5;
    double top = 0.5;
    double right = 0.5;
    double bottom = -0.5;

    float r = 1;
    float g = 1;
    float b = 1;

    double w = 0.05;

    //LEFT-SIDE
    manual->position(Square(left-w,top).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(left-w,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(left+w,top).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(left-w,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(left+w,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(left+w,top).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    //RIGHT-SIDE
    manual->position(Square(right-w,top).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(right-w,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(right+w,top).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(right-w,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(right+w,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(right+w,top).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    //TOP-SIDE
    manual->position(Square(left,top+w).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(left,top-w).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(right,top+w).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(left,top-w).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(right,top-w).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(right,top+w).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    //BOTTOM-SIDE
    manual->position(Square(left,bottom+w).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(left,bottom-w).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(right,bottom+w).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(left,bottom-w).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(right,bottom-w).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(right,bottom+w).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->end();

    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    manual->setBoundingBox(aabInf);

    manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);

    scene_node = Neurocaster::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    scene_node->attachObject(manual);
    scene_node->setPosition(pos.convert());

    setVisible();
}

TileHilight::TileHilight()
    : NeuroManualObject(Square(1,1),get_unique_name("TileHilight"))
{

}
