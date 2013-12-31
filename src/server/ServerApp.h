#ifndef __SERVERAPP__H__
#define __SERVERAPP__H__

#include "network/BaseApp.h"
#include "network/NetEventWaitor.h"
#include "network/NetPoint.h"

class ServerApp : public BaseApp, NetEventWaitorDelegete
{
public:
	ServerApp();
	virtual bool init(int argc,const char ** argv);
	virtual void fini();

	//callback
	void onReadError(dove::NubPtr ,dove::Nub::SOCKETSTATUS);
	void onAccept(dove::NetPoint&);
		
protected:
	NetEventWaitor   waitor_;
	
	virtual void update(float dTime);
};


#endif //__SERVERAPP__H__