#include "player.h"
#include "core/neurocaster.h"
#include "gameplay/astar.h"
#include "utility/util.h"

using namespace neuro;

Player::Player(Square pos) :
    Actor(pos,"Player",Tile::HERO,1),
    update_tick(0),
    current_path_num(0),
    walk_mu(0),
    ready_to_move(false)
{

}

void Player::update_turn(double turn_number)
{
    //    current_path_num = turn_number - 1;
    if(ready_to_move)
    {
        update_tick += 20./500.;
        current_path_num = update_tick;
        double dummy;
        walk_mu = modf(update_tick,&dummy);
        //        std::cout << "Player::update_turn: " << update_tick << std::endl;
        move();
    }
}

void Player::continuous_update()
{

}

void Player::init_manual()
{
    manual = Neurocaster::getSingletonPtr()->mSceneMgr->createManualObject(name);
    manual->setUseIdentityView(true);

    manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

    double left = -0.25;
    double top = 0.25;
    double right = 0.25;
    double bottom = -0.25;

    float r = 1;
    float g = 1;
    float b = 1;


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

    manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_2);

    scene_node = Neurocaster::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    scene_node->attachObject(manual);
    scene_node->setPosition(pos.convert());
}

void Player::findPath(Square targetPos)
{
    ready_to_move = false;
    std::cout << "PLAYER findPath: [" << targetPos.getX() << "," << targetPos.getY() << "]" << std::endl;
    path.clear();
    if(astar::pathFind(Neurocaster::getSingletonPtr()->getCurrentMap(),pos.rounded().toOgre(),targetPos.toOgre(),Tile::HERO,&path,astar::DiagonalHate,false,true))
    {
        update_tick = 0;
        current_path_num = 0;
        //        walk_mu = 0;
        ready_to_move = true;
    }
}

void Player::addAbility(Ability::Ptr ability)
{
    abilityMap[ability->getName()] = ability;
}

void Player::removeAbility(const std::string& abilityName)
{
    abilityMap.erase(abilityName);
}

void Player::setActiveAbility(const std::string& abilityName)
{
    boost::unordered::unordered_map<std::string, Ability::Ptr>::iterator find = abilityMap.find(abilityName);

    if(find != abilityMap.end())
        activeAbility = find->second;

    std::cout << "Player::setActiveAbility: " << abilityName << std::endl;
}

void Player::useAbility(Tile::Ptr tile)
{
    //    if(abilityName.empty())
    //    {
    //        findPath(tile);
    //    }
    switch(tile->getChar())
    {
    case Tile::PORT:
        std::cout << "PORT" << std::endl;
        findPath(tile->getPos());
        break;
    case Tile::ENEMYPORT:
        std::cout << "ENEMYPORT" << std::endl;
        findPath(tile->getPos());
        break;
    case Tile::NODE:
        std::cout << "NODE" << std::endl;
        findPath(tile->getPos());
        break;
    case Tile::ENEMYNODE:
        std::cout << "ENEMYNODE" << std::endl;
        findPath(tile->getPos());
        break;
    default:
        std::cout << "HALLWAY" << std::endl;
        findPath(tile->getPos());
    }
}

void Player::move()
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
    else
    {
        ready_to_move = false;
    }
}
