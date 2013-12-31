#include "MySqlConnectionPool.h"
#include "DBConfig.h"
#include "Wrapper.h"


#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;


MySqlConnectionPool & MySqlConnectionPool::instance()
{
	static MySqlConnectionPool	s_inst;
	return s_inst;
}

bool MySqlConnectionPool::initMySqlOneConnection(const ConnectionInfo& connectInfo)
{
		MySql * connect = new MySql(connectInfo);
		if(!connect->reconnect())
		{
			delete connect;
			return false;
		}
		mysql_conns_.push_back(connect);
    return true;
}


bool MySqlConnectionPool::init(const ConnectionInfo& connectInfo,int count)
{
    if(pthread_mutex_init(&g_mutex,NULL))
    {
    	printf("init mutex in MySql connection Pool.\n");
    	return false;
    }
    if(pthread_cond_init(&g_cond,NULL))
    {
    	printf("init cond in MySql connection Pool.\n");
    	return false;
    }

    bool success = false;
    for(int i=0;i<count;++i)
    {
        success |= initMySqlOneConnection(connectInfo);
    }

    printf("init mysql conns pool size is:%i\n",(int)mysql_conns_.size());
    return success;
}

void MySqlConnectionPool::fini()
{
    for(size_t i=0;i<mysql_conns_.size();++i)
    {
        MySql * mysql = mysql_conns_[i];
        mysql->close();
        delete mysql;
    }
    mysql_conns_.clear();
}

MySql * MySqlConnectionPool::get()
{
    MySql * conn;
    pthread_mutex_lock(&g_mutex);
    while(mysql_conns_.size() == 0)
    {
        pthread_cond_wait(&g_cond,&g_mutex);
    }
    if(!mysql_conns_.empty())
    {
       float a = rand()/((float)RAND_MAX+1);
       int index = floor(a*mysql_conns_.size());
       conn = mysql_conns_[index];
    }
    pthread_mutex_unlock(&g_mutex);
    return conn; 
}

void MySqlConnectionPool::give(MySql * conn)
{
    pthread_mutex_lock(&g_mutex);
    mysql_conns_.push_back(conn);
    pthread_mutex_unlock(&g_mutex);
    pthread_cond_signal(&g_cond);
}
