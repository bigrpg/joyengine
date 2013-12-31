#ifndef __QUERYHELPER__H__
#define __QUERYHELPER__H__

#include <string.h>

#include <sstream>

class MySql;
class Query;
class ResultSet;


/**
 *
 */
class QueryHelper
{
public:
	QueryHelper( MySql & conn, const Query & query );

	template <class ARG>
	void pushArg( const ARG & arg );

	void pushArg( const char * arg, int length );

	void pushArg( const std::string & arg )
	{
		this->pushArg( arg.c_str(), arg.size() );
	}

	void pushArg( const char * arg )
	{
		this->pushArg( arg, strlen( arg ) );
	}

	void execute( ResultSet * pResults ) const;

	MySql & connection() const		{ return conn_; }

private:
	const Query & query_;
	std::ostringstream stream_;
	int numArgsAdded_;
	MySql & conn_;
};


#include "Query.h"
#include "StringConv.h"

template <class ARG>
void QueryHelper::pushArg( const ARG & arg )
{
	StringConv::addToStream( stream_, arg );
	stream_ << query_.getPart( ++numArgsAdded_ );
}


#endif 