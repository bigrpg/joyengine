#include "Nub.h"
#include "stdinc/BufferStream.h"
#include "InterfaceHolder.h"
#include "network/Handlers.h"
#include "stdinc/MemoryStream.h"
#include "Bundle.h"
#include "Helper.h"
#include "RequestManager.h"
#include "NetPoint.h"


namespace dove
{
	
ssize_t mysend(int socket,const void * buffer,size_t length,int flags)
{
//	if(length>5)
//		length = 5;
	return ::send(socket,buffer,length,flags);
}	
	
Nub::Nub(dove::InterfaceHolder & interfaceHolder) :
remLen_(0)
,sremLen_(0)
,fd_(NOSOCKET)
,interfaceHolder_(interfaceHolder)
,userData_(intptr())
,fdStatus_(SOCKET_INVALID)
{
}

void Nub::setfd(int fd,SOCKETSTATUS status)
{
	fd_ = fd;
	fdStatus_ = status;
}

bool Nub::combineMsg(char * data,int len,char *& outdata,int & outlen,MsgInfoVec & msgs)
{
	outdata = data;
	outlen = len;

	if(len<sizeof(dove::MessageID))
		return true;

	dove::MessageID id = *((dove::MessageID*)data);
	data += sizeof(dove::MessageID);
	len -= sizeof(dove::MessageID);
		
  const dove::InterfaceElement* element = interfaceHolder_.interfaceElement(id);
  if(!element)
  {
  	printf("Unkonwn messageID:%d\n",id);
  	return false;
  }
  
  MemoryIStream stream(data,len);

	int msgLen = element->nominalBodySize();
	if(msgLen == 0) //var len
	{
		msgLen = element->unpackLength(stream); //return value: -1: error
		if(msgLen == -1)
			return true;
		len -= element->lengthParam();
		data += element->lengthParam();
	}
	if(len < msgLen)
		return true;
		

	MsgInfo msg;
	msg.begin_ = data;
	msg.length_ = msgLen;
	msg.id_ = id;

	msgs.push_back(msg);

	data += msgLen;
	len -= msgLen;
	
	return combineMsg(data,len,outdata,outlen,msgs);

}

dove::Nub::SOCKETSTATUS Nub::processMsg()
{
	char * data = buff_ + remLen_;
	int len = recv(fd_,data,buffLength - remLen_,0);
	if(len == -1)
	{
		if(errno == EAGAIN || errno == EWOULDBLOCK)
			return STATUS_NODATA;
		printf("recv error code:%d\n",errno);
		return STATUS_READERROR;
	}
	else if(len == 0)
		return STATUS_CLOSED_REMOTE;

	int dataTotalSize = remLen_ + len;
	MsgInfoVec msgs;
	bool ok = combineMsg(buff_,dataTotalSize,data,len,msgs);
	if(!ok) //
		return STATUS_READERROR;

	//process msg
	for(size_t i=0;i<msgs.size();++i)
 	{
 		const MsgInfo & msgInfo = msgs[i];
	  const dove::InterfaceElement* element = interfaceHolder_.interfaceElement(msgInfo.id_);
	  dove::InputMessageHandler *pHandler = element->pHandler();
	  if(pHandler)
	  {
		  dove::UnpackedMessageHeader unpackedHead(msgInfo.id_,msgInfo.length_);
		  unpackedHead.info_ = msgInfo.info_;
		  unpackedHead.nub_ = this;
	  	MemoryIStream msg(msgInfo.begin_,msgInfo.length_);
	  	dove::Address address(0,0,fd_);	
	  	pHandler->handleMessage(address,unpackedHead,msg);
	  	if( msgInfo.id_ == COMP_MESSAGE_ID)
	  	{
	  		size_t curMsgCount = msgs.size();	  		
	  		char * out;int outlen;
	  		combineMsg((char*)msg.data(),msg.remainingLength(),out,outlen,msgs);
	  		for(size_t k=curMsgCount;k<msgs.size();++k)
	  		{
	  			printf("replyID:[%d],msgID:%d\n",unpackedHead.info_.replyID_,msgs[k].id_);
	  			msgs[k].info_ = unpackedHead.info_;
	  		}
	  	}
	  }
	}
 			
	remLen_ = len;
	if(len >0)
	{
		int gap = dataTotalSize - len;
		if(gap>=len)
		{
			memcpy(buff_,data,len);
		}
		else
		{
			memcpy(buff_,data,gap);
			memcpy(buff_+gap,data+gap,len-gap);
		}
	}
	return STATUS_NORMAL;
}

//called in main thread
dove::Nub::SOCKETSTATUS Nub::processAllReachMsg()
{
	dove::Nub::SOCKETSTATUS status;
	while( (status = processMsg()) == STATUS_NORMAL);
	return status;
}

dove::Nub::SOCKETSTATUS Nub::sendLastRemain()
{
	char * data;
	int len;
	SOCKETSTATUS ret;
	
	if(sremLen_>0)
	{
		ret = sendMsg(sbuff_,sremLen_,data,len);
		if(ret == STATUS_SENDERROR)
			return ret;
		if(ret == STATUS_SENDPART)
		{
			JG_ASSERT(len>0);
			JG_ASSERT(sremLen_ >=len);
			int gap = sremLen_ - len;
			if(gap >= len)
				memcpy(sbuff_,sbuff_+gap,len);
			else if(gap>0)
			{
				memcpy(sbuff_,sbuff_+gap,gap);
				memcpy(sbuff_+gap,sbuff_+2*gap,len-gap);
			}
			sremLen_ = len;
		}
		else
			sremLen_ = 0;
	}
	
	if(sremLen_>0)
		return STATUS_SENDPART;
	return STATUS_SENDALL;
	
}

dove::Nub::SOCKETSTATUS Nub::send(dove::Bundle& b)
{
	b.endFinal();
	
	char * data;
	int len;
	SOCKETSTATUS ret;
	
	ret = sendLastRemain();
	if( ret != STATUS_SENDALL)
		return ret;
	
	if(b.size()==0)
		return STATUS_SENDALL;
		
	const ReplyOrders & orders = b.replyOrders();
	RequestManager::instance().addReplyOrders(orders);
	
			
	ret = sendMsg((char*)b.data(),b.size(),data,len);
	if(ret == STATUS_SENDPART)
	{
		JG_ASSERT(len>0);
		JG_ASSERT(len <= sbuffLength);
		memcpy(sbuff_,(char*)b.data()+b.size()-len,len);
		sremLen_ = len;
	}

	return ret;
}


dove::Nub::SOCKETSTATUS Nub::sendMsg(char * data,int len,char* & outdata,int& outlen)
{
	outdata = data;
	outlen = len;
	
	if(len ==0)
		return STATUS_SENDALL;
	
	int sendLen = mysend(fd_,data,len,0);
	if(sendLen == -1)
	{
		if(errno == EAGAIN)
			return STATUS_SENDPART;
		return STATUS_SENDERROR;
	}
		
	data += sendLen;
	len -= sendLen;
		
	return sendMsg(data,len,outdata,outlen);
	
}

void Nub::clear()
{
	remLen_ 	= 0;
	sremLen_ 	= 0;
	fd_ 			= NOSOCKET;
}

void Nub::close()
{
	while( sendLastRemain() == STATUS_SENDPART);
	
	dove::NetPoint np;
	np.setfd(fd_);
	np.close();
	
	clear();
}
		
dove::Nub::SOCKETSTATUS Nub::onRead()
{
	if(fdStatus_ == SOCKET_CONNECTING)
		return checkConnected();
	return processAllReachMsg();
}
	
dove::Nub::SOCKETSTATUS Nub::onWrite()
{
	if(fdStatus_ == SOCKET_CONNECTING)
		return checkConnected();

	return sendLastRemain();
}

int Nub::onAccept()
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	
	return accept(fd_, (struct sockaddr *)&client_addr, &client_len);
}

dove::Nub::SOCKETSTATUS Nub::checkConnected()
{
	uint8 c;
	if(read(fd_,&c,0) == 0)
	{
		fdStatus_ = SOCKET_CONNECTED;
		return SOCKET_CONNECTED;
	}
	return SOCKET_INVALID;
}

////// NubException

NubException::NubException( Reason reason, const Address & addr ) :
	reason_( reason ),
	address_( addr )
{}

Reason NubException::reason() const
{
	return reason_;
}

bool NubException::getAddress( Address & addr ) const
{
	addr = address_;
	return address_ != Address::NONE;
}

} //namespace dove