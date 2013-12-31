#include "msg.h"
#include "db_mysql/MySqlConnectionPool.h"
#include "db_mysql/Query.h"
#include "db_mysql/ResultSet.h"
#include "network/Helper.h"
#include "network/Nub.h"
#include "stdinc/MemoryStream.h"
#include "stdinc/BgTask.h"


#include "msg.h" 
#include "connection/ClientInterface.h"

#define __INTERFACE__HERE__
#include "connection/ClientInterface.h"


///////////////////////////////////////////////////////////////////////////  
static Query s_queryUser("select * from auth where user = ?;");
static Query s_queryRole("select * from role where id = ?;");
static Query s_createChar("insert into role (id,name) values(?,?);");

class  BgTask_login : public BgTask
{
	std::string user_;
	std::string pwd_;
			
	dove::NubPtr nub_;
	dove::Bundle msg_;
	
public:
	BgTask_login(dove::NubPtr nub,const std::string& user,const std::string& pwd);
		
	void workInMainThread();
	void workInBackThread();
};

BgTask_login::BgTask_login(dove::NubPtr nub,const std::string& user,const std::string& pwd) :
	BgTask()
	,user_(user)
	,pwd_(pwd)
  ,msg_(64)
{
	nub_ = nub;
}

void BgTask_login::workInBackThread()
{	
  MySql * conn = MySqlConnectionPool::instance().get();
  ResultSet result;
  s_queryUser.execute(*conn,user_,&result);
  
  bool ok = true;
  int  uuid ;
  std::string pwd;
  if(result.getResult(user_,pwd,uuid))
  {
  	printf("get data from mysql: name:%s  pwd:%s\n",user_.c_str(),pwd.c_str());
 	}
  else
  {
  	ok = false;
  	printf("query nothing data\n");
  }
  
  MySqlConnectionPool::instance().give(conn);
  
  if(canceled())
  	return;
  
  ClientInterface::loginArgs & args = ClientInterface::loginArgs::startMessage(msg_);
  args.status = 2; //has not this user

	if(!ok)
		return;
	
	if(pwd == pwd_)
	{
		printf("login successfully\n");
		args.status = 0;
        
    ClientInterface::sendUUIDArgs & args = ClientInterface::sendUUIDArgs::startMessage(msg_);
    args.uuid = uuid ;
        
    ClientInterface::sendPlayerInfoArgs info;
    s_queryRole.execute(*conn,uuid,&result);
    if(!result.getResult(uuid,info.roleName,info.level,info.money,info.gold,info.exp))
    {
        args.charCount = 0;
    }
    else
    {
        args.charCount = 1;
        ClientInterface::sendPlayerInfoArgs::startMessage(msg_);
        msg_<<info;
    }   
    msg_.endMessage();
    nub_->setUserData(intptr(uuid));
  }
  else
  {
  	printf("login pwd is error\n");
  	args.status = 1;
  }
}

void BgTask_login::workInMainThread()
{
	if(msg_.size()>0)
		nub_->send(msg_);	
}


class  BgTask_createChar : public BgTask
{
	dove::NubPtr nub_;
	int uuid_;
	std::string name_;
  dove::Bundle msg_;
public:
	BgTask_createChar(dove::NubPtr,int uuid,const std::string& name);
    
	void workInMainThread();
	void workInBackThread();
};

BgTask_createChar::BgTask_createChar(dove::NubPtr nub,int uuid, const std::string& name) :
BgTask()
,name_(name)
,uuid_(uuid)
,msg_(1024)
{
	nub_ = nub;
}

void BgTask_createChar::workInBackThread()
{
    
    MySql * conn = MySqlConnectionPool::instance().get();
    ResultSet result;
    s_createChar.execute(*conn,uuid_,name_,&result);
    
    s_queryRole.execute(*conn,uuid_,&result);
    
    int id;
    ClientInterface::sendPlayerInfoArgs info;
    if(result.getResult(id,info.roleName,info.level,info.money,info.gold,info.exp))
    {
     
   	  ClientInterface::sendPlayerInfoArgs::startMessage(msg_);
   	  msg_ << info;
    	msg_.endMessage();
       
    }
}

void BgTask_createChar::workInMainThread()
{
	if(msg_.size()>0)
		nub_->send(msg_);
}


//interface handlers
namespace ServerInterface
{
	
	void msg1(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,const ServerInterface::msg1Args & args)
	{
		printf("get data from server:%d %d\n",args.param,args.id);
	}
	
	void login(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,const ServerInterface::loginArgs& args)
	{
		printf("reply id:%d\n",header.info_.replyID_);
		std::string name,pwd;
		name = args.name;
		pwd = args.pwd;
		printf("get user info:name=%s pwd=%s\n",name.c_str(),pwd.c_str());
		BgTask* task= new BgTask_login(header.nub_,name,pwd);
		WorkThreadManager::instance().addTask(task,0,srcAddr.fd_); 
	}
    
    void createChar(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,const ServerInterface::createCharArgs& args)
    {
      BgTask* task= new BgTask_createChar(header.nub_,args.uuid,args.name);
			WorkThreadManager::instance().addTask(task,0,srcAddr.fd_);

    }
    
	void notice(dove::Address & srcAddr,const dove::UnpackedMessageHeader & header,BufferIStream& data)
	{
		int size = data.remainingLength();
		uint8 a;
		data >> a;
		printf("notice size:%d, %d\n",size,a);
	}

}