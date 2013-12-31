#include "Bundle.h"
#include "InterfaceElement.h"
#include "RequestManager.h"

namespace dove
{


Bundle::Bundle( int size ) : 
MemoryOStream(size)
,pIE_(NULL)
,comp_(0)
,pLengthReplace_(NULL)
,pCompLengthReplace_(NULL)
,pCompHeaderReplace_(NULL)
,curReplyID_(0)
{
}

Bundle::~Bundle()
{
	//JG_ASSERT(!pLengthReplace_);
}

Bundle& Bundle::startMessage(const dove::InterfaceElement& ie)
{
	if(comp_ == 0)
		endFinal();
	else
		--comp_;

	pIE_ = &ie;
	(*this) << ie.id();
	
	pLengthReplace_ = NULL;
	if(ie.nominalBodySize() ==0)
	{
		pLengthReplace_ = (char *)reserve(ie.lengthParam());
	}
	
	return *this;
}

Bundle& Bundle::endMessage()
{
	if(pLengthReplace_)
	{
		_endMessage(pLengthReplace_,*pIE_);		
		pLengthReplace_ = NULL;
		pIE_ = NULL;
	}
	return *this;
}
	
Bundle& Bundle::endFinal()
{
	endMessage();
	
	if(pCompLengthReplace_)
	{
		_endMessage(pCompLengthReplace_,InterfaceElement::s_compIE);	
		pCompLengthReplace_ = NULL;
	}
	comp_ = 0;
	curReplyID_ = 0;
	
	return *this;
}

Bundle& Bundle::_endMessage(char * replace,const dove::InterfaceElement& ie)
{
	int msgLen = size() - (replace - (char*)data()) - ie.lengthParam();
	ie.packLength(msgLen,replace);
	return *this;
}

Bundle& Bundle::startRequest(const dove::InterfaceElement & ie,
	ReplyMessageHandler * handler,
	void * arg,
	int timeout)
{
	startCompMessage();
	
	ReplyOrder	ro = {handler, arg, timeout, &pCompHeaderReplace_->replyID_};
	replyOrders_.push_back(ro);
	
	curReplyID_ = RequestManager::instance().getNextReplyID();
	pCompHeaderReplace_->replyID_ = curReplyID_;
	pCompHeaderReplace_->flag_ |= MSGHEAD_FLAG_REQUEST;
	pCompHeaderReplace_->flag_ &= ~MSGHEAD_FLAG_REPLY;
	
	return *this;
}

Bundle& Bundle::startReply(const dove::InterfaceElement & ie,dove::ReplyID replyID)
{
	startCompMessage();
	pCompHeaderReplace_->replyID_ = replyID;	
	pCompHeaderReplace_->flag_ |= MSGHEAD_FLAG_REPLY;
	pCompHeaderReplace_->flag_ &= ~MSGHEAD_FLAG_REQUEST;

	return *this;
}

void Bundle::startCompMessage()
{
	endFinal();
	
	comp_ = 1; //max count that this CompMessage can hold
	InterfaceElement& ie = InterfaceElement::s_compIE;
	(*this) << ie.id();
	pCompLengthReplace_ = (char *)reserve(ie.lengthParam());;
	pCompHeaderReplace_ = (CompMessageHeader*)reserve(sizeof(dove::CompMessageHeader));
		
	(pCompHeaderReplace_->replyID_) = 0;
	
}

} //namespace dove