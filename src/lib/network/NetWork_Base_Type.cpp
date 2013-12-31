#include "network_base_type.h"
#include "stdinc/BufferStream.h"

namespace dove
{
	const Address Address::NONE( 0, 0, 0 );

	Address::Address() :
	ip_(0)
	,port_(0)
	,fd_(0)
	{
	}


	Address::Address(uint32 ip,uint16 port,int fd):
		ip_(ip)
		,port_(port)
		,fd_(fd)
	{
	}

BufferOStream& operator<<( BufferOStream &os, const Address &a )
{
	os.insertRaw( a.ip_ );
	os.insertRaw( a.port_ );
	//temp os << a.salt;

	return os;
}


BufferIStream& operator>>( BufferIStream &is, Address &a )
{
	is.extractRaw( a.ip_ );
	is.extractRaw( a.port_ );
	//temp is >> a.salt;

	return is;
}

bool operator==(const dove::Address & a, const dove::Address & b)
{
	return (a.ip_ == b.ip_) && (a.port_ == b.port_);
}

bool operator!=(const dove::Address & a, const dove::Address & b)
{
	return (a.ip_ != b.ip_) || (a.port_ != b.port_);
}

bool operator<(const dove::Address & a, const dove::Address & b)
{
	return (a.ip_ < b.ip_) || (a.ip_ == b.ip_ && (a.port_ < b.port_));
}


} //namespace dove