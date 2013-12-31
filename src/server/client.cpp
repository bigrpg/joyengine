
#include "stdinc/MemoryStream.h"
#include "network/network_base_type.h"
#include "network/Helper.h"
#include "stdinc/RefCount.h"
#include "network/NetEventWaitor.h"


#include "connection/ServerInterface.h"
#define __INTERFACE__HERE__
#include "connection/ServerInterface.h"


#include "connection/ClientInterface.h"
#include "network/Nub.h"
#include "network/NetPoint.h"
#include "network/RequestManager.h"
#include "network/Handlers.h"

#include <string.h>
#include <stdlib.h>

////
namespace ClientInterface
{
	void login(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,const ClientInterface::loginArgs & args)
	{
		printf("login status:%d\n",args.status);
	}

	void sendPlayerInfo(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,const ClientInterface::sendPlayerInfoArgs & args)
	{
		printf("get rolename:[%s] from server\n",args.roleName.c_str());
	}
	
	void sendHero(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,BufferIStream& data)
	{
		
	}
   void sendUUID(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,const ClientInterface::sendUUIDArgs & args)
	{
	}
 void sendAllHero(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,const ClientInterface::sendAllHeroArgs & args)
{
}
}


#define __INTERFACE__HANDER__HERE__
#include "connection/ClientInterface.h"

#include	<stdio.h>

class LoginReplyMessageHandler : public dove::ReplyMessageHandler
{
public:
	virtual ~LoginReplyMessageHandler() {};
	virtual void handleMessage( const dove::Address & source,
		dove::UnpackedMessageHeader & header,
		BufferIStream & data,
		void * arg ) 
		{
			printf("reply ok\n");
			
			delete this;
		}

	virtual void handleException( const dove::NubException & exception,
		void * arg, dove::ReplyID replyID ) 
		{
			printf("reply timeout\n");
			delete this;
		}

};

class MyConnCB : public ConnectTimeoutCallback
{
public:
	static bool s_connected;
	virtual void onTimeout()
	{
		printf("async connect failed\n");
		delete this;
	}
	virtual void onConnected()
	{
		printf("async connect is ok....\n");
		s_connected = true;
		delete this;
	}
		
};
bool MyConnCB::s_connected = false;
	
void alarmfunc(int signo)
{
	return;
}

#define PORT 5000 
int main(int argc, char** argv)
{
  std::string ip = "127.0.0.1";

  if( argc >= 2){
  	ip = argv[1];
  }
 
  NetEventWaitor waitor_;
  waitor_.init(NULL);

#if 1
  dove::NetPoint np;
  if(np.socket(SOCK_STREAM)<0)
  	exit(-1);
  	
  //signal(SIGALRM,alarmfunc);
  //alarm(5);
    	
  printf("begin connect to %s:%d\n",ip.c_str(),PORT);
  if(np.connect(ip.c_str(),PORT)<0)
  {
  	if(errno == EINTR)
  		printf("break in alarm signal\n");
    printf("connect error: %s\n",np.error());
    exit(-1);
  }
  np.setnonblocking(true);
  	
  dove::NubPtr nub = new dove::Nub(ClientInterface::getInterfaceHolder());
  nub->setfd(np);
  np.detach();
  
  waitor_.addReadEvent(nub,false);
  
#else

	dove::NubPtr nub = new dove::Nub(ClientInterface::getInterfaceHolder());
	waitor_.connectAsync(nub,ip.c_str(),PORT,new MyConnCB,3.f);
	
	while(!MyConnCB::s_connected)
	{
		sleep(1);
		waitor_.update(0.1f);
	}
#endif

	char sendline[1024];
		printf("send msg to server -------\n");
		fgets(sendline, 4096, stdin);
    sendline[strlen(sendline)-1]='\0';
	
  while(1)
	{
 
 		dove::Bundle msg(102400);
   
    if( strcmp(sendline,"1") ==0)
    {
			//kind 1: struct message
			
			ServerInterface::msg1Args & args = ServerInterface::msg1Args::startMessage(msg);
			args.param = 1000;
			args.id = 23;
			
			//kind 2: stream struct message
			ServerInterface::loginArgs::startRequest(msg,new LoginReplyMessageHandler,NULL,5000);
			ServerInterface::loginArgs loginArgs;
			loginArgs.name = "wang";
			loginArgs.pwd = "123";
			msg << loginArgs;
	
			//msg.endFinal();
			
			//kind 3: stream message
			ServerInterface::noticeArgs::startMessage(msg);
			char * data = (char *)msg.reserve(60000);
			//write 100 of '1' into stream
			memset(data,1,1);
	
	
			//you can combine any kind of messages in any order
			ServerInterface::msg1Args & args2 = ServerInterface::msg1Args::startMessage(msg);
			args2.param = 1001;
			args2.id = 55;
		}
		
		
		
		
		//if you send message by Nub, you may not call "endRequest" explicitly
		dove::Nub::SOCKETSTATUS sret = nub->send(msg);
		
		if(sret == dove::Nub::STATUS_SENDERROR)
		{
			printf("send data error\n");
			break;
		}
		if(sret == dove::Nub::STATUS_SENDPART)
		{
			waitor_.addWriteEvent(nub);
			while(waitor_.pendingWrite())
			{
				waitor_.update(0,true);
				sleep(1);
			}
		}

		sleep(1);
		
		waitor_.update(0,true);
		
		dove::RequestManager::instance().update(0);
		
		sendline[0] = 0;
	}
	
	waitor_.fini();
    
  return 0;

}
