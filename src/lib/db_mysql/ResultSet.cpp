#include "ResultSet.h"

#include <stdio.h>


ResultSet::ResultSet() :
	pResultSet_(NULL)
{
	
}

ResultSet::~ResultSet()
{
	setResults( NULL );
}

void ResultSet::setResults( MYSQL_RES * result)
{
	if (pResultSet_ != NULL)
	{
		mysql_free_result( pResultSet_ );
	}

	pResultSet_ = result;
}

int ResultSet::numRows() const
{
	return pResultSet_ ? mysql_num_rows( pResultSet_ ) : 0;
}