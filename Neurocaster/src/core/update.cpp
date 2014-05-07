#include "update.h"
#include <boost/foreach.hpp>
#include "neurocaster.h"
#include "boost/chrono.hpp"
#include "boost/chrono/duration.hpp"
#include "boost/asio/basic_waitable_timer.hpp"

Update::Update() :
    paused(false),
    sleepInterval(boost::posix_time::microseconds(4000)),
    updateInterval(boost::chrono::milliseconds(20)),
    turnInterval(boost::chrono::milliseconds(500)),
    turn_number(1),
    waiting_for_player_decision(true)
{
    start();
}

Update::~Update()
{
    stop();
}

void Update::run()
{
    lastUpdateTime = clock.now();
    last_turn_time = clock.now();
    while(running)
    {

        boost::chrono::steady_clock::time_point time = clock.now();

        if(time - lastUpdateTime >= updateInterval)
        {
            lastUpdateTime = time;
            //TODO: call continuous update here
            Neurocaster::getSingletonPtr()->update();

            if(!waiting_for_player_decision)
            {
//                turn_number += (time - last_turn_time) / turnInterval;
                turn_number += 20. / 500.;
                if(turn_number >= 14)
                {
                    Neurocaster::getSingletonPtr()->update_turn(14);
                    turn_number = 1;
//                    waiting_for_player_decision = true;
                    Neurocaster::getSingletonPtr()->new_round();
                }
                else
                {
                    Neurocaster::getSingletonPtr()->update_turn(turn_number);
                }
                //TODO: call turn update here
            }
        }

        boost::this_thread::sleep(sleepInterval);
    }
}

void Update::wait_for_player()
{
    pause_time = clock.now();
}

void Update::resume_from_player()
{
//    last_turn_time += last_turn_time - pause_time;
    waiting_for_player_decision = !waiting_for_player_decision;
//    waiting_for_player_decision = false;
}

void Update::start()
{
    running = true;
    thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Update::run, this)));
}

void Update::stop()
{
    running = false;
    thread->join();
}

void Update::pause(){
    paused = true;
}

void Update::unpause()
{
    paused = false;
}

template<> Update* Ogre::Singleton<Update>::msSingleton = 0;
Update* Update::getSingletonPtr(void)
{
    return msSingleton;
}
Update& Update::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
