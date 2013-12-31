#ifndef __NETWORK_BASETYPES_H__
#define __NETWORK_BASETYPES_H__

#include "stdinc/stdinc.h"
#include "stdinc/RefCount.h"


#include <vector>

#define DEFAULT_REQUEST_TIMEOUT  5			//Request
#define COMP_MESSAGE_ID	255							//ID
#define MAXREPLYID   65535							//
#define NOSOCKET 	-1

class BufferIStream;
class BufferOStream;

namespace dove
{
	
typedef uint8 	MessageID;
typedef uint16	ReplyID;
typedef int Reason;
class Nub;

class Address
{
public:

	Address();
	Address( uint32 ip, uint16 port ,int fd);

	uint32	ip_;
	uint16	port_;
	int fd_;
	
	static const Address NONE;
};
bool operator==(const Address & a, const Address & b);
bool operator!=(const Address & a, const Address & b);
bool operator<(const Address & a, const Address & b);

BufferIStream& operator>>( BufferIStream &in, Address &a );
BufferOStream& operator<<( BufferOStream &out, const Address &a );


#pragma pack(push, 1)
class CompMessageHeader
{
	public:
		ReplyID	replyID_;
		uint16	flag_;
		
		CompMessageHeader() : replyID_(0),flag_(0)
		{}
};
#pragma pack( pop )
	
class UnpackedMessageHeader
{
	public:
		MessageID id_;
		int length_;
		Nub* nub_;
		
		CompMessageHeader info_;
		
		UnpackedMessageHeader(dove::MessageID id,int length) : id_(id),length_(length)
			{}
};


enum
{
	MSGHEAD_FLAG_REQUEST 	= 1<<0,
	MSGHEAD_FLAG_REPLY		= 1<<1,
};

} //namespace dove

#endif //__NETWORK_BASETYPES_H__