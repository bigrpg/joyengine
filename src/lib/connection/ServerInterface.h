#if  defined(__INTERFACE__HERE__) ||    defined(__INTERFACE__HANDER__HERE__)
#undef __SERVERINTERFACE__
#endif


#ifndef __SERVERINTERFACE__
#define __SERVERINTERFACE__

#include "network/InterfaceMacro.h"
#include "MsgHandler.h"



#define JG_BEGIN_STRUCT_MSG( NAME )											\
	BEGIN_HANDLED_STRUCT_MESSAGE( NAME,												\
			StructMessageHandler< ServerInterface::NAME##Args >,			\
			&ServerInterface::NAME )										\


#define JG_BEGIN_STREAM_MSG( NAME ,PARAM)											\
	BEGIN_HANDLED_STREAM_MESSAGE( NAME,	PARAM,											\
			StreamStructMessageHandler< ServerInterface::NAME##Args >,			\
			&ServerInterface::NAME )										\

	
#define JG_STREAM_MSG( NAME, PARAM )  \
	BEGIN_HANDLED_STREAM_MESSAGE( NAME,	PARAM,											\
			StreamMessageHandler,			\
			&ServerInterface::NAME )										\
	END_STREAM_MESSAGE()


#pragma pack(push, 1)
BEGIN_MERCURY_INTERFACE(ServerInterface)


//******************************************   msg1        ****************************************
	JG_BEGIN_STRUCT_MSG( msg1 )
		uint32				param;
		uint8					id;
	END_STRUCT_MESSAGE()


//******************************************   login        ****************************************
	JG_BEGIN_STREAM_MSG( login,1 )
		std::string 				name;
		std::string					pwd;
		struct RoleInfo{
			std::string role;
			int id;
		};
		std::vector<RoleInfo> roles;
	END_STREAM_MESSAGE()
	MERCURY_ISTREAM(login, x.name >> x.pwd >> x.roles)
	MERCURY_OSTREAM(login, x.name << x.pwd << x.roles)

//******************************************   notice        ****************************************

	JG_STREAM_MSG( notice, 2 )


//******************************************   createChar    ****************************************
    JG_BEGIN_STREAM_MSG(createChar, 1)
        int  uuid;
        std::string    name;
    END_STREAM_MESSAGE()
    MERCURY_ISTREAM(createChar, x.uuid>>x.name )
    MERCURY_OSTREAM(createChar, x.uuid<<x.name )



END_MERCURY_INTERFACE()
#pragma pack( pop )

#endif //__SERVERINTERFACE__