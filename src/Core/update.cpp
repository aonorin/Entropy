#include "update.h"
#include <boost/foreach.hpp>
#include "src/Core/timedevent.h"
#include "Entropy.h"
#include "src/Graphics/graphics.h"
#include "src/gameplay/actors.h"
#include "src/Physics/physics.h"
#include "src/Graphics/graphics.h"
#include "src/Graphics/OgreDebugDraw/DebugDrawer.h"
#include "boost/chrono.hpp"
#include "boost/chrono/duration.hpp"
#include "boost/asio/basic_waitable_timer.hpp"
#include "src/gameplay/playerinputaction.h"

Update::Update(Entropy* entropy) :
    timeOut(16),
    entropy(entropy),
    paused(false),
    sleepInterval(boost::posix_time::microseconds(4000)),
    updateInterval(boost::chrono::milliseconds(16)),
    physicsInterval(boost::chrono::milliseconds(4))
{
    lastUpdateTime = clock.now();
    lastPhysicsTime = lastUpdateTime;
    start();
}

Update::~Update()
{
    stop();
}

void Update::run()
{
    while(running)
    {
        //        if(!paused) {
        // processEvents();
        //        }

        boost::chrono::steady_clock::time_point time = clock.now();

        boost::chrono::microseconds timeSinceLastFrame = boost::chrono::duration_cast<boost::chrono::microseconds>(time.time_since_epoch() - lastPhysicsTime.time_since_epoch());
        physics::world.update(((double) timeSinceLastFrame.count()) / (double) 1000000.0);
        lastPhysicsTime = time;

        /*
        if(time - lastPhysicsTime >= physicsInterval)
        {
            boost::chrono::milliseconds timeSinceLastFrame = boost::chrono::duration_cast<boost::chrono::milliseconds>(time.time_since_epoch() - lastPhysicsTime.time_since_epoch());
            physics::world.update(((double) timeSinceLastFrame.count()) / 1000.0);
            lastPhysicsTime = time;
        }
        */

        if(time - lastUpdateTime >= updateInterval)
        {
            // physics::world.update((double) timeSinceLastFrame.time_since_epoch().count / 1000.0);
            gameplay::actors::update();
            physics::updateSensors();
            gameplay::PlayerInputAction::updateActions();
            processEvents();
            lastUpdateTime = time;
        }

        boost::this_thread::sleep(sleepInterval);
        /*
        processEvents();
        gameplay::actors::update();
//        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        boost::chrono::steady_clock::time_point go = boost::chrono::steady_clock::now() + boost::chrono::milliseconds(10);
        while(boost::chrono::steady_clock::now() < go);*/
    }
}

void Update::processEvents()
{
    //    boost::lock_guard<boost::mutex> lock(eventMutex);
    //Find local Time, Update all the Timed events
    boost::posix_time::ptime currentTime = boost::posix_time::microsec_clock::local_time();

    //Delete executed Events
    for(int i=0;i<executedIDs.size();i++) {
        delete timedEvents[executedIDs.at(i)];
        timedEvents.erase(executedIDs.at(i));
    }
    std::pair<int,TimedEvent*> eventPair;
    BOOST_FOREACH(eventPair,timedEvents) {
        if(eventPair.second->update(currentTime) <=0) {
            executedIDs.push_back(eventPair.second->getID());
        }
    }
    //Delete executed Events
    for(int i=0;i<executedIDs.size();i++) {
        delete timedEvents[executedIDs.at(i)];
        timedEvents.erase(executedIDs.at(i));
    }
    entropy->update();
}

void Update::removeEvent(int eid)
{
    if(timedEvents.find(eid) != timedEvents.end()) {
        //        executedIDs.push_back(eid);
    }
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
    try{
        std::pair<int,TimedEvent*> eventPair;
        BOOST_FOREACH(eventPair,timedEvents) {
            delete eventPair.second;
        }
        timedEvents.clear();
    } catch(...) {
        timedEvents.clear();
    }
}

void Update::pause(){
    paused = true;
}

void Update::unpause()
{
    paused = false;
}

void Update::testFunc()
{
    std::cout <<"DONE!" << std::endl;
}

int Update::registerEvent(boost::posix_time::time_duration time, boost::function<void (void)> voidFunction)
{
    boost::lock_guard<boost::mutex> lock(eventMutex);
    TimedEvent* event = new TimedEvent(time, voidFunction);
    event->init();
    timedEvents[event->getID()] = event;
    return event->getID();
}

int Update::registerEvent(boost::posix_time::time_duration time, boost::function<void (int)> intFunction, int num)
{
    boost::lock_guard<boost::mutex> lock(eventMutex);
    TimedEvent* event = new TimedEvent(time, intFunction,num);
    event->init();
    timedEvents[event->getID()] = event;
    return event->getID();
}

int Update::registerEvent(boost::posix_time::time_duration time, boost::function<void (void)> voidFunction,boost::function<void (float)> progressFunction)
{
    boost::lock_guard<boost::mutex> lock(eventMutex);
    TimedEvent* event = new TimedEvent(time, voidFunction,progressFunction);
    event->init();
    timedEvents[event->getID()] = event;
    return event->getID();
}


int Update::registerEvent(boost::posix_time::time_duration time, boost::function<void (int)> intFunction,boost::function<void (float)> progressFunction, int num)
{
    boost::lock_guard<boost::mutex> lock(eventMutex);
    TimedEvent* event = new TimedEvent(time, intFunction,progressFunction,num);
    event->init();
    timedEvents[event->getID()] = event;
    return event->getID();
}

int Update::registerEvent(boost::posix_time::time_duration time, script::lua_functor_t luaFunctor)
{
    boost::lock_guard<boost::mutex> lock(eventMutex);
    TimedEvent* event = new TimedEvent(time, luaFunctor);
    event->init();
    timedEvents[event->getID()] = event;
    return event->getID();
}

int Update::registerEvent(boost::posix_time::time_duration time, gameplay::EventAction eventAction)
{
    boost::lock_guard<boost::mutex> lock(eventMutex);
    TimedEvent* event = new TimedEvent(time, eventAction);
    event->init();
    timedEvents[event->getID()] = event;
    return event->getID();
}

int Update::registerEvent(boost::posix_time::time_duration time, script::lua_functor_t luaFunctor, script::lua_functor_t progressFunction)
{
    boost::lock_guard<boost::mutex> lock(eventMutex);
    TimedEvent* event = new TimedEvent(time, luaFunctor, progressFunction);
    event->init();
    timedEvents[event->getID()] = event;
    return event->getID();
}

namespace script {
    int registerLuaTimedEvent(int milliseconds, script::lua_functor_t luaFunctor) {
        return Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(milliseconds),luaFunctor);
    }

    int registerLuaProgressTimedEvent(int milliseconds, script::lua_functor_t luaFunctor, script::lua_functor_t progressFunction) {
        return Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(milliseconds),luaFunctor,progressFunction);
    }

    int registerLuaTimedEventAction(int milliseconds, gameplay::EventAction eventAction) {
        return Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(milliseconds), eventAction);
    }
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> Update* Ogre::Singleton<Update>::ms_Singleton = 0;
Update* Update::getSingletonPtr(void)
{
    return ms_Singleton;
}
Update& Update::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}

#else
template<> Update* Ogre::Singleton<Update>::msSingleton = 0;
Update* Update::getSingletonPtr(void)
{
    return msSingleton;
}
Update& Update::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
