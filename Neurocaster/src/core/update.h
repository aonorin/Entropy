#ifndef UPDATE_H
#define UPDATE_H

#ifndef WIN32
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

#include "boost/thread/thread.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"

#include "boost/timer.hpp"
#include "boost/chrono.hpp"
#include "boost/chrono/time_point.hpp"
#include "OGRE/OgreSingleton.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/locks.hpp"

/**
 * @brief The Update class
 * This is the main update loop in Neurocaster.
 */
class Update : public Ogre::Singleton<Update>
{
public:
    Update();
    ~Update();

    /**
     * @brief start Starts the update loop.
     */
    void start();

    /**
     * @brief stop Stops the update loop.
     */
    void stop();

    /**
    void endAndJoin();

    /**
     * @brief getSingleton Returns a reference to the singleton Update loop.
     * @return
     */
    static Update& getSingleton(void);

    /**
     * @brief getSingletonPtr Returns a pointer to the singleton Update loop.
     * @return
     */
    static Update* getSingletonPtr(void);

    /**
     * @brief wait_for_player This is called when the player needs to make a choice. Pauses the step updates (but keeps updating continuous updates, such as animations).
     */
    void wait_for_player();

    /**
     * @brief resume_from_player Resumes the step updates after the player has made a choice.
     */
    void resume_from_player();

    /**
     * @brief pause Pauses the update loop.
     */
    void pause();

    /**
     * @brief unpause Unpauses the update loop.
     */
    void unpause();

private:

    /**
     * @brief paused Whether ot not the Update loop is paused.
     */
    bool paused;

    /**
     * @brief run This is the run function which is called by the boost thread that is spawned when start is called. The heart of the update loop.
     */
    void run();

    /**
     * @brief running Whether or not the Update loop is running.
     */
    bool running;

    /**
     * @brief thread The boost thread spwaned when calling start on Update. Where the update loop resides.
     */
    boost::shared_ptr<boost::thread> thread;

    /**
     * @brief lastUpdateTime The last point in time the Update loop updated.
     */
    boost::chrono::steady_clock::time_point lastUpdateTime;

    /**
     * @brief updateInterval The interval at which the update loop updates. Keeps things regimented (may need to be synchronized with the FPS).
     */
    boost::chrono::milliseconds updateInterval;

    /**
     * @brief turnInterval The interval at which turns are updated.
     */
    boost::chrono::milliseconds turnInterval;

    /**
     * @brief sleepInterval The period of time for which the update loop sleeps.
     */
    boost::posix_time::microseconds sleepInterval;

    /**
     * @brief clock A steady timer for the game.
     */
    boost::chrono::steady_clock clock;

    /**
     * @brief waiting_for_player_decision Whether or not the step updates are waiting on the player to make a decision.
     */
    bool waiting_for_player_decision;

    /**
     * @brief turn_number The turn number since the start of the map.
     */
    double turn_number;

    double round_cycle;

    double turn_cycle;

    /**
     * @brief last_turn_time The last point in time a turn was updated.
     */
    boost::chrono::steady_clock::time_point last_turn_time;

    /**
     * @brief pause_time Keeps track of the amount of time the set updates have been waiting for the player to make a decision.
     */
    boost::chrono::steady_clock::time_point pause_time;
};

#endif // UPDATE_H
