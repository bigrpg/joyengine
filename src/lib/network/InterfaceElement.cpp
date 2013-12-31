#include "InterfaceElement.h"
#include "stdinc/BufferStream.h"
#include "network/Handlers.h"

namespace dove
{

//message handler for Comp Message 
//it only extra UnpackedMessageHeader
class CompMessageHandler : public dove::InputMessageHandler
{
private:
	virtual void handleMessage( dove::Address & srcAddr,
			dove::UnpackedMessageHeader & header,
			::BufferIStream & data )
	{
		data >> header.info_;
		printf("called by compmmsg\n");
	}
};
static CompMessageHandler s_compMsgHandler;
InterfaceElement InterfaceElement::s_compIE("CompIE",COMP_MESSAGE_ID,VARIABLE_LENGTH_MESSAGE,2,&s_compMsgHandler);

InterfaceElement::InterfaceElement( const char * name, MessageID id,
		int8 lengthStyle, int lengthParam,
		dove::InputMessageHandler * pHandler ) :
	id_( id ),
	lengthStyle_( lengthStyle ),
	lengthParam_( lengthParam ),
	name_( name ),
	pHandler_( pHandler )
{
}

int InterfaceElement::headerSize() const
{
	int headerLen = sizeof( MessageID );
	switch (lengthStyle_)
	{
		case FIXED_LENGTH_MESSAGE:
			break;
		case VARIABLE_LENGTH_MESSAGE:
			headerLen += lengthParam_;
			break;
		default:
			headerLen = -1;
			break;
	}
	return headerLen;
}

int InterfaceElement::nominalBodySize() const
{
	// never guesses for variable-length messages
	return (lengthStyle_ == FIXED_LENGTH_MESSAGE) ? lengthParam_ : 0;
}

const char * InterfaceElement::c_str() const
{
	static char buf[ 256 ];
	snprintf( buf, sizeof( buf ), "%s/%d", name_, id_ );
	return buf;
}

void InterfaceElement::packLength(int length, BufferOStream& stream) const
{
	if(lengthStyle_ == FIXED_LENGTH_MESSAGE)
		return;

	char * data = (char *)stream.reserve(lengthParam_);
	packLength(length,data);
}		
		


void InterfaceElement::packLength(int length, char * data) const
{
	if(lengthStyle_ == FIXED_LENGTH_MESSAGE)
		return;
			
	if(	lengthParam_ == 1)
	{
		JG_ASSERT(length<255);
		*data = (uint8)length;
	}
	else if(lengthParam_ == 2)
	{
		JG_ASSERT(length<65535);
		*((uint16*)data) = JG_HTONS( (uint16)length );
	}
//	else if(lengthParam_ == 4)
//	{
//		JG_ASSERT(length<int(0x7fffffff));
//		*((uint32*)data) = JG_HTONL( (uint32)length );
//	}
	
}

int InterfaceElement::unpackLength(BufferIStream& stream) const
{
	if(lengthStyle_ == FIXED_LENGTH_MESSAGE)
		return 0;
		
	if(stream.remainingLength() < lengthParam_)
		return -1;
		
	const char * data = (const char *)stream.retrieve(lengthParam_);
	
	int len = 0;
	if(	lengthParam_ == 1)
	{
		len =  int(uint8(*data));
	}
	else if(lengthParam_ == 2)
	{
		len = JG_NTOHS( *(uint16*)data );	
	}
//	else if(lengthParam_ == 4)
//	{
//		len = JG_NTOHL( *(uint32*)data );	
//	}
	else
	{
		JG_ASSERT(0);
	}
	return len;
}

} //dove