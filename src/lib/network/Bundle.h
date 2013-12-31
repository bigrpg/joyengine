#ifndef __BUNDLE__H__
#define __BUNDLE__H__


#include "stdinc/MemoryStream.h"
#include "network_base_type.h"


namespace dove
{
class InterfaceElement;
class ReplyMessageHandler;
class CompMessageHeader;


class ReplyOrder
{
public:
	ReplyMessageHandler * handler_;
	void * arg_;
	int timeout_;
	ReplyID * replyID_;
};

typedef std::vector<ReplyOrder>  ReplyOrders;
	
class Bundle : public MemoryOStream
{
	char * pLengthReplace_;
	char * pCompLengthReplace_;
	CompMessageHeader * pCompHeaderReplace_;
	const dove::InterfaceElement * pIE_;
	ReplyOrders  replyOrders_;
	int comp_;
	ReplyID curReplyID_;
		
protected:
	void startCompMessage();
	Bundle& _endMessage(char * replace,const dove::InterfaceElement& ie);
		
public:
	Bundle( int size = 64 );
	~Bundle();
		
	Bundle& startMessage(const dove::InterfaceElement& );
	Bundle& startRequest(const dove::InterfaceElement & ie,
		ReplyMessageHandler * handler,
		void * arg = NULL,
		int timeout = DEFAULT_REQUEST_TIMEOUT);
	Bundle& startReply(const dove::InterfaceElement & ie,dove::ReplyID replyID);
		
	Bundle& endMessage();
	Bundle& endFinal();
	
	const ReplyOrders& replyOrders() const { return replyOrders_;}
	ReplyID curReplyID() const { return curReplyID_;}
	

};
} //namesapce dove

#endif //__BUNDLE__H__