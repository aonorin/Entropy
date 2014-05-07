#include "timedevent.h"

TimedEvent::TimedEvent(boost::posix_time::time_duration delay,boost::function<void (void)> &function)
{
    ++eventID;
    this->id = eventID;
    //    this->voidObject = voidObject;
    this->function = function;
    progressFuncFlag = false;
    intFuncFlag = false;
    luaFlag = false;
    eventActionFlag = false;
    executed = false;
    this->delay = delay;
}

TimedEvent::TimedEvent(boost::posix_time::time_duration delay,boost::function<void (int)> &function, int num)
{
    ++eventID;
    this->id = eventID;
    //    this->voidObject = voidObject;
    this->intFunction = function;
    progressFuncFlag = false;
    intFuncFlag = true;
    luaFlag = false;
    eventActionFlag = false;
    executed = false;
    this->delay = delay;
    this->num = num;
}

TimedEvent::TimedEvent(boost::posix_time::time_duration delay,boost::function<void (void)> &function,boost::function<void(float)> &progressFunction)
{
    ++eventID;
    this->id = eventID;
    //    this->voidObject = voidObject;
    this->function = function;
    progressFuncFlag = true;
    intFuncFlag = false;
    luaFlag = false;
    executed = false;
    eventActionFlag = false;
    this->progressFunction = progressFunction;
    this->delay = delay;

}

TimedEvent::TimedEvent(boost::posix_time::time_duration delay,boost::function<void (int)> &function,boost::function<void(float)> &progressFunction,int num)
{
    ++eventID;
    this->id = eventID;
    //    this->voidObject = voidObject;
    this->intFunction = function;
    progressFuncFlag = true;
    intFuncFlag = true;
    luaFlag = false;
    this->num = num;
    executed = false;
    eventActionFlag = false;
    this->progressFunction = progressFunction;
    this->delay = delay;
}

TimedEvent::TimedEvent(boost::posix_time::time_duration delay, script::lua_functor_t luaFunctor)
{
    ++eventID;
    this->id = eventID;
    progressFuncFlag = false;
    intFuncFlag = false;
    luaFlag = true;
    executed = false;
    eventActionFlag = false;
    this->delay = delay;
    this->luaFunctor = luaFunctor;
}

TimedEvent::TimedEvent(boost::posix_time::time_duration delay, script::lua_functor_t luaFunctor, script::lua_functor_t luaProgessFunction)
{
    ++eventID;
    this->id = eventID;
    progressFuncFlag = true;
    intFuncFlag = false;
    luaFlag = true;
    executed = false;
    eventActionFlag = false;
    this->luaProgessFunction = luaProgessFunction;
    this->delay = delay;
    this->luaFunctor = luaFunctor;
}

TimedEvent::TimedEvent(boost::posix_time::time_duration delay, gameplay::EventAction eventAction)
{
    ++eventID;
    this->id = eventID;
    progressFuncFlag = true;
    intFuncFlag = false;
    luaFlag = false;
    executed = false;
    eventActionFlag = true;
    this->delay = delay;
    this->eventAction = eventAction;
}

void TimedEvent::init()
{
    execTime = boost::posix_time::microsec_clock::local_time() + delay;
}

void TimedEvent::execute()
{
    if(!executed) {
        try{
            if(luaFlag) {
                //std::cout << "executing timed lua functor" << std::endl;
                luaFunctor.get()->call();
            } else if(eventActionFlag) {
                eventAction.call();
            } else if(intFuncFlag) {
                intFunction(num);
            } else {
                function();
            }
        } catch(...) {
            std::cout << "Bound object no longer exists!" << std::endl;
        }

        executed = true;
    }
}

float TimedEvent::update(boost::posix_time::ptime currentTime)
{
    boost::posix_time::time_duration passedTime;
    try{
        passedTime = execTime - currentTime;
    } catch(...) {
        passedTime = boost::posix_time::milliseconds(10000);
    }

    if(passedTime.total_milliseconds()<=0.0) {
        execute();
    }

    if(eventActionFlag) {
        progress = ((float)passedTime.total_milliseconds()/(float)delay.total_milliseconds());
        eventAction.updateCall(1-progress);
    } else if(!luaFlag) {
        if(progressFuncFlag) {
            try{
                progress = ((float)passedTime.total_milliseconds()/(float)delay.total_milliseconds());
                progressFunction(progress);
            } catch(...) {
                std::cout << "Bound object no longer exists!" << std::endl;
            }
        }
    } else if(luaFlag) {
        if(progressFuncFlag) {
            progress = ((float)passedTime.total_milliseconds()/(float)delay.total_milliseconds());
            luaProgessFunction.get()->addArgument(1-progress);
            luaProgessFunction.get()->call();
            luaProgessFunction.get()->clearArguments();
        }
    } else {

    }
    return passedTime.total_milliseconds();
}

int TimedEvent::getID()
{
    //    std::cout << "Getting TimedEvent ID: " << this->id << std::endl;
    return this->id;
}
