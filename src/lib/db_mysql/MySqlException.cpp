#include "MySqlException.h"

#include <mysql/mysqld_error.h>
#include <mysql/errmsg.h>

DatabaseException::DatabaseException( MYSQL * pConnection ) :
			errStr_( mysql_error( pConnection ) ),
			errNum_( mysql_errno( pConnection ) )
{
}

DatabaseException::~DatabaseException() throw()
{
}

bool DatabaseException::shouldRetry() const
{
	return (errNum_== ER_LOCK_DEADLOCK) ||
			(errNum_ == ER_LOCK_WAIT_TIMEOUT);
}

bool DatabaseException::isLostConnection() const
{
	return (errNum_ == CR_SERVER_GONE_ERROR) ||
			(errNum_ == CR_SERVER_LOST);
}
