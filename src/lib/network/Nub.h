#ifndef __NUB__H__
#define __NUB__H__

#include "network_base_type.h"
#include "stdinc/RefCount.h"

#include <vector>


#define buffLength		65536
#define sbuffLength		65536


class NetEventWaitor;
namespace dove
{
	class InterfaceHolder;
	class Bundle;


class Nub : public RefCount
{
	
public:	
	typedef enum
	{
		//接收状态
		STATUS_NODATA = 0,			
		STATUS_CLOSED_REMOTE,
		STATUS_READERROR,
		STATUS_NORMAL,

		//发送状态
		STATUS_SENDPART,	//发送了一部分
		STATUS_SENDERROR,			//send error
		STATUS_SENDALL,				//全部发送完成

		//socket状态
		SOCKET_INVALID,
		SOCKET_CONNECTED,
		SOCKET_CONNECTING,
	}SOCKETSTATUS;

protected:
	struct MsgInfo
	{
		dove::MessageID id_;
		int length_;
		CompMessageHeader  info_;
		char * begin_;
	};

	typedef std::vector<MsgInfo>  MsgInfoVec;

	intptr	userData_;
	char buff_[buffLength];
	int remLen_;

	char sbuff_[sbuffLength];
	int sremLen_;
	
	int fd_;
	SOCKETSTATUS fdStatus_;
	
	dove::InterfaceHolder & interfaceHolder_;
	
	//if msg intruppt,return false
	bool combineMsg(char * indata,int inlen,char *& outdata,int & outlen,MsgInfoVec & msgs);
	dove::Nub::SOCKETSTATUS processMsg();
	dove::Nub::SOCKETSTATUS sendMsg(char * data,int len,char* & outdata,int& outlen);
	dove::Nub::SOCKETSTATUS sendLastRemain();
	dove::Nub::SOCKETSTATUS checkConnected();	

public:
	Nub(dove::InterfaceHolder &);
	void setfd(int fd,SOCKETSTATUS status=SOCKET_CONNECTED);
	int getfd() const { return fd_;}
		
	int remLen() const { return remLen_;}
	
	dove::Nub::SOCKETSTATUS send(dove::Bundle& b);
	dove::Nub::SOCKETSTATUS processAllReachMsg();

	void clear();
	void close();
	
	intptr getUserData() const { return userData_;}
	void setUserData(intptr u) { userData_ = u;}
	
	dove::Nub::SOCKETSTATUS onRead();
	dove::Nub::SOCKETSTATUS onWrite();
	int onAccept();

	friend class NetEventWaitor;

};

typedef SmartPointer<Nub>  NubPtr;



class NubException
{
public:
	NubException( Reason reason, const Address & addr = Address::NONE );
	virtual ~NubException() {};
	Reason reason() const;
	bool getAddress( Address & addr ) const;

private:
	Reason		reason_;
	Address address_;
};


} // namespace dove
#endif //__NUB__H__