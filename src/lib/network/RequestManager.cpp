#include "RequestManager.h"
#include "Helper.h"
#include "Handlers.h"
#include "Nub.h"

namespace dove
{

RequestManager& RequestManager::instance()
{
	static RequestManager s_inst;
	return s_inst;
}

RequestManager::RequestManager() :
nextReplyID_(1)
{

}	

RequestManager::~RequestManager()
{
}

void RequestManager::addReplyOrders( const ReplyOrders & replyOrders)
{
	for (ReplyOrders::const_iterator it = replyOrders.begin();
		it != replyOrders.end(); ++it)
	{
		const ReplyOrder& order = *it;
		//int replyID = this->getNextReplyID();
		ReplyID replyID = *order.replyID_;

		requestMap_[ replyID ] = Request();
		Request & req = requestMap_[ replyID ];
		req.handler_ = order.handler_;
		req.arg_ = order.arg_;
		req.timeout_ = getTime() + order.timeout_;
		printf("timeout:%ld\n",req.timeout_);

		// fix up the replyID in the bundle
		//*(order.replyID_) = JG_HTONL( replyID );
	}
}

void RequestManager::handleMessage( const Address & source,
		UnpackedMessageHeader & header,
		BufferIStream & data )
{
	ReplyID replyID = header.info_.replyID_;
	RequestMap::iterator it = requestMap_.find(replyID);
	if( it == requestMap_.end())
	{
		printf("replyID:%d has expired\n",replyID);
		return;
	}
	it->second.handler_->handleMessage(source,header,data,it->second.arg_);
	requestMap_.erase(it);
}
	
ReplyID RequestManager::getNextReplyID()
{
	if (nextReplyID_ >= MAXREPLYID)
		nextReplyID_ = 1;

	return nextReplyID_++;
	
}

void RequestManager::update(float dTime)
{
	RequestMap::iterator it = requestMap_.begin();
	for(;it != requestMap_.end(); )
	{
		Request & req = it->second;
		if(req.timeout_ < getTime())
		{
			req.handler_->handleException(NubException(0),req.arg_,it->first);
			RequestMap::iterator curit = it++;
			ReplyID next = 0;
			if(it == requestMap_.end())
			    requestMap_.erase(curit);
			else
			{
			    next = it->first;
			    requestMap_.erase(curit);
			}
			if(next>0)
			    it = requestMap_.find(next);
			else
			    it = requestMap_.end();
		}
		else
			++it;	
	}
}

}