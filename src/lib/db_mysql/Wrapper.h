#ifndef __WRAPPER__H__
#define __WRAPPER__H__

#include "DBConfig.h"


#include <assert.h>
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>

class ResultSet;

class MySql
{
public:
	MySql( const ConnectionInfo & connectInfo );
	~MySql();
	

	MYSQL * get() { return sql_; }
	
	void close();	
	bool reconnectTo( const ConnectionInfo & connectInfo );
	bool reconnect()
	{
		return this->reconnectTo( connectInfo_ );
	}
	bool ping()					{ return mysql_ping( sql_ ) == 0; }
		
	const char* info()			{ return mysql_info( sql_ ); }
	const char* getLastError()	{ return mysql_error( sql_ ); }
	unsigned int getLastErrorNum() { return mysql_errno( sql_ ); }

	void inTransaction( bool value )
	{
		assert( inTransaction_ != value );
		inTransaction_ = value;
	}

	bool hasLostConnection() const		{ return hasLostConnection_; }
	void hasLostConnection( bool v )	{ hasLostConnection_ = v; }

	void execute( const std::string & queryStr, ResultSet * pResultSet =0 );

	
private:
	void throwError( MYSQL * pConnection );
	int realQuery( const std::string & query );
	void connect( const ConnectionInfo & connectInfo );

protected:
	MYSQL * sql_;
	bool inTransaction_;
	bool hasLostConnection_;

	ConnectionInfo connectInfo_;

};



#endif
