#ifndef __JGMSG__H__
#define __JGMSG__H__


#include "stdinc/BgTask.h"
#include "network/network_base_type.h"
#include "connection/ServerInterface.h"


//interface handlers  decl
namespace ServerInterface
{
	
	//纯结构化接口
	void msg1(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,const ServerInterface::msg1Args& args);
	//流结构化接口
	void login(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,const ServerInterface::loginArgs& args);
	//纯流接口
	void notice(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,BufferIStream& data);
	void createChar(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,const ServerInterface::createCharArgs& args);	
		
	
}
#endif //__JGMSG__H__
