#ifndef EVENTACTION_H
#define EVENTACTION_H

// Boost includes
#include "boost/function.hpp"

// Entropy includes
#include "src/Script/scripttypes.h"

namespace gameplay {

enum EventActionType
{
    AbstractEventAction = 0,
    FireballEventAction = 1,
    CustomEventAction = 2
};

typedef boost::function<void()> event_call_t;
typedef boost::function<void(float)> update_call_t;

////////////////
// EventAction
////////////////

class EventAction
{
public:
    EventAction();

    virtual void call(); // Overload this!
    virtual void updateCall(float progress);
    virtual EventActionType type(); // Overload this!

    // Uses static methods like this, bind to Lua, to create new actions
    static EventAction newEventAction(); // Don't use this, just an example

protected:

    event_call_t childEventCall;
    update_call_t childUpdateCall;
    EventActionType actionType;
};

/////////////////
// FireballEvent
/////////////////

class FireballEvent : public EventAction
{
public:
    FireballEvent(int damage);

    virtual void call();
    virtual void updateCall(float progress);
    virtual EventActionType type();
    int getDamage();

    static FireballEvent newFireballEvent(int damage);

protected:

    int damage;
};

////////////////
// CustomEvent
////////////////

class CustomEvent : public EventAction
{
public:
    CustomEvent(script::lua_functor_t customFunction);

    virtual void call();
    virtual EventActionType type();

    static CustomEvent newCustomEvent(script::lua_functor_t customFunction);

protected:

    script::lua_functor_t customFunction;
};

} // gameplay namespace

#endif // EVENTACTION_H
