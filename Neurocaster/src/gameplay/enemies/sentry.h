#ifndef SENTRY_H
#define SENTRY_H

#include "gameplay/neuroai.h"

namespace neuro
{

class Sentry : public NeuroAI
{
public:

    typedef boost::shared_ptr<Sentry> Ptr;

    Sentry(Square pos);

    void update_turn(double turn_number);
    void continuous_update();

    void update_action() {}
    void complete_action() {}

    void init_manual();

    void move();

    void find_new_sub_system();

    void findPath(Square targetPos);

    void init_ai();

    void update_ai();

protected:
    int current_ss_x;
    int current_ss_y;
    int current_path_num;
    double walk_mu;
};

}

#endif // SENTRY_H
