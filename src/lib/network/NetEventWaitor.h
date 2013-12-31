#ifndef __NETEVENTWAITOR__H__
#define __NETEVENTWAITOR__H__

#include "NetPoint.h"
#include "Helper.h"
#include "db_mysql/MySqlConnectionPool.h"
#include "stdinc/BgTask.h"
#include "stdinc/RefCount.h"
#include "Nub.h"

#include <event.h>
#include <map>

class NetEventWaitor;
class NetEventWaitorDelegete
{
public:
	virtual void onReadError(dove::NubPtr ,dove::Nub::SOCKETSTATUS)=0;
	virtual void onAccept(dove::NetPoint&){}
};

class ConnectTimeoutCallback
{
public:
	virtual ~ConnectTimeoutCallback() {}
	virtual void onTimeout() {}
	virtual void onConnected() {}
};

struct EventExtra
{
	NetEventWaitor * waitor_;
	dove::NubPtr nub_;
};


struct ConnectEventInfo
{
	dove::NubPtr nub_;
	float timeout_;
	ConnectTimeoutCallback * cb_;

	ConnectEventInfo():timeout_(0.f),cb_(0) {}

	ConnectEventInfo(const ConnectEventInfo& other)
	{
		nub_ = other.nub_;
		timeout_ = other.timeout_;
		cb_ = other.cb_;
	}
  
	ConnectEventInfo& operator = (ConnectEventInfo& other)
	{
		nub_ = other.nub_;
		timeout_ = other.timeout_;
		cb_ = other.cb_;
	}
	bool operator < (ConnectEventInfo& other)
	{
		return nub_.getObject() < other.nub_.getObject();
	}
};

inline bool operator ==(const ConnectEventInfo& lhs,const dove::NubPtr& nub)
{
	return lhs.nub_ == nub;
}

class NetEventWaitor
{
protected:
	struct event_base *base_;
	NetEventWaitorDelegete *  delege_;
	
	typedef std::map<int,struct event*>  EventMap;
	EventMap  writeEventMap_;
	EventMap  readEventMap_;
	
	typedef std::list<ConnectEventInfo>  ConnectEventRec;
	ConnectEventRec 	connectEventRec_;
	
public:
	NetEventWaitor();
	bool init(NetEventWaitorDelegete * delege);
	void fini();
	void clear();
	void update(float dTime,bool nonblock=true);
	
	void addReadEvent(dove::NubPtr nub,bool listen);
	void addWriteEvent(dove::NubPtr nub);
	void addConnectEvent(dove::NubPtr nub,ConnectTimeoutCallback * cb,float timeout);
	void delEvent(dove::NubPtr nub);
	bool pendingWrite() const { return !writeEventMap_.empty(); }
	
	bool connectAsync(dove::NubPtr nub,const char* ip,uint16 port,ConnectTimeoutCallback *cb,float timeout);
		
protected:
	void delEvent(int fd,EventMap& eventMap);
	void connectOneNub(dove::NubPtr nub,bool connected);
	
	static EventExtra * getExtra(void * c);
	static struct event * newEvent();
	static void deleteEvent(struct event * ev);

	static void _onRead(int fd, short flag, void *arg);
	static void _onWrite(int fd, short flag, void *arg);
	static void _onAccept(int fd, short flag, void *arg);
	
};

#endif //__NETEVENTWAITOR__H__
