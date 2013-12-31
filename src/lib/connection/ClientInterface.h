#if  defined(__INTERFACE__HERE__) ||    defined(__INTERFACE__HANDER__HERE__)
#undef __CLIENTINTERFACE__
#endif


#ifndef __CLIENTINTERFACE__
#define __CLIENTINTERFACE__

#include "network/InterfaceMacro.h"
#include "MsgHandler.h"
#include "StructInfo.h"


#define JG_CLIENT_BEGIN_STRUCT_MSG( NAME )											\
	BEGIN_HANDLED_STRUCT_MESSAGE( NAME,												\
			StructMessageHandler< ClientInterface::NAME##Args >,			\
			&ClientInterface::NAME )										\


#define JG_CLIENT_BEGIN_STREAM_MSG( NAME ,PARAM)											\
	BEGIN_HANDLED_STREAM_MESSAGE( NAME,	PARAM,											\
			StreamStructMessageHandler< ClientInterface::NAME##Args >,			\
			&ClientInterface::NAME )										\

#define  JG_CLIENT_STREAM_MSG(NAME,PARAM)  \
	BEGIN_HANDLED_STREAM_MESSAGE( NAME,	PARAM,											\
			StreamMessageHandler,			\
			&ClientInterface::NAME )										\
    END_STREAM_MESSAGE() 


#define JG_CLIENT_STRUCT_MSG(NAME)                          \
JG_CLIENT_BEGIN_STRUCT_MSG( NAME )                         \
END_STRUCT_MESSAGE()


#pragma pack(push, 1)
BEGIN_MERCURY_INTERFACE(ClientInterface)

//******************************************    login             ****************************************
	JG_CLIENT_BEGIN_STRUCT_MSG( login )
		uint32				status;
	END_STRUCT_MESSAGE()


//******************************************    login             ****************************************
    JG_CLIENT_BEGIN_STRUCT_MSG( sendUUID )
        uint32				uuid;
        uint8               charCount;
    END_STRUCT_MESSAGE()

//******************************************    sendPlayerInfo    ****************************************
  JG_CLIENT_BEGIN_STREAM_MSG( sendPlayerInfo,1)
    int32           uuid;
  	std::string		roleName;
    uint16           level;
    uint32           money;
    uint32           gold;
    uint32           exp;

    END_STREAM_MESSAGE()

	MERCURY_ISTREAM(sendPlayerInfo, x.roleName>>x.level>>x.money>>x.gold>>x.exp )
	MERCURY_OSTREAM(sendPlayerInfo, x.roleName<<x.level<<x.money<<x.gold<<x.exp )


 
//******************************************   sendAllHero        ****************************************

 JG_CLIENT_BEGIN_STREAM_MSG(sendAllHero,1)
  
    std::vector<HeroInfo> heros;

    END_STREAM_MESSAGE()

    MERCURY_ISTREAM(sendAllHero, x.heros)
    MERCURY_OSTREAM(sendAllHero, x.heros)

//******************************************                     ****************************************

END_MERCURY_INTERFACE()
#pragma pack( pop )

#endif //__CLIENTINTERFACE__