#include "Transaction.h"
#include "MySqlException.h"
#include "Wrapper.h"


#include <assert.h>
#include <stdio.h>


MySqlTransaction::MySqlTransaction( MySql& sql ) :
	sql_( sql ),
	committed_( false )
{
	sql_.execute( "START TRANSACTION" );
	sql_.inTransaction( true );
}

MySqlTransaction::~MySqlTransaction()
{
	if (!committed_ && !sql_.hasLostConnection())
	{
		try
		{
			printf( "MySqlTransaction::~MySqlTransaction: Rolling back\n" );
			sql_.execute( "ROLLBACK" );
		}
		catch (DatabaseException & e)
		{
			if (e.isLostConnection())
			{
				sql_.hasLostConnection( true );
			}
		}
	}

	sql_.inTransaction( false );
}


/**
 *
 */
bool MySqlTransaction::shouldRetry() const
{
	return (sql_.getLastErrorNum() == ER_LOCK_DEADLOCK);
}


/**
 *
 */
void MySqlTransaction::commit()
{
	assert( !committed_ );
	sql_.execute( "COMMIT" );
	committed_ = true;
}
