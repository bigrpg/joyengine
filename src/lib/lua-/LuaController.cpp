//
//  LuaController.cpp
//  luatest
//
//  Created by appl on 13-5-14.
//
//

#include "LuaController.h"

#include <stdio.h>
#include <assert.h>

#include <vector>
#include <string>


#define SETDEFAULTLUA \
if(!L) \
L = g_defaultLua;


namespace script
{
    std::vector<std::string>    s_importSerachPaths;
    lua_State * g_defaultLua = NULL;
    
    template void getFieldTuple<std::string>(std::vector<std::string> &vec,lua_State *L);
    
    template void getFieldTuple<double>(std::vector<double> &vec,lua_State *L);
    
    template bool getFieldValue<const char *,std::string>(const char * key,std::string & value,const std::string & defaultValue,lua_State *L);
    
    template bool getFieldTable<const char *>(const char * key,lua_State * L);
    
    
    template void getFieldsTuple<int>(const char * path, std::vector<int> & vec,lua_State * L);
    
    template bool getFieldsValue<int,int>(const char * path,int key,int & value,const int & defaultValue ,lua_State *L);
    
    template bool getFieldsValue<const char *,int>(const char * path,const char * key,int & value,const int & defaultValue ,lua_State *L);
    
    template bool getFieldsValue<const char *,std::string>(const char * path,const char * key,std::string & value,const std::string & defaultValue ,lua_State *L);
    
    
    template<typename T>
    struct PushValue
    {
        PushValue(lua_State * L,T & v)
        {
            lua_pushnumber(L,(lua_Number)v);
        }
    };

    
    template<>
    struct PushValue<const char *>
    {
        PushValue(lua_State *L,const char *& v)
        {
            lua_pushstring(L,v);
        }
    };
    
    template<>
    struct PushValue< char *>
    {
        PushValue(lua_State *L, char *& v)
        {
            lua_pushstring(L,v);
        }
    };
    
    template<typename T>
    struct PopValue
    {
        PopValue(lua_State * L,T & v)
        {
            v = (T)lua_tonumber(L, -1);
        }
    };
    
    template<>
    struct PopValue<std::string>
    {
        PopValue(lua_State * L,std::string & v)
        {
            v = lua_tostring(L, -1);
        }
    };
    
    
    template<typename T>
    bool isT(lua_State * L,const T& dummy)
    {
        return false;
    }
   
    template<>
    bool isT(lua_State * L,const double& dummy)
    {
        return !!lua_isnumber(L, -1);
    }
    
    template<>
    bool isT(lua_State * L,const int& dummy)
    {
        return !!lua_isnumber(L, -1);
    }
    
    template<>
    bool isT(lua_State * L,const std::string& dummy)
    {
        return !!lua_isstring(L, -1);
    }

    
    template<typename T>
    bool getFieldTable(T key,lua_State * L)
    {
        SETDEFAULTLUA
        PushValue<T> pushkey(L,key);
        
        lua_gettable(L, -2); /* get background[key] */
        if (!lua_istable(L, -1))
        {
            printf("should must be a table\n");
            lua_pop(L, 1);
            return false;
        }
        return true;
    }
    
    template<typename T>
    bool _getFieldValue(T & result,const T& defaultValue,lua_State * L)
    {
        lua_gettable(L, -2); /* get background[key] */
        if (!isT(L,result))
        {
            printf("should must be a int\n");
            lua_pop(L, 1);
            result = defaultValue;
            return false;
        }
        PopValue<T>  popValue(L,result);
        lua_pop(L, 1); /* remove number */
    
        return true;
        
    }
    
    template<typename T,typename V>
    bool getFieldValue(T key,V & value,const V & defaultValue,lua_State *L)
    {
        SETDEFAULTLUA
        PushValue<T> pushkey(L,key);
        return _getFieldValue(value,defaultValue,L);
    }
   
    template<typename T>
    void getFieldTuple(std::vector<T> & vec,lua_State * L)
    {
        SETDEFAULTLUA
        int count = luaL_len(L,-1);
        for(int i=1; i <= count; i++)
        {
            T value;
            getFieldValue(i,value,T(),L);
            vec.push_back(value);
        }
    }

    lua_State * initLua()
    {
        lua_State * L=  luaL_newstate();
        luaL_openlibs(L);
        return L;
    }
    
    void finiLua(lua_State * L)
    {
        SETDEFAULTLUA
    
        s_importSerachPaths.clear();
    
        if(!L)
            return;
    
        lua_close(L);
        if( L == g_defaultLua)
            g_defaultLua = NULL;
    }
    
    void setDefaultLua(lua_State *L)
    {
        g_defaultLua = L;
    }

    void getGlobalTable(lua_State * L)
    {
        SETDEFAULTLUA
        lua_pushglobaltable(L);
    }
    
    void removeTable(int times, lua_State * L)
    {
        SETDEFAULTLUA
        lua_pop(L, times);
    }
    
    void getFieldFile(const char * file)
    {
        assert(script::import(file));
        script::getGlobalTable();

    }
    
    int  getFieldsTable(const char * path, lua_State * L)
    {
        SETDEFAULTLUA
        bool  module = false;
        if (*path != '/')
            module = true;
        else
            path++ ;
        
        char * p = strchr(path,'/');
        
        char  buff[50] ;
        if(p==NULL)
        {
            strcpy(buff, path);
            int size = strlen(path);
            buff[size] = '\0';
           
        }
        else
        {
            int size = p-path;
            strncpy(buff, path,size);
            buff[size] = '\0';
        }
       
        if(module)
        {
            assert(script::import(buff));
            script::getGlobalTable();
        }
        else
        {
            if (buff[0] ==':')
            {
                int  key = atoi(buff+1);
                getFieldTable(key,L);
            }
            else
            {
                getFieldTable(buff,L);
            }
        }
        
        if(p==NULL)
            return 1;
        
        return 1 + getFieldsTable(p,L);
        
    }
    
    template<typename T,typename V>
    bool getFieldsValue(const char * path,T key,V & value,const V & defaultValue,lua_State *L)
    {
        SETDEFAULTLUA
        int num = getFieldsTable(path);
        getFieldValue(key,value,defaultValue,L);
        removeTable(num);
        return  true;
    }
    
    template<typename T>
    void getFieldsTuple(const char * path, std::vector<T> & vec,lua_State * L)
    {
        SETDEFAULTLUA
        int num = getFieldsTable(path);
        getFieldTuple(vec);
        removeTable(num);
    }
    
    
    void appendImportSearchPath(const char * str)
    {
        std::string path = str;
        size_t len = path.length();
        if( path[len-1] == '/')
            s_importSerachPaths.push_back(path);
        else
            s_importSerachPaths.push_back(path+"/");
    }
    
    //keep balance of stack
    bool import(const char * name,lua_State * L)
    {
        SETDEFAULTLUA
        
        lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
        
        lua_getfield(L, -1, name);  /* _LOADED[name] */
        if (lua_toboolean(L, -1))  /* is it there? */
        {
            lua_pop(L, 2);
            return true;  /* package is already loaded */
        }
        
        lua_pop(L,1);
        
        //_LOADED in stack
        
        lua_pushboolean(L,1);
        lua_setfield(L, -2, name);  //记录它，即使它失败

        
        FILE * fp = NULL;
        std::vector<std::string>::const_iterator it = s_importSerachPaths.begin();
        for(;it !=s_importSerachPaths.end();++it)
        {
            const std::string& path = *it;
            fp = fopen( (path + name).c_str(),"rb");
            if(fp)
                break;
        }
        
        if(fp)
        {
            fseek(fp,0,SEEK_END);
            size_t len = ftell(fp);
            char * buff = new char[len];
            if(buff)
            {
                fseek(fp,0,SEEK_SET);
                assert(fread(buff,1,len,fp) == len);
                //解密处理
                //....
                fclose(fp);
                if(LUA_OK == luaL_loadbufferx(L,buff,len,name,NULL))    //[_LOADED,f]
                {
                    if( LUA_OK != lua_pcall(L, 0, 0, 0))
                    {
                        printf("import %s error:%s\n",name,lua_tostring(L, -1));
                        lua_pop(L,2);   //pop掉错误信息和_LOADED表，是的stack恢复
                        delete [] buff;
                        return false;
                    }
                    //如果pcall成功，stack只剩下_LOADED
                    //lua_pushboolean(L,1);
                    //lua_setfield(L, -2, name);
                    lua_pop(L, 1);
                    delete [] buff;
                    return true;
                }
                else
                {
                    printf("import %s error:%s\n",name,lua_tostring(L, -1));
                    lua_pop(L,2);   //pop掉错误信息和_LOADED表，是的stack恢复
                }
                delete [] buff;
                return false;
            }
        }
        lua_pop(L, 1); //pop掉_LOADED
        return false;
    }

}