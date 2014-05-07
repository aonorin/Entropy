// OgreIncludes
#include "OGRE/OgreResource.h"
#include "OGRE/OgreDataStream.h"
#include <OGRE/OgreSceneManager.h>

// Local includes
#include "script.h"
#include "wrappers.h"

using namespace Ogre;

namespace script {

//////////////////
// Namespace Vars
//////////////////
SLB::Manager manager;
SLB::EntropyScript interpreter(&manager);

///////////////////////
// Namespace Functions
///////////////////////

void initialize()
{
    compileWrappers(&manager);
    loadScriptLibrary();
}

bool interpret(const char *luaCode)
{
    return interpreter.safeDoString(luaCode);
}

void loadScriptLibrary()
{
    StringVectorPtr names = ResourceGroupManager::getSingleton().listResourceNames("DefaultLuaScripts");
    for(int i = 0; i < names->size(); ++i)
    {
        loadScript(names->at(i));
    }
    names.setNull();
}

void loadScript(std::string fileName)
{
    DataStreamPtr pStream = ResourceGroupManager::getSingleton().openResource(
                fileName,
                "LuaScripts");

    String data = pStream.getPointer()->getAsString();
    interpret(data.c_str());
    pStream.getPointer()->close();
    pStream.setNull();
}

lua_State* luaState()
{
    return interpreter.getState();
}

void setPrintCallback(SLB::EntropyScript::PrintCallback printCallback)
{
    interpreter.setPrintCallback(printCallback);
}

void addWrapper(SLB::String funcName, boost::any funcCall)
{
    manager.set(funcName,SLB::FuncCall::create(boost::any_cast<lua_CFunction>(funcCall)));
}

} // script namespace
