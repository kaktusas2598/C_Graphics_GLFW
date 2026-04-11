#include "lua_engine.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdio.h>

#include "input.h"

void luaEngineInit(LuaEngine* engine) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    engine->L = L;

    // create "input" table
    lua_newtable(L);

    lua_pushcfunction(L, l_input_isKeyDown);
    lua_setfield(L, -2, "isKeyDown");

    lua_pushcfunction(L, l_input_isKeyPressed);
    lua_setfield(L, -2, "isKeyPressed");

    // global input = { ... }
    lua_setglobal(L, "input");
}

void luaEngineDestroy(LuaEngine* engine) {
    lua_close((lua_State*)engine->L);
}

int luaEngineRunFile(LuaEngine* engine, const char* filename) {
    lua_State* L = (lua_State*)engine->L;

    if (luaL_dofile(L, filename)) {
        printf("Lua error: %s\n", lua_tostring(L, -1));
        return 0;
    }

    return 1;
}

int luaEngineRunGlobalFunction(LuaEngine* engine, const char* name) {
    lua_State* L = (lua_State*)engine->L;

    lua_getglobal(L, name);

    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
    } else {
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            printf("Lua error: %s\n", lua_tostring(L, -1));
            return 0;
        }
    }

    return 1;
}

int luaEngineRunString(LuaEngine* engine, const char* code) {
    lua_State* L = (lua_State*)engine->L;

    if (luaL_dostring(L, code)) {
        printf("Lua error: %s\n", lua_tostring(L, -1));
        return 0;
    }

    return 1;
}

int l_input_isKeyDown(lua_State* L) {
    int key = (int)luaL_checkinteger(L, 1);

    int result = inputIsKeyDown(key);

    lua_pushboolean(L, result);
    return 1;
}

int l_input_isKeyPressed(lua_State* L) {
    int key = (int)luaL_checkinteger(L, 1);

    int result = inputIsKeyPressed(key);

    lua_pushboolean(L, result);
    return 1;
}
