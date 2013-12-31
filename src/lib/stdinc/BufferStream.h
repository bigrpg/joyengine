#ifndef __BUFFERSTREAM__H__
#define __BUFFERSTREAM__H__

#include "stdinc.h"

#include <vector>
#include <string>
#include <cstring>

const int MSG_BUFF_MAX_SIZE = 2048;

class BufferIStream;


class BufferOStream
{
public:
	virtual ~BufferOStream() {};

	/**
	 *	This method reserves the input number of bytes on this stream.
	 *
	 *	@param nBytes	Number of bytes to reserve.
	 *
	 *	@return Pointer to the reserved memory.
	 */
	virtual void * reserve( int nBytes ) = 0;

	virtual int size() const	{ return -1; }
	inline void appendString( const char * data, int length );

	void transfer( BufferIStream & from, int length );
	virtual void addBlob( const void * pBlob, int size );

	template <class TYPE>
	void insertRaw( const TYPE & t );

	inline void writePackedInt( int len );
	void writeStringLength( int len )	{ this->writePackedInt( len ); }

	static int calculatePackedIntSize( int value );
	static int calculateStreamedSizeOfString( int stringSize )
	{
		return calculatePackedIntSize( stringSize ) + stringSize;
	}
};


class BufferIStream
{
public:

	BufferIStream() { error_ = false; }
	virtual ~BufferIStream() {};

	/**
	 *	This method retrieves the input number of bytes from this stream.
	 *	The returned data is still owned by the stream, so the caller
	 *	is not responsible for freeing it.
	 *
	 *	@param nBytes	Number of bytes to retrieve.
	 *
	 *	@return Pointer to returned data.
	 */
	virtual const void * retrieve( int nBytes ) = 0;
	virtual int remainingLength() const = 0;
	virtual void finish() {}

	virtual char peek() = 0;

	template <class TYPE>
	void extractRaw( TYPE &t );

	inline int readPackedInt();
	int readStringLength()	{ return this->readPackedInt(); }

	bool error() const { return error_; }
	void error( bool error ) { error_ = error; }

	// The buffer returned from retrieve() when someone reads past the end of a
	// stream.
	static char errBuf[ MSG_BUFF_MAX_SIZE ];

protected:
	bool error_;
};

/**
 *	This convenience method is used to transfer data from a BufferIStream to
 *	this BufferOStream.
 */
inline void BufferOStream::transfer( BufferIStream & from, int length )
{
	this->addBlob( from.retrieve( length ), length );
}


/**
 *	This convenience method is used to add a block of memory to this stream.
 */
inline void BufferOStream::addBlob( const void * pBlob, int size )
{
	std::memcpy( this->reserve( size ), pBlob, size );
}


/**
 *  Insert something into the stream in raw byte order, bypassing the normal
 *  platform-specific endian conversions.
 */
template <class TYPE>
inline void BufferOStream::insertRaw( const TYPE & t )
{
	*((TYPE*)this->reserve( sizeof( TYPE ) )) = t;
}


/**
 *  Inverse operation of insertRaw() above, i.e. extract some data from the
 *  stream, bypassing endian conversions.
 */
template <class TYPE>
inline void BufferIStream::extractRaw( TYPE &t )
{
	t = *((TYPE*)this->retrieve( sizeof( TYPE ) ));
}

#include "BufferStream.ipp"

/**
 *	This method provides output streaming for a string.
 *
 *	@param b	The binary stream.
 *	@param str	The string to be streamed.
 *
 *	@return A reference to the stream that was passed in.
 */
inline BufferOStream & operator<<( BufferOStream & b, const std::string & str )
{
	b.appendString( str.data(), int(str.length()) );

	return b;
}

/**
 * 	This method provides output streaming for a char *
 *
 *	@param b	The binary stream.
 *	@param str	The string to be streamed.
 *
 *	@return A reference to the stream that was passed in.
 */
inline BufferOStream & operator<<( BufferOStream & b, char * str )
{
	b.appendString( str, int(strlen( str )) );

	return b;
}


/**
 * 	This method provides output streaming for a const char *
 *
 *	@param b	The binary stream.
 *	@param str	The string to be streamed.
 *
 *	@return A reference to the stream that was passed in.
 */
inline BufferOStream & operator<<( BufferOStream & b, const char * str )
{
	return (b << const_cast< char * >( str ) );
}


/**
 * 	This method provides output streaming for a vector.
 *
 *	@param b	The binary stream.
 *	@param data	The vector to be streamed.
 *
 *	@return A reference to the stream that was passed in.
 */
template <class T, class A>
inline BufferOStream & operator<<( BufferOStream & b,
		const std::vector<T,A> & data)
{
	uint32 max = (uint32)data.size();
	b << max;
	for (uint32 i=0; i < max; i++)
		b << data[i];
	return b;
}

/**
 *	This method puts an integer into the stream in a packed format.
 *
 *	Note: The value cannot be larger than +/- 2^23.
 *
 *	@param	value The integer value.
 */
inline void BufferOStream::writePackedInt( int value )
{
	if (value >= 255)
	{
		// As we can't represent the full 4 bytes, ensure that the value being
		// stored is within the 3 byte range.
		JG_ASSERT( value < (1<<24) );

		(*this) << (uint8)0xff;
		JG_PACK3( (char*)this->reserve( 3 ), value );
	}
	else
	{
		(*this) << (uint8)value;
	}
}


/**
 *	This static method returns the number of bytes this value will be when
 *	packed.
 */
inline int BufferOStream::calculatePackedIntSize( int value )
{
	return (value >= 0xff) ? 4 : 1;
}


/**
 *	This method gets a packed integer from the stream.
 *
 *	@return	The unpacked integer.
 */
inline int BufferIStream::readPackedInt()
{
	int	value = *(uint8*)this->retrieve( 1 );

	if (value == 0xFF)
	{	// line below is very slightly dodgy ('tho safe here)
		value = JG_UNPACK3( (const char*)this->retrieve( 3 ) );
		// ASSERT(LITTLE_ENDIAN)
	}

	return value;
}


/**
 * 	This method provides input streaming for a string.
 *
 *	@param b	The binary stream.
 *	@param str	The string to be streamed.
 *
 *	@return A reference to the stream that was passed in.
 */
inline BufferIStream & operator>>( BufferIStream & b, std::string & str )
{
	int	length = b.readPackedInt();
	char *s = (char*)b.retrieve( length );

	if (s != NULL)
		str.assign( s, length );

	return b;
}


/**
 *	This method provides input streaming for a char *.
 *
 *	@param b	The binary stream.
 *	@param str	The string to be streamed.
 *
 *	@return A reference to the stream that was passed in.
 */
inline BufferIStream & operator>>( BufferIStream & b, char * str )
{
	int	length = b.readPackedInt();
	char *s = (char*)b.retrieve( length );

	if (s != NULL)
	{
		memcpy( str, s, length );
		str[ length ] ='\0';
	}

	return b;
}


/**
 *	This method provides input streaming for a vector.
 *
 *	@param b	The binary stream.
 *	@param data	The vector to be streamed.
 *
 *	@return A reference to the stream that was passed in.
 */
template <class T, class A>
inline BufferIStream & operator>>( BufferIStream & b, std::vector<T,A> & data)
{
	T elt;
	uint32 max;

	b >> max;

	data.clear();
	data.reserve( max );
	for (uint32 i=0; i < max; i++)
	{
		b >> elt;
		data.push_back( elt );
	}
	return b;
}

/**
 *	This method appends the input string onto this stream.
 *
 *	@param data		A pointer to the character array to put on the stream.
 *	@param length	The length of the data pointed to by <i>data</i>.
 */
inline void BufferOStream::appendString( const char * data, int length )
{
	/* Note decision taken to NOT inline this fn
		(reversed temporarily to get it to compile) */
	/* Why don't we null-terminate?
		1. Costs in c_str call (altho could be avoided)
		2. std::strings can contain null characters
	*/
	this->writePackedInt( length );
	this->addBlob( data, length );
}

#endif //__BUFFERSTREAM__H__