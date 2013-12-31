#ifndef __NETPOINT__H__
#define __NETPOINT__H__

#include "stdinc/stdinc.h"
#include "network_base_type.h"
#include "Helper.h"

namespace dove
{
	
class NetPoint
{
public:
	NetPoint();
	~NetPoint();
	
	operator int() const
	{
		return socket_;
	}

	void setfd(int fd);	
	int socket( int type );
	
	int setnonblocking( bool nonblocking );
	
	int connect( uint32 networkAddr, uint16 networkPort);
	int connect( const char * networkAddr, uint16 networkPort);
	int bind( uint16 networkPort = 0 ,uint32 networkAddr = INADDR_ANY);
	int listen( int backlog = 5 );
	int setreuseaddr();

	int send( const void * data, int size );
	int recv( void * data, int size );

	int close();
	
	const char * error();
	int detach();
	
protected:
	int socket_;

};


} //end namespace dove

#endif //__NETPOINT__H__