#include <math.h>

#include "sentry.h"
#include "core/neurocaster.h"
#include "core/Global.h"
#include "gameplay/astar.h"
#include "utility/util.h"


//COUNTDOWN TO SHUTDOWN!?

using namespace neuro;

Sentry::Sentry(Square pos) :
    NeuroAI(pos,get_unique_name("Sentry"),Tile::SENTRY,1),
    current_ss_x(0),
    current_ss_y(0),
    current_path_num(0)
{

}

void Sentry::init_manual()
{
    manual = Neurocaster::getSingletonPtr()->mSceneMgr->createManualObject(name);
    manual->setUseIdentityView(true);

    manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

    double left = -0.25;
    double top = 0.25;
    double right = 0.25;
    double bottom = -0.25;

    float r = 1;
    float g = 0.25;
    float b = 0.125;


    manual->position(Square(left,top).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(left,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(right,top).relative_convert());
    manual->textureCoord(1,1);
    manual->colour(r,g,b);

    manual->position(Square(right,bottom).relative_convert());
    manual->textureCoord(1,0);
    manual->colour(r,g,b);

    manual->end();

    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    manual->setBoundingBox(aabInf);

    manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_4);

    scene_node = Neurocaster::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    scene_node->attachObject(manual);
    scene_node->setPosition(pos.convert());
}

double dummy;

void Sentry::update_turn(double turn_number)
{
    current_path_num = turn_number - 1;
    walk_mu = modf(turn_number,&dummy);

    //Check to see if we are at destination
//    Square targetPos = Neurocaster::getSingletonPtr()->getCurrentMap()->getSS(current_ss_x,current_ss_y)->getPos();
//    targetPos.setX(targetPos.x + 1);
//    targetPos.setY(targetPos.y + 2);
//    if( pos.int_equals(targetPos) )
//    {
//        find_new_sub_system();
//        targetPos = Neurocaster::getSingletonPtr()->getCurrentMap()->getSS(current_ss_x,current_ss_y)->getPos();
//        targetPos.setX(targetPos.x + 1);
//        targetPos.setY(targetPos.y + 2);
//        path.clear();
//        findPath(targetPos);
//    }

    move();
}

void Sentry::continuous_update()
{

}

void Sentry::init_ai()
{
    find_new_sub_system();
}

void Sentry::update_ai()
{
    path.clear();

    Square targetPos = Neurocaster::getSingletonPtr()->getCurrentMap()->getSS(current_ss_x,current_ss_y)->getPos();
    targetPos.setX(targetPos.x + 1);
    targetPos.setY(targetPos.y + 2);

//    std::cout << "Current Post: " << pos.getX() <<"," << pos.getY() << std::endl;
//    std::cout << "Target Post: " << targetPos.getX() <<"," << targetPos.getY() << std::endl;

    if( pos.int_equals(targetPos) )
    {
        find_new_sub_system();
        targetPos = Neurocaster::getSingletonPtr()->getCurrentMap()->getSS(current_ss_x,current_ss_y)->getPos();
        targetPos.setX(targetPos.x + 1);
        targetPos.setY(targetPos.y + 2);
    }

    findPath(targetPos);
}

void Sentry::find_new_sub_system()
{
    Map::Ptr map = Neurocaster::getSingletonPtr()->getCurrentMap();

    while(true)
    {
        int x = nrand(0,map->getSSWidth()-1);
        int y = nrand(0,map->getSSHeight()-1);
        if(
                !map->getSS(x,y)->isGone() &&
                !(x==current_ss_x && y==current_ss_y)
                )
        {
            current_ss_x = x;
            current_ss_y = y;
            break;
        }
    }

//    std::cout << "find_new_sub_system: " << current_ss_x << "," <<current_ss_y << std::endl;

    Square targetPos = map->getSS(current_ss_x,current_ss_y)->getPos();
    targetPos.setX(targetPos.x + 1);
    targetPos.setY(targetPos.y + 2);
    findPath(targetPos);
}

void Sentry::move()
{
    if(path.size() > current_path_num + 1)
    {
        Ogre::Vector2 new_pos = util::linear_interpolate_pos(
                    path[current_path_num],
                    path[current_path_num + 1],
                walk_mu
                );

        setPos(Square(new_pos.x,new_pos.y));
    }
}

void Sentry::findPath(Square targetPos)
{
    // std::cout << "findPath: [" << targetPos.getX() << "," << targetPos.getY() << "]" << std::endl;
    astar::pathFind(Neurocaster::getSingletonPtr()->getCurrentMap(),pos.rounded().toOgre(),targetPos.toOgre(),Tile::SENTRY,&path,astar::DiagonalHate,false,true);
}
