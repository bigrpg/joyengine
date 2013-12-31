#ifndef __MYSQLCONNECTION_POOL__H__
#define __MYSQLCONNECTION_POOL__H__

#include "DBConfig.h"

class MySql;


#include <vector>

class MySqlConnectionPool
{
	std::vector<MySql *> mysql_conns_;

protected:
	MySqlConnectionPool(){}
	bool initMySqlOneConnection(const ConnectionInfo& connectInfo);
	
public:
	bool init(const ConnectionInfo& connectInfo,int count);
	void fini();
	MySql * get();
	void give(MySql * conn);
	
	static MySqlConnectionPool & instance();

};




#endif //__MYSQLCONNECTION_POOL__H__