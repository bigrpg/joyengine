#ifndef __QUERY__H__
#define __QUERY__H__

#include <string>
#include <vector>

class MySql;
class QueryHelper;
class ResultSet;


/**
 *
 */
class Query
{
public:
	Query( const std::string & stmt = "", bool shouldPartitionArgs = true );
	bool init( const std::string & stmt, bool shouldPartitionArgs = true );

	void execute( MySql & conn, ResultSet * pResults ) const;

	template <class ARG0>
	void execute( MySql & conn, const ARG0 & arg0, ResultSet * pResults ) const;

	template <class ARG0, class ARG1>
	void execute( MySql & conn,
			const ARG0 & arg0, const ARG1 & arg1,
			ResultSet * pResults ) const;

	template <class ARG0, class ARG1, class ARG2>
	void execute( MySql & conn,
			const ARG0 & arg0, const ARG1 & arg1, const ARG2 & arg2,
			ResultSet * pResults ) const;

	template <class ARG0, class ARG1, class ARG2, class ARG3>
	void execute( MySql & conn,
			const ARG0 & arg0, const ARG1 & arg1, const ARG2 & arg2, const ARG3 & arg3,
			ResultSet * pResults ) const;

	template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4>
	void execute( MySql & conn,
			const ARG0 & arg0, const ARG1 & arg1, const ARG2 & arg2, const ARG3 & arg3,
			const ARG4 & arg4,
			ResultSet * pResults ) const;

	template <class ARG0, class ARG1, class ARG2,
			 class ARG3, class ARG4, class ARG5>
	void execute( MySql & conn,
			const ARG0 & arg0, const ARG1 & arg1, const ARG2 & arg2,
			const ARG3 & arg3, const ARG4 & arg4, const ARG5 & arg5,
			ResultSet * pResults ) const;

	int numArgs() const	{ return queryParts_.size() - 1; }

	std::string getPart( int index ) const
	{
		return size_t(index) < queryParts_.size() ? queryParts_[ index ] : "";
	}

private:
	Query( const Query& );
	void operator=( const Query& );

	std::vector< std::string > queryParts_;
};


#include "QueryHelper.h"

inline void Query::execute( MySql & conn, ResultSet * pResults ) const
{
	QueryHelper runner( conn, *this );
	runner.execute( pResults );
}

template <class ARG0>
inline void Query::execute( MySql & conn, const ARG0 & arg0,
		ResultSet * pResults ) const
{
	QueryHelper runner( conn, *this );
	runner.pushArg( arg0 );
	runner.execute( pResults );
}

template <class ARG0, class ARG1>
inline void Query::execute( MySql & conn,
		const ARG0 & arg0, const ARG1 & arg1,
		ResultSet * pResults ) const
{
	QueryHelper runner( conn, *this );
	runner.pushArg( arg0 );
	runner.pushArg( arg1 );
	runner.execute( pResults );
}

template <class ARG0, class ARG1, class ARG2>
inline void Query::execute( MySql & conn,
		const ARG0 & arg0, const ARG1 & arg1, const ARG2 & arg2,
		ResultSet * pResults ) const
{
	QueryHelper runner( conn, *this );
	runner.pushArg( arg0 );
	runner.pushArg( arg1 );
	runner.pushArg( arg2 );
	runner.execute( pResults );
}

template <class ARG0, class ARG1, class ARG2, class ARG3>
inline void Query::execute( MySql & conn,
		const ARG0 & arg0, const ARG1 & arg1, const ARG2 & arg2, const ARG3 & arg3,
		ResultSet * pResults ) const
{
	QueryHelper runner( conn, *this );
	runner.pushArg( arg0 );
	runner.pushArg( arg1 );
	runner.pushArg( arg2 );
	runner.pushArg( arg3 );
	runner.execute( pResults );
}

template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4>
inline void Query::execute( MySql & conn,
		const ARG0 & arg0, const ARG1 & arg1, const ARG2 & arg2, const ARG3 & arg3,
		const ARG4 & arg4,
		ResultSet * pResults ) const
{
	QueryHelper runner( conn, *this );
	runner.pushArg( arg0 );
	runner.pushArg( arg1 );
	runner.pushArg( arg2 );
	runner.pushArg( arg3 );
	runner.pushArg( arg4 );
	runner.execute( pResults );
}

template <class ARG0, class ARG1, class ARG2,
		 class ARG3, class ARG4, class ARG5>
inline void Query::execute( MySql & conn,
		const ARG0 & arg0, const ARG1 & arg1, const ARG2 & arg2,
		const ARG3 & arg3, const ARG4 & arg4, const ARG5 & arg5,
		ResultSet * pResults ) const
{
	QueryHelper runner( conn, *this );
	runner.pushArg( arg0 );
	runner.pushArg( arg1 );
	runner.pushArg( arg2 );
	runner.pushArg( arg3 );
	runner.pushArg( arg4 );
	runner.pushArg( arg5 );
	runner.execute( pResults );
}


#endif