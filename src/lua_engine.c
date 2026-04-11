#include "lua_engine.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdio.h>

#include "input.h"

void luaEngineInit(LuaEngine* engine, Camera* camera) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    engine->L = L;
    engine->camera = camera;

    // Store engine pointer in Lua registry
    lua_pushlightuserdata(engine->L, engine);
    lua_setglobal(engine->L, "_engine");

    // create "input" table
    lua_newtable(L);

    lua_pushcfunction(L, l_input_isKeyDown);
    lua_setfield(L, -2, "isKeyDown");

    lua_pushcfunction(L, l_input_isKeyPressed);
    lua_setfield(L, -2, "isKeyPressed");

    // global input = { ... }
    lua_setglobal(L, "input");

    //create "camera" table
    lua_newtable(L);

    lua_pushcfunction(L, l_camera_getPosition);
    lua_setfield(L, -2, "getPosition");

    lua_pushcfunction(L, l_camera_setPosition);
    lua_setfield(L, -2, "setPosition");

    lua_pushcfunction(L, l_camera_addPosition);
    lua_setfield(L, -2, "addPosition");

    lua_pushcfunction(L, l_camera_setRotation);
    lua_setfield(L, -2, "setRotation");

    lua_setglobal(L, "camera");
}

void luaEngineDestroy(LuaEngine* engine) {
    lua_close((lua_State*)engine->L);
}

static Camera* getCamera(LuaEngine* engine) {
    return engine->camera;
}

static LuaEngine* getEngine(lua_State* L) {
    lua_getglobal(L, "_engine");
    LuaEngine* engine = (LuaEngine*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return engine;
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

static int l_camera_getPosition(lua_State* L) {
    LuaEngine* engine = getEngine(L);
    Camera* camera = engine->camera;

    lua_pushnumber(L, camera->transform.position[0]);
    lua_pushnumber(L, camera->transform.position[1]);
    lua_pushnumber(L, camera->transform.position[2]);

    return 3;
}

static int l_camera_setPosition(lua_State* L) {
    LuaEngine* engine = getEngine(L);
    Camera* camera = engine->camera;

    camera->transform.position[0] = (float)luaL_checknumber(L, 1);
    camera->transform.position[1] = (float)luaL_checknumber(L, 2);
    camera->transform.position[2] = (float)luaL_checknumber(L, 3);

    cameraUpdateVectors(camera);
    return 0;
}

static int l_camera_addPosition(lua_State* L) {
    LuaEngine* engine = getEngine(L);
    Camera* camera = engine->camera;

    camera->transform.position[0] += (float)luaL_checknumber(L, 1);
    camera->transform.position[1] += (float)luaL_checknumber(L, 2);
    camera->transform.position[2] += (float)luaL_checknumber(L, 3);

    cameraUpdateVectors(camera);
    return 0;
}

static int l_camera_setRotation(lua_State* L) {
    LuaEngine* engine = getEngine(L);
    Camera* camera = engine->camera;

    camera->yaw = (float)luaL_checknumber(L, 1);
    camera->pitch = (float)luaL_checknumber(L, 2);

    cameraUpdateVectors(camera);
    return 0;
}
