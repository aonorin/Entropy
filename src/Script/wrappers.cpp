// system includes
#include <iostream>
#include <vector>

// SLB includes
#ifndef WIN32
#include "libraries/SLB2/SLB.hpp"
#else
#include "libraries/SLB2/SLB.hpp"
#endif

// Boost
#include "boost/function.hpp"

// Entropy includes
#include "wrappers.h"
#include "src/Physics/physics.h"
#include "src/Menu/inventory.h"
#include "src/gameplay/quest.h"
#include "src/Script/script.h"
#include "src/gameplay/actors.h"
#include "src/Core/update.h"
#include "src/gameplay/EventAction.h"
#include "src/Menu/chatscript.h"
#include "VoxelTerrain/terraingenerator.h"
#include "gameplay/actor/doombot.h"

namespace script {

void lout(std::string printString)
{
    std::cout << printString << std::endl;
}

lua_functor_t testFunction;
// int testFunctionRef

void saveFunction(lua_functor_t functionRef)
{
    testFunction = functionRef;
    //lua_pushcfunction(interpreter._lua_state, functionRef);
    //testFunctionRef = luaL_ref(interpreter._lua_state, LUA_REGISTRYINDEX);
}

// NOTE: FOR SOME REASON CALLING A FUNCTION NOT IMMEDIATELY DOESN'T WORK
// PERHAPS IT HAS SOMETHING TO DO WITH THE CONTEXT??????????????????????
void printFunction()
{
    testFunction->call();
    //lua_rawgeti(interpreter._lua_state, LUA_REGISTRYINDEX, testFunctionRef); // Push the function on top the stack
    // NOT USED LAST interpreter.pushString("Hello Lua! This is C++!");
    //lua_call(interpreter._lua_state, 1, 0);
    //testFunction = functionRef;
    //interpreter.pushString("Hello Lua! This is C++!");
    //interpreter.callLuaFunction(testFunction, 0);
    //std::cout << "Function Reference: " << testFunction << std::endl;
}

void printEventAction(gameplay::EventAction eventAction)
{
    eventAction.call();
    std::cout << "Event Action type: " << eventAction.type() << std::endl;
}

void compileTypes(SLB::Manager* manager)
{

}

void compileWrappers(SLB::Manager* manager)
{
    // print out from Lua
    manager->set("lout", SLB::FuncCall::create(lout));

    // print function reference
    manager->set("saveFunction", SLB::FuncCall::create(saveFunction));
    manager->set("printFunction", SLB::FuncCall::create(printFunction));

    // Physics namespace
    manager->set("setGravity", SLB::FuncCall::create(physics::setGravity)); // Note DON'T use & reference for namespace functions
    manager->set("applyForce", SLB::FuncCall::create(physics::applyForce));
    manager->set("spawnRope", SLB::FuncCall::create(physics::spawnTestRope));
    manager->set("spawnBlob", SLB::FuncCall::create(physics::spawnBlob));

    // Script namespace
    manager->set("loadScript", SLB::FuncCall::create(script::loadScript));

    // Inventory
    manager->set("addItem", SLB::FuncCall::create(&Inventory::staticAddItem));
    manager->set("removeItem", SLB::FuncCall::create(&Inventory::staticRemoveItem));
    manager->set("combineItems", SLB::FuncCall::create(&Inventory::staticCombineItems));
    manager->set("addUseItemAction", SLB::FuncCall::create(&Inventory::staticAddUseAction));
    manager->set("removeUseItemAction", SLB::FuncCall::create(&Inventory::staticRemoveUseAction));
    manager->set("useItemAction", SLB::FuncCall::create(&Inventory::staticUseAction));

    // MetaQuest
    manager->set("addNewQuest", SLB::FuncCall::create(&gameplay::MetaQuest::addNewQuest));
    manager->set("addQuestItem", SLB::FuncCall::create(&gameplay::MetaQuest::addQuestItem));
    manager->set("setQuestItemCompleted", SLB::FuncCall::create(&gameplay::MetaQuest::setQuestItemCompleted));
    manager->set("activateQuest", SLB::FuncCall::create(&gameplay::MetaQuest::activateQuest));
    manager->set("forceCompleteQuest", SLB::FuncCall::create(&gameplay::MetaQuest::forceCompleteQuest));
    manager->set("silentCompleteQuest", SLB::FuncCall::create(&gameplay::MetaQuest::silentCompleteQuest));
    manager->set("questActive", SLB::FuncCall::create(&gameplay::MetaQuest::questActive));
    manager->set("questInactive", SLB::FuncCall::create(&gameplay::MetaQuest::questInactive));
    manager->set("questCompleted", SLB::FuncCall::create(&gameplay::MetaQuest::questCompleted));
    manager->set("resetQuest", SLB::FuncCall::create(&gameplay::MetaQuest::resetQuest));
    manager->set("clearQuests", SLB::FuncCall::create(&gameplay::MetaQuest::reset));
    manager->set("setQuestDescription", SLB::FuncCall::create(&gameplay::MetaQuest::setQuestDescription));
    manager->set("setQuestItemDescription", SLB::FuncCall::create(&gameplay::MetaQuest::setQuestItemDescription));

    // Actors
    manager->set("createActor", SLB::FuncCall::create(gameplay::actors::createActor));

    // ActorUpdateActions static methods bound like a namespace so we don't have to make objects
    manager->set("newPathFindAction", SLB::FuncCall::create(&gameplay::PathFindUpdate::newPathFindAction));

    // CollisionGroupManager
    manager->set("addCollisionGroup", SLB::FuncCall::create(&physics::CollisionGroupManager::addCollisionGroup));

    //Lua Timed Events
    manager->set("timedFunction", SLB::FuncCall::create(script::registerLuaTimedEvent));
    manager->set("progressFunction", SLB::FuncCall::create(script::registerLuaProgressTimedEvent));
    manager->set("timedEvent", SLB::FuncCall::create(script::registerLuaTimedEventAction));

    //TerrainGenerator
    manager->set("genGame",SLB::FuncCall::create(&voxel::TerrainGenerator::genGameOfLife));
    manager->set("genGame3D",SLB::FuncCall::create(&voxel::TerrainGenerator::genGameOfLife3d));
    manager->set("genWolfram",SLB::FuncCall::create(&voxel::TerrainGenerator::genWolfram));
    manager->set("terrainReset",SLB::FuncCall::create(&voxel::TerrainGenerator::reset));
    manager->set("genSimplex",SLB::FuncCall::create(&voxel::TerrainGenerator::genSimplex));

    //CollisionCamera
    manager->set("flashlightAttenuation",SLB::FuncCall::create(&CollisionCamera::setFlashLightAttenuation));
    manager->set("flashlightPowerscale",SLB::FuncCall::create(&CollisionCamera::setFlashlightPowerscale));
    manager->set("flashlightSpacing", SLB::FuncCall::create(&CollisionCamera::setFlashlightSpacingScale));
    manager->set("flashlightFocusDistance",SLB::FuncCall::create(&CollisionCamera::setFlashlightFocusDistance));
    manager->set("setPosition",SLB::FuncCall::create(&Entropy::setPlayerPosition));

    ////////////////
    // EventActions
    ////////////////

    // If you're confused about how to bind something, reference this link:
    // http://code.google.com/p/slb/wiki/SLBClassReference

    // EventAction, don't use this class directly, we're binding just to make sure SLB knows about it
    SLB::Class<gameplay::EventAction>("EventAction", manager)
            .constructor()
            .enumValue("type", gameplay::AbstractEventAction)
            .set("call", &gameplay::EventAction::call)
            .set("updateCall", &gameplay::EventAction::updateCall);

    // FireballEvent
    SLB::Class<gameplay::FireballEvent>("FireballEvent", manager)
            .constructor<int>() // Note that the constructor argument is given in a template format
            .dynamic_inherits<gameplay::EventAction>() // Make sure to make it inherit!
            .enumValue("type", gameplay::FireballEventAction) // Notice the lack of an & sign!
            .set("call", &gameplay::FireballEvent::call)
            .set("getDamage", &gameplay::FireballEvent::getDamage)
            .set("updateCall", &gameplay::FireballEvent::updateCall);

    // Other style of binding for the static method. EventActions can be created using either the static call or the normal lua style
    manager->set("newFireballEvent", SLB::FuncCall::create(&gameplay::FireballEvent::newFireballEvent));

    // CustomEvent
    SLB::Class<gameplay::CustomEvent>("CustomEvent", manager)
            .constructor<script::lua_functor_t>()
            .dynamic_inherits<gameplay::EventAction>()
            .enumValue("type", gameplay::CustomEventAction)
            .set("call", &gameplay::CustomEvent::call);

    manager->set("newCustomEvent", SLB::FuncCall::create(&gameplay::CustomEvent::newCustomEvent));

    // Testing
    manager->set("printEventAction", SLB::FuncCall::create(printEventAction));

    manager->set("chatAction",SLB::FuncCall::create(&ChatScript::addChatAction));
    manager->set("doombot",SLB::FuncCall::create(&hypnos::DoomBot::constructDoomBot));

    ////////////////////
    // CollisionActions
    ////////////////////

    manager->set("newForceCollisionAction", SLB::FuncCall::create(&gameplay::ForceCollision::newForceCollisionAction));
}

} // script namespace
