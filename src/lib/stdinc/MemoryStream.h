#ifndef __MEMORYSTREAM__H__
#define __MEMORYSTREAM__H__


#include "BufferStream.h"

class MemoryOStream : public BufferOStream, public BufferIStream
{
public: // constructors and destructor
	inline MemoryOStream( int size = 64 );

	virtual ~MemoryOStream();

protected:
	MemoryOStream( const MemoryOStream & );
	MemoryOStream & operator=( const MemoryOStream & );

public: // overrides from BufferOStream
	virtual void * reserve( int nBytes );
	virtual int size() const;

public: // overrides from BufferIStream
	virtual const void * retrieve( int nBytes );
	virtual int remainingLength() const;
	virtual char peek();

public: // own methods
	bool shouldDelete() const;
	void shouldDelete( bool value );
	void * data();
	void reset();
	void rewind();


protected:
	char * pBegin_;
	char * pCurr_;
	char * pEnd_;
	char * pRead_;
	bool shouldDelete_;
};

/**
 *	This class is used to stream data off a memory block.
 */
class MemoryIStream : public BufferIStream
{
public: // constructors and destructor
	MemoryIStream( const char * pData, int length ) :
		BufferIStream(),
		pCurr_( pData ),
		pEnd_( pCurr_ + length )
	{}

	MemoryIStream( const void * pData, int length ) :
		pCurr_( reinterpret_cast< const char * >( pData ) ),
		pEnd_( pCurr_ + length )
	{}

	virtual ~MemoryIStream();


public: // overrides from BufferIStream
	virtual const void * retrieve( int nBytes );

	virtual int remainingLength() const { return (int)(pEnd_ - pCurr_); }

	virtual char peek();

	virtual void finish()	{ pCurr_ = pEnd_; }

public: // new methods
	const void * data() { return pCurr_; }

protected:
	MemoryIStream() :
		pCurr_( NULL ),
		pEnd_( NULL )
	{}

	void init( const char * pData, int length )
	{
		pCurr_ = pData;
		pEnd_ = pCurr_ + length;
	}

private:
	const char * pCurr_;
	const char * pEnd_;
};


/**
 * TODO: to be documented.
 */
class MessageStream : public MemoryOStream
{
public: // constructors and destructor
	MessageStream( int size = 64 );

	virtual ~MessageStream() {}

public: // own methods
	bool addToStream( BufferOStream & stream, uint8 messageID );
	bool getMessage( BufferIStream & stream,
		int & messageID, int & length );
};

#include "MemoryStream.ipp"

#endif //__MEMORYSTREAM__H__