//#ifndef UPDATE_H
//#define UPDATE_H
#pragma once

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
#include "Script/scripttypes.h"
#include "gameplay/EventAction.h"

class TimedEvent;
class Entropy;

class Update : public Ogre::Singleton<Update>
{
public:
    Update(Entropy* entropy);
    ~Update();
    void start();
    void stop();
    void testFunc();    
    static Update& getSingleton(void);
    static Update* getSingletonPtr(void);
    std::tr1::unordered_map<int,TimedEvent* > timedEvents;
    int registerEvent(boost::posix_time::time_duration time,boost::function<void (void)> voidFunction);
    int registerEvent(boost::posix_time::time_duration time,boost::function<void (int)> intFunction,int num);
    int registerEvent(boost::posix_time::time_duration time, boost::function<void (void)> voidFunction,boost::function<void (float)> progressFunction);
    int registerEvent(boost::posix_time::time_duration time,boost::function<void (int)> intFunction,boost::function<void (float)> floatFunction,int num);
    int registerEvent(boost::posix_time::time_duration time, script::lua_functor_t luaFunctor);
    int registerEvent(boost::posix_time::time_duration time, script::lua_functor_t luaFunctor,script::lua_functor_t progressFunction);
    int registerEvent(boost::posix_time::time_duration time, gameplay::EventAction eventAction);

    void removeEvent(int eid);

    void pause();
    void unpause();

private:
    bool paused;
    void run();
    void processEvents();
    int timeOut;
    bool running;
    std::vector<int> executedIDs;
    void removeEvent();
    Entropy* entropy;
    boost::shared_ptr<boost::thread> thread;
    boost::mutex eventMutex;
    boost::chrono::steady_clock::time_point lastUpdateTime, lastPhysicsTime;
    boost::chrono::milliseconds updateInterval, physicsInterval;
    boost::posix_time::microseconds sleepInterval;
    boost::chrono::steady_clock clock;
};

namespace script {

int registerLuaTimedEvent(int milliseconds, script::lua_functor_t luaFunctor);

int registerLuaProgressTimedEvent(int milliseconds, script::lua_functor_t luaFunctor,script::lua_functor_t progressFunction);

int registerLuaTimedEventAction(int milliseconds, gameplay::EventAction eventAction);

}
//#endif // UPDATE_H
