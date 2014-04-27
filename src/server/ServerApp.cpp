#include "ServerApp.h"
#include "connection/ServerInterface.h"
#include "network/RequestManager.h"

#include <stdlib.h>



ServerApp::ServerApp() : BaseApp()
{
}


bool ServerApp::init(int argc,const char ** argv)
{	
	bool ret = BaseApp::init(argc,argv);
	if(!ret)
	{
		printf("init error\n");
		return false;
	}
		
	bool good = waitor_.init(static_cast<NetEventWaitorDelegete*>(this));

	int port = 50000;

  for(int i=0;i<argc-1;++i)
  {
    if(strcmp(argv[i],"-port") ==0)
    {
       port = atoi(argv[i+1]);
    }
  }	
  
  dove::NetPoint np;
  if(np.socket(SOCK_STREAM)<0)
  	return false;
  	
  if(np.setreuseaddr())
  	return false;
  	
  if(np.bind(port))
  {
  	printf("bind error\n");
  	return false;
  }
  if(np.listen())
  {
  	printf("listen error\n");
  	return false;
  }
  np.setnonblocking(true);
  
  dove::NubPtr nub = new dove::Nub(ServerInterface::getInterfaceHolder());
  nub->setfd(np);
  np.detach();
  waitor_.addReadEvent(nub,true);

  return true;
}

void ServerApp::fini()
{
	BaseApp::fini();
	waitor_.fini();
}


void ServerApp::update(float dTime)
{
	BaseApp::update(dTime);
	waitor_.update(dTime);
	dove::RequestManager::instance().update(dTime);
}

void ServerApp::onReadError(dove::NubPtr ,dove::Nub::SOCKETSTATUS status)
{
	switch(status)
	{
		case dove::Nub::STATUS_CLOSED_REMOTE:
			printf("socket closed by remote \n");
			break;
		case dove::Nub::STATUS_READERROR:
			printf("socket closed by read error\n");
			break;
		default:
			printf("socket closed by other reason\n");
	}
}

void ServerApp::onAccept(dove::NetPoint& np)
{
	dove::NubPtr nub = new dove::Nub(ServerInterface::getInterfaceHolder());
	nub->setfd(np);
 	waitor_.addReadEvent(nub,false);
	
}
