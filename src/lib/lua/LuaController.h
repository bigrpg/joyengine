//
//  LuaController.h
//  luatest
//
//  Created by appl on 13-5-14.
//
//

#ifndef __luatest__LuaController__
#define __luatest__LuaController__

#include <iostream>
#include <vector>
#include <string>
//#include "ScriptDefine.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace script
{
    lua_State * initLua();
    
    void finiLua(lua_State * L=NULL);
    
    void setDefaultLua(lua_State *L);
    
    // 获取当前表key的value
    template<typename T,typename V>
    bool getFieldValue(T key,V & value,const V & defaultValue = V(),lua_State *L=NULL);

    // push key 对应的表
    template<typename T>
    bool getFieldTable(T key,lua_State * L=NULL);
    
    void getGlobalTable(lua_State * L=NULL);
    
    void removeTable(int times=1, lua_State * L=NULL);
    
    void getFieldFile(const char * file);
    
    // 取得当前表的tuple
    template<typename T>
    void getFieldTuple(std::vector<T> & vec,lua_State * L=NULL);
    
    // 获取多级表: 包含文件: "fuben.lua/fuben/:1:/child", 不包含文件 "/fuben"
    int  getFieldsTable(const char * path, lua_State * L=NULL);
    
    // 获取多级表path中key对应的value
    template<typename T,typename V>
    bool getFieldsValue(const char * path,T key,V & value,const V & defaultValue = V(),lua_State *L=NULL);
    
    // 取得多级表的path对应的tuple
    template<typename T>
    void getFieldsTuple(const char * path, std::vector<T> & vec,lua_State * L=NULL);
    
    
    void appendImportSearchPath(const char * path);
    
    bool import(const char * luafile,lua_State * L=NULL);
}

#endif /* defined(__luatest__LuaController__) */
