#include "grid.h"
#include "core/neurocaster.h"

using namespace neuro;

Grid::Grid() :
    NeuroManualObject(Square(0,0),"Grid")
{
}

void Grid::init_manual()
{
    manual = Neurocaster::getSingletonPtr()->mSceneMgr->createManualObject(name);
//    manual->setUseIdentityProjection(true);
//    manual->setUseIdentityView(true);

    manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);

    double width = Neurocaster::getSingletonPtr()->mCamera->getViewport()->getActualWidth();
    double height = Neurocaster::getSingletonPtr()->mCamera->getViewport()->getActualHeight();
    double aspectRatio = ((double)height)/((double)width);

    std::cout << "ASPECT RATIO: " << aspectRatio << std::endl;

    //Draw Lines on Y Axis
    for(int x=0;x<Neurocaster::getSingletonPtr()->current_map_x;++x)
    {
        float xx = ((float)x-0.5)/Neurocaster::getSingletonPtr()->current_map_x;
        xx = (xx * 2) - 1;
        xx *= aspectRatio;
        manual->position(xx, -1, 0);
        manual->textureCoord(0,1);
        manual->colour(0.9,0.9,1);

        manual->position(xx, 1, 0);
        manual->textureCoord(0,1);
        manual->colour(0.8,0.9,1);
    }

    //Draw Lines on X Axis
    for(int y=0;y<Neurocaster::getSingletonPtr()->current_map_y;++y)
    {
        float yy = ((float)y-0.5)/Neurocaster::getSingletonPtr()->current_map_y;
        yy = (yy * 2) - 1;
        manual->position(-1,yy, 0);
        manual->textureCoord(0,1);
        manual->colour(0.8,0.9,1);

        manual->position(1,yy, 0);
        manual->textureCoord(0,1);
        manual->colour(0.9,0.9,1);
    }

    manual->end();

    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    manual->setBoundingBox(aabInf);

    manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_1);

    scene_node = Neurocaster::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    scene_node->attachObject(manual);
//    std::cout << pos.convert().x << "," << pos.convert().y << std::endl;
    scene_node->setPosition(0,0,0);
}
