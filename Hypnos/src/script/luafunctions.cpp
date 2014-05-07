#include "luafunctions.h"
//#include "Script/wrappers.h"
//#include "Hypnos/src/script/luafunctions.h"
//#include "Script/script.h"
//#include "Script/EntropyScript.hpp"
//#include "Script/luafunctor.h"
//#include "Script/scripttypes.h"
#ifndef WIN32
//#include "SLB/SLB.hpp"
//#include "SLB/FuncCall.hpp"
#else
#include "libraries/SLB2/SLB.hpp"
#endif
//#include "gameplay/actor/doombot.h"

namespace hypnos {

void testFunc()
{

}

LuaFunctions::LuaFunctions()
{
//    script::manager.set("doomBot", SLB::FuncCall::create(&DoomBot::constructDoomBot));
//    boost::function func =
//    boost::function<void ()> func;
//    void(*)() test = &testFunc;
//    void (*foo)();
//    foo = &testFunc;
//    boost::any test = &testFunc;
//    lua_CFunction test2 = boost::any_cast<lua_CFunction>(test);
//    lua_CFunction = &testFunc;
//    void (*foo)(void) test = &testFunc;
//    script::addWrapper("doombot",&DoomBot::constructDoomBot);
//    SLB::FuncCall::create(&DoomBot::constructDoomBot);
}

}
