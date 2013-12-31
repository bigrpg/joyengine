#ifndef __INTERFACE_MACRO__H__
#define __INTERFACE_MACRO__H__


#include "network/Bundle.h"
#include "network/InterfaceElement.h"
#include "network/InterfaceHolder.h"

namespace dove
{
	class ReplyMessageHandler;
}

	//struct msg interface
	#define BEGIN_HANDLED_STRUCT_MESSAGE( NAME, HANDLERTYPE, HANDLERARG )	\
		MERCURY_HANDLED_STRUCT_MESSAGE( NAME, HANDLERTYPE, HANDLERARG )		\
		{																	\
			MERCURY_STRUCT_GOODIES( NAME )									\


	#define END_HANDLED_STRUCT_MESSAGE()									\
		};																	\


	#define MERCURY_HANDLED_STRUCT_MESSAGE(									\
				NAME, HANDLERTYPE, HANDLERARG )								\
		HANDLER_STATEMENT( NAME, HANDLERTYPE, HANDLERARG)					\
		MERCURY_STRUCT_MESSAGE( NAME, HANDLER_ARGUMENT( NAME ) )			\

	#define END_MERCURY_INTERFACE()											\
		} \

	#define END_STRUCT_MESSAGE()											\
		};
		
		
		
	//stream msg interface
	#define BEGIN_HANDLED_STREAM_MESSAGE( NAME, PARAM, HANDLERTYPE, HANDLERARG )	\
		MERCURY_HANDLED_STREAM_MESSAGE( NAME, PARAM, HANDLERTYPE, HANDLERARG )		\
		{																	\
			MERCURY_STREAM_GOODIES( NAME )									\
		
		
	#define MERCURY_HANDLED_STREAM_MESSAGE(									\
				NAME, PARAM, HANDLERTYPE, HANDLERARG )								\
		HANDLER_STATEMENT( NAME, HANDLERTYPE, HANDLERARG)					\
		MERCURY_STREAM_MESSAGE( NAME, PARAM,HANDLER_ARGUMENT( NAME ) )			\

	#define END_STREAM_MESSAGE()											\
		};		

	
#endif //__INTERFACE_MACRO__H__

		

#ifdef BEGIN_MERCURY_INTERFACE
	#undef BEGIN_MERCURY_INTERFACE
	#undef MERCURY_STRUCT_MESSAGE
	#undef MERCURY_STRUCT_GOODIES
	#undef MERCURY_STREAM_MESSAGE
	#undef MERCURY_STREAM_GOODIES
	#undef MERCURY_ISTREAM
	#undef MERCURY_OSTREAM
	#undef MERCURY_MESSAGE
	#undef HANDLER_STATEMENT
	#undef HANDLER_ARGUMENT
	#undef NULL_IF_NOT_HANDLE
#endif

		
#ifdef __INTERFACE__HANDER__HERE__
	#undef __INTERFACE__HANDER__HERE__
	#define __INTERFACE__HERE__
	#define HANDLER_STATEMENT(NAME,TYPE,ARG)								\
		TYPE gHandler_##NAME(ARG);
	#define NULL_IF_NOT_HANDLE(ARG)  ARG
	#define HANDLER_ARGUMENT(NAME) &gHandler_##NAME
#else
	#define HANDLER_STATEMENT(NAME,TYPE,ARG)							
	#define NULL_IF_NOT_HANDLE(ARG)	NULL
	#define HANDLER_ARGUMENT(NAME) NULL
#endif





#ifdef __INTERFACE__HERE__
	#undef __INTERFACE__HERE__
	
	
	#define MERCURY_STRUCT_GOODIES( NAME )
	#define MERCURY_STREAM_GOODIES( NAME )


	#define BEGIN_MERCURY_INTERFACE( INAME ) 								\
		namespace INAME { 													\
			dove::InterfaceHolder gHolder( #INAME );						\
																			\
			void registerWithInterface(										\
					dove::NetworkInterface & networkInterface )			\
			{																\
				gHolder.registerWithInterface( networkInterface );			\
			}																\
			dove::InterfaceHolder& getInterfaceHolder() { return gHolder; } \


	#define MERCURY_STRUCT_MESSAGE( NAME, HANDLER )							\
		MERCURY_MESSAGE( NAME,												\
			FIXED_LENGTH_MESSAGE,											\
			sizeof(struct NAME##Args),										\
			NULL_IF_NOT_HANDLE(HANDLER) )														\
		BufferOStream & operator<<( BufferOStream & b,					\
			const struct NAME##Args &s )									\
		{																	\
			b << s;									\
			return b;														\
		}																	\
		struct _garbage_##NAME##Args


	#define MERCURY_STREAM_MESSAGE(NAME, PARAM, HANDLER) \
		MERCURY_MESSAGE( NAME,												\
			VARIABLE_LENGTH_MESSAGE,											\
			PARAM,										\
			NULL_IF_NOT_HANDLE(HANDLER) )														\
			struct _garbage_##NAME##Args


	#define MERCURY_MESSAGE( NAME, STYLE, PARAM, HANDLER)					\
			const dove::InterfaceElement & NAME##element =						\
				gHolder.add( (const char*)#NAME, (int8)dove::STYLE, (int)PARAM,					\
						(dove::InputMessageHandler*)(HANDLER) );  \
							

	#define MERCURY_ISTREAM( NAME, XSTREAM )	 								\
	inline BufferIStream& operator>>( BufferIStream &is, NAME##Args &x )		\
	{																			\
		return is >> XSTREAM;													\
	}

	#define MERCURY_OSTREAM( NAME, XSTREAM ) 									\
	inline BufferOStream& operator<<( BufferOStream &os, const NAME##Args &x )	\
	{																			\
		return os << XSTREAM;													\
	}

#else //not __INTERFACE__HERE__

	#define MERCURY_STRUCT_GOODIES( NAME )										\
		static NAME##Args & startMessage( dove::Bundle & b)				\
		{																	\
			b.startMessage(NAME##element); \
			return *(NAME##Args*)b.reserve( sizeof(NAME##Args));			\
		}																	\
		static NAME##Args & startRequest( dove::Bundle & b,dove::ReplyMessageHandler * handler,void * arg,int timeout)				\
		{																	\
			b.startRequest(NAME##element,handler,arg,timeout); \
			return startMessage(b);			\
		}																	\
		static NAME##Args & startReply( dove::Bundle & b,dove::ReplyID replyID) 	\
		{	\
			b.startReply(NAME##element,replyID); \
			return startMessage(b);			\
		}\
		

	#define MERCURY_STREAM_GOODIES( NAME )										\
		static void startMessage( dove::Bundle & b)				\
		{																	\
			b.startMessage(NAME##element); \
		}																	\
		static void startRequest( dove::Bundle & b,dove::ReplyMessageHandler * handler,void * arg,int timeout)				\
		{																	\
			b.startRequest(NAME##element,handler,arg,timeout); \
			startMessage(b);			\
		}																	\
		static void startReply( dove::Bundle & b,dove::ReplyID replyID) 	\
		{	\
			b.startReply(NAME##element,replyID); \
			startMessage(b);			\
		}	\


	#define BEGIN_MERCURY_INTERFACE( INAME ) 								\
		namespace INAME { 													\
			extern dove::InterfaceHolder gHolder;						\
																			\
			void registerWithInterface(										\
					dove::NetworkInterface & networkInterface );			\
						\
			dove::InterfaceHolder& getInterfaceHolder();			\
		
		
	#define MERCURY_STRUCT_MESSAGE( NAME, HANDLER )							\
		struct NAME##Args;															\
		MERCURY_MESSAGE( NAME,0,0,NULL )														\
		BufferOStream & operator<<( BufferOStream & b,					\
			const struct NAME##Args &s );									\
		struct NAME##Args


	#define MERCURY_STREAM_MESSAGE( NAME, PARAM, HANDLER )							\
		MERCURY_MESSAGE( NAME,0,0,NULL )														\
		struct NAME##Args						\


	#define MERCURY_MESSAGE( NAME, STYLE, PARAM, HANDLER)					\
			extern const dove::InterfaceElement & NAME##element;					\
		
	#define MERCURY_ISTREAM( NAME, XSTREAM )	 								\
	inline BufferIStream& operator>>( BufferIStream &is, NAME##Args &x );		\


	#define MERCURY_OSTREAM( NAME, XSTREAM ) 									\
	inline BufferOStream& operator<<( BufferOStream &os, const NAME##Args &x );	\

		
#endif	//end of __INTERFACE__HERE__
		
		

