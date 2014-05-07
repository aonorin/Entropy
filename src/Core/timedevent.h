#pragma once
#include <time.h>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Utility/WeakFunc.h"
#include "Script/scripttypes.h"
#include "gameplay/EventAction.h"

static int eventID = 1;

class TimedEvent
{
public:
    TimedEvent(boost::posix_time::time_duration delay,boost::function<void (void)> &function);
    TimedEvent(boost::posix_time::time_duration delay,boost::function<void (int)> &function, int num);
    TimedEvent(boost::posix_time::time_duration delay,boost::function<void (void)> &function,boost::function<void (float)> &progressFunction);
    TimedEvent(boost::posix_time::time_duration delay,boost::function<void (int)> &function,boost::function<void (float)> &progressFunction,int num);

    //Lua timed event
    TimedEvent(boost::posix_time::time_duration delay,script::lua_functor_t luaFunctor);
    TimedEvent(boost::posix_time::time_duration delay,script::lua_functor_t luaFunctor, script::lua_functor_t luaProgessFunction);

    //Lua EventAction
    TimedEvent(boost::posix_time::time_duration delay, gameplay::EventAction eventAction);

    int getID();
    void init();

    void execute();
    float update(boost::posix_time::ptime currentTime);

private:
    int id;
    bool progressFuncFlag;
    bool intFuncFlag;
    bool luaFlag;
    int num;
    bool executed;
    float progress;
    boost::weak_ptr<void> voidObject;
    boost::posix_time::ptime execTime;
    boost::posix_time::time_duration delay;
    boost::function<void (void)> function;
    boost::function<void (int)> intFunction;
    boost::function<void (float)> progressFunction;
    script::lua_functor_t luaFunctor;
    script::lua_functor_t luaProgessFunction;
    gameplay::EventAction eventAction;
    bool eventActionFlag;
};
