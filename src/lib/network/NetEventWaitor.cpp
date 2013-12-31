#include "NetEventWaitor.h"

#include <stdlib.h>
#include <algorithm>


#define EXTRACTARGS \
	struct event* ev = (struct event*)arg;	\
    (void)ev;\
	EventExtra * extra = getExtra(arg);	\
	NetEventWaitor * waitor = extra->waitor_;	\
	dove::NubPtr nub = extra->nub_;	\


NetEventWaitor::NetEventWaitor() :
base_(NULL)
{}


bool NetEventWaitor::init(NetEventWaitorDelegete * delege)
{
	delege_ = delege;
	
	base_ = event_base_new();
	if(!base_)
	{
	   printf("create libevent error\n");
	   return false; 
	}
	
	return true;
}

void NetEventWaitor::fini()
{
    clear();
	event_base_free(base_);
	base_ = NULL;
	delege_ = NULL;
}

void NetEventWaitor::clear()
{
 	EventMap::iterator it = readEventMap_.begin();
	EventMap::iterator end = readEventMap_.begin();
	for(;it != end;++it)
	{
		close(it->first);
		event_del(it->second);
		deleteEvent(it->second);
	}
	readEventMap_.clear();
	
	it = writeEventMap_.begin();
	end = writeEventMap_.begin();
	for(;it != end;++it)
	{
		event_del(it->second);
		deleteEvent(it->second);
	}
	writeEventMap_.clear();   
}

void NetEventWaitor::update(float dTime,bool nonblock)
{
	event_base_loop(base_,nonblock ? EVLOOP_NONBLOCK : 0);
		
	ConnectEventRec::iterator it = connectEventRec_.begin();
	for(;it != connectEventRec_.end();)
	{
		ConnectEventInfo& rec = *it;
		rec.timeout_ -= dTime;
		if(rec.timeout_ <0.f)
		{
			rec.cb_->onTimeout();
			rec.cb_ = NULL;
			delEvent(rec.nub_);
			it = connectEventRec_.erase(it);	
			continue;
		}
		++it;
	}
}

void NetEventWaitor::addReadEvent(dove::NubPtr nub,bool listen)
{
	EventMap::iterator found = readEventMap_.find(nub->getfd());
	if( found != readEventMap_.end())
		return;

	struct event * ev = newEvent();
	EventExtra * extra = getExtra(ev);
	extra->waitor_ = this;
	extra->nub_ = nub;
	
	int flag = EV_READ|EV_PERSIST;
	event_assign(ev,base_,nub->getfd(),flag, listen ? _onAccept : _onRead,ev);
    event_add(ev,NULL);
	readEventMap_[nub->getfd()] = ev;
	
	printf("addReadEvent\n");
}

void NetEventWaitor::addWriteEvent(dove::NubPtr nub)
{
	EventMap::iterator found = writeEventMap_.find(nub->getfd());
	if( found != writeEventMap_.end())
		return;
	struct event * ev = newEvent();
	EventExtra * extra = getExtra(ev);
	extra->waitor_ = this;
	extra->nub_ = nub;
	
	int flag = EV_WRITE|EV_PERSIST;
	event_assign(ev,base_,nub->getfd(),flag, _onWrite,ev);
    event_add(ev,NULL);
	writeEventMap_[nub->getfd()] = ev;
	
	printf("addWriteEvent\n");
}

void NetEventWaitor::addConnectEvent(dove::NubPtr nub,ConnectTimeoutCallback * cb,float timeout)
{
	ConnectEventRec::iterator found = std::find(connectEventRec_.begin(),connectEventRec_.end(),nub);
	if(found != connectEventRec_.end())
		return;
		
	addReadEvent(nub,false);
	addWriteEvent(nub);
	
	connectEventRec_.push_back(ConnectEventInfo());
	ConnectEventInfo& info = connectEventRec_.back();
	info.nub_ = nub;
	info.timeout_ = timeout;
	info.cb_ = cb;
	
	printf("add async connect\n");
	
}
void NetEventWaitor::delEvent(dove::NubPtr nub)
{
	delEvent(nub->getfd(),readEventMap_);
	delEvent(nub->getfd(),writeEventMap_);	
}

////  callback function
void NetEventWaitor::_onRead(int fd, short flag, void *arg)
{
	printf("onRead....\n");
	EXTRACTARGS
	
	dove::NetPoint np;
	np.setfd(fd);
	
	dove::Nub::SOCKETSTATUS ret = nub->onRead();
	
	if( ret == dove::Nub::STATUS_CLOSED_REMOTE || ret == dove::Nub::STATUS_READERROR)
	{
		if(waitor->delege_)
			waitor->delege_->onReadError(nub,ret);
		waitor->delEvent(fd,waitor->readEventMap_);
		waitor->delEvent(fd,waitor->writeEventMap_);
		return;
	}
  else if(ret == dove::Nub::SOCKET_CONNECTED)
  {
  	waitor->connectOneNub(nub,true);
  	np.detach();
  	return;
  }
  else if(ret == dove::Nub::SOCKET_INVALID)
  {
  	waitor->connectOneNub(nub,false);
  	return;
  }
	np.detach();
}
void NetEventWaitor::_onAccept(int fd, short flag, void *arg)
{
	EXTRACTARGS
	
	/* 接受新的连接 */
	int client_fd = nub->onAccept();
	if (client_fd == -1) {
    printf("accept failed");
    return;
	}
	dove::NetPoint np;
	np.setfd(client_fd);
	np.setnonblocking(true);
	
	
	if(waitor->delege_)
	{
		waitor->delege_->onAccept(np);
	}
	np.detach();		
}

void NetEventWaitor::_onWrite(int fd, short flag, void *arg)
{
	EXTRACTARGS
	
	printf("onWrite\n");
	
	dove::NetPoint np;
	np.setfd(fd);

	dove::Nub::SOCKETSTATUS ret = nub->onWrite();	 //发送上次没发完的数据
	if(ret == dove::Nub::STATUS_SENDALL) 
	{
    waitor->delEvent(fd,waitor->writeEventMap_);
    np.detach();
  }
  else if(ret == dove::Nub::SOCKET_CONNECTED)
  {
  	waitor->connectOneNub(nub,true);
  	np.detach();
  }
  else if(ret == dove::Nub::SOCKET_INVALID)
  {
  	waitor->connectOneNub(nub,false);
  }
}

//internal helper function
void NetEventWaitor::delEvent(int fd,EventMap& eventMap)
{
	EventMap::iterator found = eventMap.find(fd);
	if(found != eventMap.end())
	{
		struct event * ev = found->second;
		event_del(ev);
		deleteEvent(ev);
		eventMap.erase(found);
	}
}

EventExtra * NetEventWaitor::getExtra(void * c)
{
	return (EventExtra*)(((char*)c)+event_get_struct_event_size());
}

struct event * NetEventWaitor::newEvent()
{
	int size = event_get_struct_event_size();
	struct event * ev = (struct event*)malloc(size+sizeof(EventExtra));
	new(getExtra(ev)) EventExtra;
	return ev;
}

void NetEventWaitor::deleteEvent(struct event * ev)
{
	EventExtra * extra = getExtra(ev);
	extra->~EventExtra();	
}

void NetEventWaitor::connectOneNub(dove::NubPtr nub,bool connected)
{
	ConnectEventRec::iterator found = std::find(connectEventRec_.begin(),connectEventRec_.end(),nub);
	assert(found != connectEventRec_.end());
	ConnectEventInfo& rec = *found;
    	ConnectTimeoutCallback * cb = rec.cb_;
	rec.cb_ = NULL;
	int fd = nub->getfd();
	connectEventRec_.erase(found);	
	delEvent(fd,writeEventMap_);
	if(!connected)
	{
		nub->close();
		delEvent(fd,readEventMap_);
        cb->onTimeout();
	}
    else
    {
        cb->onConnected();
    }
}

//ret value:
//true: start to async connect
//false: failed to start
bool NetEventWaitor::connectAsync(dove::NubPtr nub,const char* ip,uint16 port,ConnectTimeoutCallback *cb,float timeout)
{
  dove::NetPoint np;
  if(np.socket(SOCK_STREAM)<0)
  	return false;
   
  bool succ = np.setnonblocking(true) != -1;
  if(!succ)
  	return false;
 	
  printf("begin async connect to %s:%d\n",ip,port);
  int ret = np.connect(ip,port);
  if(ret ==0)
  {
  	printf("async connect succ directly\n");
  	nub->setfd(np);
  	np.detach();
  	addReadEvent(nub,false);
  	cb->onConnected();
  	return true;
  }
  if(errno != EINPROGRESS)
  {
    printf("failed to start async connect: %s\n",np.error());
    return false;
  }
  	
  nub->setfd(np,dove::Nub::SOCKET_CONNECTING);
  np.detach();
  addConnectEvent(nub,cb,timeout);
  return true;
}
