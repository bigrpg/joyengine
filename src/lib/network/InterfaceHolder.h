#ifndef __INTERFACEHOLDER__H__
#define __INTERFACEHOLDER__H__

#include "InterfaceElement.h"

namespace dove
{

class NetworkInterface;

class InterfaceHolder
{
public:
	InterfaceHolder( const char * name );

	InterfaceElement & add( const char * name, int8 lengthStyle,
			int lengthParam, InputMessageHandler * pHandler = NULL );

	InputMessageHandler * handler( int index );
	void handler( int index, InputMessageHandler * pHandler );
	const InterfaceElement * interfaceElement( uint8 id ) const;

	void registerWithInterface( NetworkInterface & networkInterface );

private:
	InterfaceElements		elements_;
	const char *			name_;
};

}	// end of namespace dove



#endif //__INTERFACEHOLDER__H__