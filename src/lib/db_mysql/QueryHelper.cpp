#include "QueryHelper.h"
#include "Wrapper.h"

#include <assert.h>

QueryHelper::QueryHelper( MySql & conn, const Query & query ) :
	query_( query ),
	stream_(),
	numArgsAdded_( 0 ),
	conn_( conn )
{
	stream_ << query.getPart( 0 );
}

void QueryHelper::execute( ResultSet * pResults ) const
{
	assert( numArgsAdded_ == query_.numArgs() );
	std::string queryStr = stream_.str();
	conn_.execute( queryStr, pResults );
}

void QueryHelper::pushArg( const char * arg, int length )
{
	if (length < 1024)
	{
		char buffer[ 2048 ];
		mysql_real_escape_string( conn_.get(), buffer, arg, length );
		stream_ << '\'' << buffer << '\'';
	}
	else
	{
		char * buffer = new char[ 1 + 2*length ];
		mysql_real_escape_string( conn_.get(), buffer, arg, length );
		stream_ << '\'' << buffer << '\'';
		delete [] buffer;
	}

	stream_ << query_.getPart( ++numArgsAdded_ );
}
