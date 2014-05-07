#ifndef SCRIPT_H
#define SCRIPT_H

// SLB includes
#ifndef WIN32
#include "libraries/SLB2/SLB.hpp"
#include "libraries/SLB2/lua/lua.h"
#else
#include "libraries/SLB2/SLB.hpp"
#include "libraries/SLB2/lua/lua.h"
#endif

// local includes
#include "Script/EntropyScript.hpp"
//#include "scripttypes.h"
//#include "src/Script/luafunctor.h"
#include "boost/any.hpp"

namespace script {

//////////////////
// Namespace Vars
//////////////////

extern SLB::Manager manager;
extern SLB::EntropyScript interpreter;

///////////////////////
// Namespace Functions
///////////////////////

void initialize(); // THIS MUST BE CALLED AT STARTUP BEFORE ANY CODE CAN BE INTERPRETTED
bool interpret(const char* luaCode); // returns whether the code was successfully interpretted or not
void loadScriptLibrary(); // Loads the script lua script library
void loadScript(std::string fileName); // Relative to the LuaScripts folder!!!!!!!!!!
lua_State* luaState();
void setPrintCallback(SLB::EntropyScript::PrintCallback printCallback); // Set the print callback from Lua
void addWrapper(SLB::String funcName, boost::any funcCall);

} // script namespace

#endif // SCRIPT_H
