#ifndef __REQUESTMANAGER__H__
#define __REQUESTMANAGER__H__


#include "Bundle.h"


#include <map>

namespace dove
{
typedef std::vector< ReplyOrder > ReplyOrders;
class ReplyMessageHandler;


class Request
{
public:
	ReplyMessageHandler * handler_;
	void * arg_;
	time_t timeout_;
		
};
/**
 *
 */
class RequestManager 
{
public:
	~RequestManager();
	
	static RequestManager& instance();

	void addReplyOrders( const ReplyOrders & replyOrders);
	ReplyID getNextReplyID();
	void update(float dTime);

private:
	RequestManager();


	virtual void handleMessage( const Address & source,
		UnpackedMessageHeader & header,
		BufferIStream & data );

	typedef std::map< int, Request > RequestMap;
	RequestMap requestMap_;

	ReplyID nextReplyID_;

};

}

#endif //__REQUESTMANAGER__H__