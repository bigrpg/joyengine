#ifndef __HANDLERS__H__
#define __HANDLERS__H__


class BufferIStream;

namespace dove
{
class Address;
class UnpackedMessageHeader;
class NubException;

class InputMessageHandler
{
public:
	virtual ~InputMessageHandler() {};

	/**
	 * 	This method is called by Mercury to deliver a message.
	 *
	 * 	@param source	The address at which the message originated.
	 * 	@param header	This contains the message type, size, and flags.
	 * 	@param data		The actual message data.
	 */
	virtual void handleMessage( Address & source,
		UnpackedMessageHeader & header,
		::BufferIStream & data ) = 0;
};


//Reply MessageHandler
class ReplyMessageHandler
{
public:
	virtual ~ReplyMessageHandler() {};
	virtual void handleMessage( const dove::Address & source,
		dove::UnpackedMessageHeader & header,
		BufferIStream & data,
		void * arg ) = 0;

	virtual void handleException( const dove::NubException & exception,
		void * arg, dove::ReplyID replyID ) = 0;

};


} //namespace dove

#endif //__HANDLERS__H__