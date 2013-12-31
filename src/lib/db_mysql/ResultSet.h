#ifndef  __MYSQLRESULTSET__H__
#define __MYSQLRESULTSET__H__

#include "../stdinc/ValueOrNull.h"

#include <mysql/mysql.h>
#include <sstream>

inline
bool getValueFromString( const char * str, int len, std::string & value )
{
	value.assign( str, len );

	return true;
}

template <class VALUE_TYPE>
bool getValueFromString( const char * str, int len, VALUE_TYPE & value )
{
	if (!str)
	{
		return false;
	}

	std::stringstream stream;
	stream.write( str, std::streamsize( len ) );
	stream.seekg( 0, std::ios::beg );
	stream >> value;

	return !stream.bad();
}

inline bool getValueFromString( const char * str, int len, uint8 & value )
{
	int result;

	if (!getValueFromString( str, len, result ))
	{
		return false;
	}

	value = result;
	return true;
}

inline bool getValueFromString( const char * str, int len, int8 & value )
{
	int result;

	if (!getValueFromString( str, len, result ))
	{
		return false;
	}

	value = result;
	return true;
}

template <class VALUE_TYPE>
bool getValueFromString( const char * str, int len,
		ValueOrNull< VALUE_TYPE > & value )
{
	if (!str)
	{
		value.setNull();
		return true;
	}

	VALUE_TYPE v;

	if (getValueFromString( str, len, v ))
	{
		value.setValue( v );
		return true;
	}

	return false;
}

#define RESULT_SET_GET_RESULT( ARG_COUNT )									\
		if (this->numFields() != ARG_COUNT)									\
		{																	\
			printf( "ResultSet::getResult: Expected %d. Got %d\n",		\
				ARG_COUNT, this->numFields() );								\
			return false;													\
		}																	\
																			\
		MYSQL_ROW row = mysql_fetch_row( pResultSet_ );						\
																			\
		if (!row)															\
		{																	\
			return false;													\
		}																	\
																			\
		unsigned long * lengths = mysql_fetch_lengths( pResultSet_ );		\


/////////////////////////////////////////////


class ResultSet
{
protected:
		MYSQL_RES  * pResultSet_;
		friend class ResultRow;
		
public:
	ResultSet();
	~ResultSet();
	
	void setResults( MYSQL_RES * result);
	int numRows() const;
	
	template <class ARG0>
	bool getResult( ARG0 & arg0 )
	{
		RESULT_SET_GET_RESULT( 1 );
		return getValueFromString( row[0], lengths[0], arg0 );
	}

	template <class ARG0, class ARG1>
	bool getResult( ARG0 & arg0, ARG1 & arg1 )
	{
		RESULT_SET_GET_RESULT( 2 );
		return getValueFromString( row[0], lengths[0], arg0 ) &&
			getValueFromString( row[1], lengths[1], arg1 );
	}

	template <class ARG0, class ARG1, class ARG2>
	bool getResult( ARG0 & arg0, ARG1 & arg1, ARG2 & arg2 )
	{
		RESULT_SET_GET_RESULT( 3 );
		return getValueFromString( row[0], lengths[0], arg0 ) &&
			getValueFromString( row[1], lengths[1], arg1 ) &&
			getValueFromString( row[2], lengths[2], arg2 );
	}
	template <class ARG0, class ARG1, class ARG2, class ARG3>
	bool getResult( ARG0 & arg0, ARG1 & arg1,
			ARG2 & arg2, ARG3 & arg3 )
	{
		RESULT_SET_GET_RESULT( 4 );
		return getValueFromString( row[0], lengths[0], arg0 ) &&
			getValueFromString( row[1], lengths[1], arg1 ) &&
			getValueFromString( row[2], lengths[2], arg2 ) &&
			getValueFromString( row[3], lengths[3], arg3 );
	}
	template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4>
	bool getResult( ARG0 & arg0, ARG1 & arg1,
			ARG2 & arg2, ARG3 & arg3, ARG4 & arg4 )
	{
		RESULT_SET_GET_RESULT( 5 );
		return getValueFromString( row[0], lengths[0], arg0 ) &&
			getValueFromString( row[1], lengths[1], arg1 ) &&
			getValueFromString( row[2], lengths[2], arg2 ) &&
			getValueFromString( row[3], lengths[3], arg3 ) &&
			getValueFromString( row[4], lengths[4], arg4 );
	}
	template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5>
	bool getResult( ARG0 & arg0, ARG1 & arg1,
			ARG2 & arg2, ARG3 & arg3, ARG4 & arg4, ARG5 & arg5 )
	{
		RESULT_SET_GET_RESULT( 6 );
		return getValueFromString( row[0], lengths[0], arg0 ) &&
			getValueFromString( row[1], lengths[1], arg1 ) &&
			getValueFromString( row[2], lengths[2], arg2 ) &&
			getValueFromString( row[3], lengths[3], arg3 ) &&
			getValueFromString( row[4], lengths[4], arg4 ) && 
			getValueFromString( row[5], lengths[5], arg5 );
	}
	template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6>
	bool getResult( ARG0 & arg0, ARG1 & arg1,
			ARG2 & arg2, ARG3 & arg3, ARG4 & arg4, ARG5 & arg5, ARG6 & arg6 )
	{
		RESULT_SET_GET_RESULT( 7 );
		return getValueFromString( row[0], lengths[0], arg0 ) &&
			getValueFromString( row[1], lengths[1], arg1 ) &&
			getValueFromString( row[2], lengths[2], arg2 ) &&
			getValueFromString( row[3], lengths[3], arg3 ) &&
			getValueFromString( row[4], lengths[4], arg4 ) && 
			getValueFromString( row[5], lengths[5], arg5 ) &&
			getValueFromString( row[6], lengths[6], arg6 );
	}
	template <class ARG0, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7>
	bool getResult( ARG0 & arg0, ARG1 & arg1,
			ARG2 & arg2, ARG3 & arg3, ARG4 & arg4, ARG5 & arg5, ARG6 & arg6, ARG7 & arg7 )
	{
		RESULT_SET_GET_RESULT( 8 );
		return getValueFromString( row[0], lengths[0], arg0 ) &&
			getValueFromString( row[1], lengths[1], arg1 ) &&
			getValueFromString( row[2], lengths[2], arg2 ) &&
			getValueFromString( row[3], lengths[3], arg3 ) &&
			getValueFromString( row[4], lengths[4], arg4 ) && 
			getValueFromString( row[5], lengths[5], arg5 ) &&
			getValueFromString( row[6], lengths[6], arg6 ) &&
			getValueFromString( row[7], lengths[7], arg7 );
	}

	int numFields() const
	{
		return pResultSet_ ? mysql_num_fields( pResultSet_ ) : 0;
	}

	bool hasResult() const	{ return pResultSet_ != NULL; }
};


//this class is for one row data from mysql
class ResultRow
{
public:
	ResultRow() :
		row_( NULL ),
		lengths_( NULL ),
		numFields_( 0 )
	{}

	bool fetchNextFrom( ResultSet & resultSet )
	{
		MYSQL_RES * pResultSet = resultSet.pResultSet_;

		row_ = mysql_fetch_row( pResultSet );

		if (!row_)
		{
			return false;
		}

		lengths_ = mysql_fetch_lengths( pResultSet );
		numFields_ = mysql_num_fields( pResultSet );

		return true;
	}

	template <class TYPE>
	bool getField( int fieldNum, TYPE & value ) const
	{
		if (fieldNum >= numFields_)
		{
			printf( "ResultRow::getField: "
					"Wanted field %d when there are only %d.\n",
				fieldNum, numFields_ );
			return false;
		}

		return getValueFromString( row_[ fieldNum ], lengths_[ fieldNum ],
				value );
	}

	int numFields() const				{ return numFields_; }

private:
	MYSQL_ROW row_;
	unsigned long * lengths_;
	int numFields_;
};


//wrapper class to use '>>' to put value into variable
class ResultStream
{
public:
	ResultStream( ResultRow & resultRow ) :
		resultRow_( resultRow ),
		currField_( 0 ),
		error_( false )
	{
	}

	template <class TYPE>
	bool getNextField( TYPE & value )
	{
		bool result = resultRow_.getField( currField_, value );

		++currField_;
		error_ |= !result;

		return result;
	}

	bool error() const			{ return error_; }
	void setError()				{ error_ = true; }

	bool eof() const			{ return !error_ &&
									(currField_ == resultRow_.numFields()); }

private:
	ResultRow & resultRow_;
	int currField_;
	bool error_;
};

template <class TYPE>
ResultStream & operator>>( ResultStream & stream, TYPE & v )
{
	stream.getNextField( v );

	return stream;
}

#endif
