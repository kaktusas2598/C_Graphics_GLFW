#include "lua_engine.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdio.h>

#include "input.h"

static Scene* gScene = NULL;

void luaEngineInit(LuaEngine* engine, Camera* camera, Scene* scene) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    engine->L = L;
    engine->camera = camera;
    gScene = scene;

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


    lua_newtable(L); // "spawn"

    lua_pushcfunction(L, l_spawnEntity);
    lua_setfield(L, -2, "entity");

    lua_setglobal(L, "spawn");

    lua_newtable(L); // "entity"

    lua_pushcfunction(L, l_entity_get_name);
    lua_setfield(L, -2, "getName");

    lua_pushcfunction(L, l_entity_add_position);
    lua_setfield(L, -2, "addPosition");

    lua_setglobal(L, "entity");
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

static int l_spawnEntity(lua_State* L) {
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float z = luaL_checknumber(L, 3);

    float r = luaL_checknumber(L, 4);
    float g = luaL_checknumber(L, 5);
    float b = luaL_checknumber(L, 6);

    float speed = luaL_checknumber(L, 7);
    const char* name = luaL_optstring(L, 8, "entity");

    vec3 pos = {x, y, z};
    vec3 color = {r, g, b};

    int id = sceneSpawnEntity(gScene, pos, color, speed, name);
    lua_pushinteger(L, id);

    return 1;
}

static int l_entity_get_name(lua_State* L) {
    int id = luaL_checkinteger(L, 1);

    Entity* e = sceneGetEntity(gScene, id);
    if (!e) {
        lua_pushstring(L, "");
        return 1;
    }

    lua_pushstring(L, e->name);
    return 1;
}

static int l_entity_add_position(lua_State* L) {
    int id = luaL_checkinteger(L, 1);

    vec3 delta = {
        luaL_checknumber(L, 2),
        luaL_checknumber(L, 3),
        luaL_checknumber(L, 4)
    };

    sceneAddPosition(gScene, id, delta);
    return 0;
}

static int luaCallUpdate(lua_State* L, int id, float dt) {
    lua_getglobal(L, "updateEntity");

    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        return 0;
    }

    lua_pushinteger(L, id);
    lua_pushnumber(L, dt);

    if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
        printf("Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    return 1;
}
