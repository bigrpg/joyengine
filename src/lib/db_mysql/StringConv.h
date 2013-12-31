#ifndef __STRINGCONV__H__
#define __STRINGCONV__H__


#include "../stdinc/stdinc.h"

#include <sstream>
#include <stdexcept>
#include <stdlib.h>

namespace StringConv
{
	inline void toValue( float& value, const char * str )
	{
		char* remainder;
		value = strtof( str, &remainder );
		if (*remainder)
			throw std::runtime_error( "not a number" );
	}

	inline void toValue( double& value, const char * str )
	{
		char* remainder;
		value = strtod( str, &remainder );
		if (*remainder)
			throw std::runtime_error( "not a number" );
	}

	inline void toValue( int32& value, const char * str )
	{
		char* remainder;
		value = strtol( str, &remainder, 10 );
		if (*remainder)
			throw std::runtime_error( "not a number" );
	}


	inline void toValue( int8& value, const char * str )
	{
		int32 i;
		toValue( i, str );
		value = int8(i);
		if (value != i)
			throw std::runtime_error( "out of range" );
	}

	inline void toValue( int16& value, const char * str )
	{
		int32 i;
		toValue( i, str );
		value = int16(i);
		if (value != i)
			throw std::runtime_error( "out of range" );
	}

	inline void toValue( uint32& value, const char * str )
	{
		char* remainder;
		value = strtoul( str, &remainder, 10 );
		if (*remainder)
			throw std::runtime_error( "not a number" );
	}

	inline void toValue( uint8& value, const char * str )
	{
		uint32 ui;
		toValue( ui, str );
		value = uint8(ui);
		if (value != ui)
			throw std::runtime_error( "out of range" );
	}

	inline void toValue( uint16& value, const char * str )
	{
		uint32 ui;
		toValue( ui, str );
		value = uint16(ui);
		if (value != ui)
			throw std::runtime_error( "out of range" );
	}

	inline void toValue( int64& value, const char * str )
	{
		char* remainder;
		value = strtoll( str, &remainder, 10 );
		if (*remainder)
			throw std::runtime_error( "not a number" );
	}

	inline void toValue( uint64& value, const char * str )
	{
		char* remainder;
		value = strtoull( str, &remainder, 10 );
		if (*remainder)
			throw std::runtime_error( "not a number" );
	}

	template <class TYPE>
	inline std::string toStr( const TYPE& value )
	{
		std::stringstream ss;
		addToStream( ss, value );
		return ss.str();
	}

	template < class TYPE >
	inline void addToStream( std::ostream& strm, const TYPE& value )
	{
		strm << value;
	}

	// Special version for int8 and uint8 because they are characters but
	// we want them formatted as a number.
	inline void addToStream( std::ostream& strm, int8 value )
	{
		strm << int( value );
	}
	inline void addToStream( std::ostream& strm, uint8 value )
	{
		strm << int( value );
	}

	inline void addToStream( std::ostream& strm, float value )
	{
		strm.precision( 8 );
		strm << value;
	}

	inline void addToStream( std::ostream& strm, double value )
	{
		strm.precision( 17 );
		strm << value;
	}

	inline void addToStream( std::ostream & strm, bool value )
	{
		strm << (value ? "TRUE" : "FALSE" );
	}

} // namespace StringConv

#endif // __STRINGCONV__H__
