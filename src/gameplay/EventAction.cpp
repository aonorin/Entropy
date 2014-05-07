// System includes
#include <iostream>

// Boost includes
#include "boost/bind.hpp"

// Entropy includes
#include "EventAction.h"

namespace gameplay {

///////////////
// EventAction
///////////////

EventAction::EventAction() :
    childEventCall(0),
    childUpdateCall(0)
{
    actionType = AbstractEventAction;
}

void EventAction::call()
{
    if(!childEventCall.empty())
    {
        childEventCall();
    }

    else
    {
        // in your subclass, your action will be called here
        std::cout << "Abstract EventAction called. No Action taken." << std::endl;
    }
}

void EventAction::updateCall(float progress)
{
    if(!childUpdateCall.empty())
    {
        childUpdateCall(progress);
    }

    else
    {
        // in your subclass, your action will be called here
        //std::cout << "Abstract EventAction called. No Action taken." << std::endl;
    }
}

EventActionType EventAction::type()
{
    return actionType;
}

EventAction EventAction::newEventAction()
{
    return EventAction(); // returns an instance of the event action
}

/////////////////
// FireballEvent
/////////////////

// YOU MUST MAKE SURE TO BIND YOUR CALL METHOD AND ASSIGN YOUR TYPE IN THE CONSTRUCTOR!
FireballEvent::FireballEvent(int damage) :
    damage(damage)
{
    childEventCall = boost::bind(&FireballEvent::call, this);
    childUpdateCall = boost::bind(&FireballEvent::updateCall, this, _1);
    actionType = FireballEventAction;
}

void FireballEvent::call()
{
    // Do something else here, this is just for testing.
    std::cout << "FireballEvent::call() with damage: " << damage << std::endl;
}

void FireballEvent::updateCall(float progress)
{
    // Do something else here, this is just for testing.
    std::cout << "Charging fireball: " << progress << std::endl;
}

EventActionType FireballEvent::type()
{
    return FireballEventAction;
}

int FireballEvent::getDamage()
{
    return damage;
}

FireballEvent FireballEvent::newFireballEvent(int damage)
{
    return FireballEvent(damage);
}

///////////////
// CustomEvent
///////////////

CustomEvent::CustomEvent(script::lua_functor_t customFunction) :
    customFunction(customFunction)
{
    childEventCall = boost::bind(&CustomEvent::call, this);
    actionType = CustomEventAction;
}

void CustomEvent::call()
{
    customFunction->call(); // call the function
}

EventActionType CustomEvent::type()
{
    return CustomEventAction;
}

CustomEvent CustomEvent::newCustomEvent(script::lua_functor_t customFunction)
{
    return CustomEvent(customFunction);
}

} // gameplay namespace
