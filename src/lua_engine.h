#pragma once

#include "camera.h"
#include "scene.h"
#include <lua.h>

typedef struct LuaEngine {
    void* L; // lua_State*

    // This will become array in Scene or something later on for multi camera support
    Camera* camera;
} LuaEngine;

void luaEngineInit(LuaEngine* engine, Camera* camera, Scene* scene);
void luaEngineDestroy(LuaEngine* engine);

// Safe helper for now to help void global camera usage in LuaEngine
static Camera* getCamera(LuaEngine* engine);

static LuaEngine* getEngine(lua_State* L);

// run script file
int luaEngineRunFile(LuaEngine* engine, const char* filename);

// run string (debug)
int luaEngineRunString(LuaEngine* engine, const char* code);

// runs global function in lua state
int luaEngineRunGlobalFunction(LuaEngine* engine, const char* name);

// Bindings
static int l_input_isKeyDown(lua_State* L);
static int l_input_isKeyPressed(lua_State* L);

static int l_camera_getPosition(lua_State* L);
static int l_camera_setPosition(lua_State* L);
static int l_camera_addPosition(lua_State* L);
static int l_camera_setRotation(lua_State* L);

static int l_spawnEntity(lua_State* L);
static int l_entity_get_name(lua_State* L);
static int l_entity_add_position(lua_State* L);

static int luaCallUpdate(lua_State* L, int id, float dt);
