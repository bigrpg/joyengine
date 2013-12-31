#include "Wrapper.h"
#include "ResultSet.h"
#include "MySqlException.h"


#include <mysql/errmsg.h>


MySql::MySql( const ConnectionInfo & connectInfo ) :
	connectInfo_(connectInfo)
	,sql_(NULL)
	,inTransaction_(false)
	,hasLostConnection_(false)
{
	
}


MySql::~MySql()
{

}

void MySql::close()
{
	if (sql_)
	{
		mysql_close( sql_ );
		sql_ = NULL;
	}
}

void MySql::connect( const ConnectionInfo & connectInfo )
{
	this->close();
	hasLostConnection_ = false;

	try
	{
		sql_ = mysql_init( 0 );

		if (!sql_)
		{
			this->throwError( sql_ );
		}

		if (!mysql_real_connect( sql_, connectInfo.host.c_str(),
				connectInfo.username.c_str(), connectInfo.password.c_str(),
				connectInfo.database.c_str(), connectInfo.port, NULL, 0 ))
		{
			this->throwError( sql_ );
		}

		// Set the client connection to be UTF8
		if (0 != mysql_set_character_set( sql_, "utf8" ))
		{
			printf( "MySql::MySql: "
				"Could not set client connection character set to UTF-8\n" );
			this->throwError( sql_ );
		}

		//this->queryCharsetLengths();
	}
	catch (std::exception& e)
	{
		//printf( "MySql::MySql: %s\n", e.what() );
		hasLostConnection_ = true;
		throw;
	}
}


void MySql::throwError( MYSQL * failedObj )
{
	DatabaseException e( failedObj );

	if (e.isLostConnection())
	{
		this->hasLostConnection( true );
	}

	throw e;
}

bool MySql::reconnectTo( const ConnectionInfo & connectInfo )
{
	this->close();

	try
	{
		this->connect( connectInfo );
	}
	catch (...)
	{
		return false;
	}

	return this->ping();
}


int MySql::realQuery( const std::string & query )
{
	int result = mysql_real_query( sql_, query.c_str(), query.length() );

	if (result == 0)
	{
		// Success
		return 0;
	}

	unsigned int errNum = mysql_errno( sql_ );

	if (!inTransaction_ &&
			((errNum == CR_SERVER_LOST) || (errNum == CR_SERVER_GONE_ERROR)))
	{
		printf( "MySql::realQuery: "
				"Connection lost. Attempting to reconnect.\n" );

		if (this->reconnect())
		{
			printf( "MySql::realQuery: Reconnect succeeded.\n" );
			result = mysql_real_query( sql_, query.c_str(), query.length() );
		}
		else
		{
			printf( "MySql::realQuery: "
					"Reconnect failed when executing %s.\n", query.c_str() );
		}
	}

	return result;
}



void MySql::execute( const std::string & queryStr, ResultSet * pResultSet )
{
	int result = this->realQuery( queryStr );

	MYSQL_RES * pMySqlResultSet = mysql_store_result( sql_ );

	if (pMySqlResultSet != NULL)
	{
		if (pResultSet != NULL)
		{
			pResultSet->setResults( pMySqlResultSet );
		}
		else
		{
			mysql_free_result( pMySqlResultSet );
		}
	}

	if (result != 0)
	{
		const size_t MAX_SIZE = 1000000;

		if (queryStr.length() < MAX_SIZE)
		{
			printf( "MySql::execute: Query failed (%d) '%s'\n",
					result, queryStr.c_str() );
		}
		else
		{
			//printf( "MySql::execute: Query failed (%d): "
			//			"Size of query string is %"PRIzu"\n",
			//		result, queryStr.length() );
		}
		this->throwError( sql_ );
	}
}
