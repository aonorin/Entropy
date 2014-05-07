#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <boost/unordered/unordered_map.hpp>
#include "actor.h"
#include "src/gameplay/abilities/ability.h"

namespace neuro
{

class Player : public Actor
{
public:
    typedef boost::shared_ptr<Player> Ptr;
    Player(Square pos);

    void init_manual();

    void update_turn(double turn_number);
    void continuous_update();

    virtual void update_action() {}
    virtual void complete_action() {}

    void move();

    void findPath(Square targetPos);

    void addAbility(Ability::Ptr ability);
    void removeAbility(const std::string& abilityName);
    void setActiveAbility(const std::string& abilityName);
    void useAbility(Tile::Ptr tile);

private:
    double update_tick;
    int current_path_num;
    double walk_mu;
    bool ready_to_move;

    std::string abilityName;
    Ability::Ptr activeAbility;
    boost::unordered::unordered_map<std::string, Ability::Ptr> abilityMap;
};

}

#endif // PLAYER_H
