#include "BaseApp.h"
#include "network/Helper.h"
#include "db_mysql/MySqlConnectionPool.h"
#include "stdinc/BgTask.h"


#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FRAME 10
#define MAX_MYSQL_CONN	20
#define MAX_THREAD_POOL 20

BaseApp::BaseApp() : exit_(0)
{
}

bool BaseApp::init(int argc,const char ** argv)
{
	if(argc<4)
	{
		printf("usage: -port xxxx  -mysqldb  dbname  -mysqlpwd  dbpwd\n");
		return false;
	}
	
	ConnectionInfo  connectInfo;
	connectInfo.host = "127.0.0.1";
	connectInfo.port = MYSQL_PORT;
	connectInfo.username = "root";

	for(int i=0;i<argc-1;++i)
	{
		if(strcmp(argv[i],"-mysqldb") ==0)
		{
			connectInfo.database = argv[i+1];
			++i;
		}
		else if(strcmp(argv[i],"-mysqlpwd") == 0)
		{
			connectInfo.password = argv[i+1];
			++i;
		}
	}
	if(!MySqlConnectionPool::instance().init(connectInfo,MAX_MYSQL_CONN))
	{
		printf("init mysql pool error.\n");
		return false;
	}
	if(!WorkThreadManager::instance().init(MAX_THREAD_POOL))
	{
		printf("init thread pool error.\n");
		return false;
	}
	return true;
}

void BaseApp::fini()
{
	WorkThreadManager::instance().fini();
	MySqlConnectionPool::instance().fini();
}

int BaseApp::run()
{
	WorkThreadManager & workThreadMgr = WorkThreadManager::instance();
	while(!exit_)
	{
		static time_t dTime = 0;
		time_t lastTime = getTime();
		
		//millisecond
		update(dTime);
					
		time_t nowTime = getTime();
		time_t delta = nowTime - lastTime;
		if(delta < 1000/MAX_FRAME)
		{
			usleep(1000*(1000/MAX_FRAME - delta));
		}
		dTime = getTime() - lastTime;
	}
	return exit_;
}

void BaseApp::update(float dTime)
{
	WorkThreadManager::instance().tick(dTime);
}
