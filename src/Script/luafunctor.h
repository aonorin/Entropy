#ifndef LUAFUNCTOR_H
#define LUAFUNCTOR_H

// System includes
#include <deque>

// Boost includes
#include "boost/variant.hpp"

// SLB includes
#ifndef WIN32
#include "libraries/SLB2/SLB.hpp"
#else
#include "libraries/SLB2/SLB.hpp"
#include "libraries/SLB2/lua/luaconf.h"
#include "libraries/SLB2/lua/lualib.h"
#include "libraries/SLB2/lua/lua.h"
#endif
#include "Script/script.h"

namespace script {

typedef boost::variant<bool, int, double, std::string> lua_functor_arg_t;

class PushArgVisitor : public boost::static_visitor<>
{
public:

    void operator()(bool& b) const
    {
        lua_pushboolean(script::luaState(), b);
    }

    void operator()(int& i) const
    {
        lua_pushnumber(script::luaState(), i);
    }

    void operator()(double& d) const
    {
        lua_pushnumber(script::luaState(), d);
    }

    void operator()(std::string& s) const
    {
        lua_pushlstring(script::luaState(), s.c_str(), s.length());
    }
};

// Encapsulates a Lua function upon creation. Only create this as an argument in a lua bound function!
class LuaFunctor
{
public:
    LuaFunctor(int functionKey = -1);
    ~LuaFunctor();

    void operator()();
    void call();
    void addArgument(script::lua_functor_arg_t arg); // Adds an argument to the args deque, these are retained through calls
    void clearArguments(); // Clears the args deque. Use this first if you want to supply new arguments.

    int functionKey; // lua registry key
    bool notEmpty; // indicates if we have a valid functionKey

    std::deque<script::lua_functor_arg_t> args;

protected:

    int pushArguments(); // Overload this for functions that require arguments, return the number of arguments

};

}

#endif // LUAFUNCTOR_H
