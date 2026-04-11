#pragma once

#include <lua.h>

typedef struct LuaEngine {
    void* L; // lua_State*
} LuaEngine;

void luaEngineInit(LuaEngine* engine);
void luaEngineDestroy(LuaEngine* engine);

// run script file
int luaEngineRunFile(LuaEngine* engine, const char* filename);

// run string (debug)
int luaEngineRunString(LuaEngine* engine, const char* code);

// runs global function in lua state
int luaEngineRunGlobalFunction(LuaEngine* engine, const char* name);

// Bindings
static int l_input_isKeyDown(lua_State* L);
static int l_input_isKeyPressed(lua_State* L);
