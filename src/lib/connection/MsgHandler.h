#ifndef __MSGHANDER__H__
#define __MSGHANDER__H__

#include "network/Handlers.h"


//message handler for struct msg only
template <class ARGS>
class StructMessageHandler : public dove::InputMessageHandler
{
public:
	typedef void (*Handler)(dove::Address & srcAddr, const dove::UnpackedMessageHeader & header,const ARGS & args );

	StructMessageHandler( Handler handler ) : handler_( handler ) {}

private:
	virtual void handleMessage( dove::Address & srcAddr,
			dove::UnpackedMessageHeader & header,
			::BufferIStream & data )
	{
#ifndef _BIG_ENDIAN
		ARGS & args = *(ARGS*)data.retrieve( sizeof(ARGS) );
#else
		ARGS args;
		data >> args;
#endif
		(*handler_)( srcAddr,header,args );
	}

	Handler handler_;
};


//message handler for stream msg which has MERCURY_ISTREAM and MERCURY_OSTREAM
template <class ARGS>
class StreamStructMessageHandler : public dove::InputMessageHandler
{
public:
	typedef void (*Handler)(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header, const ARGS & args );

	StreamStructMessageHandler( Handler handler ) : handler_( handler ) {}

private:
	virtual void handleMessage( dove::Address & srcAddr,
			dove::UnpackedMessageHeader & header,
			::BufferIStream & data )
	{
		ARGS args;
		data >> args;
		(*handler_)( srcAddr,header,args );
	}

	Handler handler_;
};


//message handler for stream msg only
class StreamMessageHandler : public dove::InputMessageHandler
{
public:
	typedef void (*Handler)(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header, BufferIStream & data );

	StreamMessageHandler( Handler handler ) : handler_( handler ) {}

private:
	virtual void handleMessage( dove::Address & srcAddr,
			dove::UnpackedMessageHeader & header,
			::BufferIStream & data )
	{
		(*handler_)( srcAddr,header,data );
	}

	Handler handler_;
};



#endif //__MSGHANDER__H__