#include "NetPoint.h"
#include <string.h>


namespace dove
{

NetPoint::NetPoint() : socket_(NOSOCKET)
{}

NetPoint::~NetPoint()
{
	close();
}

void NetPoint::setfd(int fd)
{
	socket_ = fd;
}

int NetPoint::detach()
{
	int cur = socket_;
	setfd(NOSOCKET);
	return cur;
}

int NetPoint::socket( int type )
{
	setfd( ::socket( AF_INET, type, 0 ) );
	if(socket_<0)
	{
		 printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
		 return socket_;
	}
	return socket_;
}	

int NetPoint::setnonblocking( bool nonblocking )
{
  int flags;
  
  flags = fcntl(socket_, F_GETFL);
  if (flags < 0)
      return flags;
  if(nonblocking)
 		flags |= O_NONBLOCK;
 	else
 		flags &= ~O_NONBLOCK;
 		
	return fcntl(socket_, F_SETFL, flags);
}


int NetPoint::connect(uint32 networkAddr, uint16 networkPort)
{
	sockaddr_in	sin;
	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = networkPort;
	sin.sin_addr.s_addr = networkAddr;
	
	return ::connect( socket_, (sockaddr*)&sin, sizeof(sin) );
	
}

int NetPoint::connect( const char * networkAddr, uint16 networkPort)
{
	sockaddr_in addr;
	inet_pton(AF_INET, networkAddr, &addr.sin_addr);
	return connect(addr.sin_addr.s_addr,htons(networkPort));
}

int NetPoint::bind( uint16 networkPort, uint32 networkAddr)
{
	sockaddr_in	sin;
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(networkPort);
	sin.sin_addr.s_addr = networkAddr;
	return ::bind( socket_, (struct sockaddr*)&sin, sizeof(sin) );

}

int NetPoint::setreuseaddr()
{
	if( socket_ == NOSOCKET)
		return 0;

	int reuseaddr_on = 1;
  return setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on,
                 sizeof(reuseaddr_on));

}

int NetPoint::listen( int backlog )
{
	return ::listen( socket_, backlog );
	
}

int NetPoint::close()
{
	if( socket_ == NOSOCKET)
		return 0;
		
	printf("close socket\n");
	int ret = ::close(socket_);
	if( ret == 0)
		setfd(NOSOCKET);
	return ret;
}

int NetPoint::send( const void * data, int size )
{
	return ::send(socket_,(char*)data,size,0);
}

int NetPoint::recv( void * data, int size )
{
	return ::recv(socket_,(char*)data,size,0);
}

const char * NetPoint::error()
{
	return strerror(errno);
}


} //end namespace dove
