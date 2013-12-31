#include "InterfaceHolder.h"

namespace dove
{

InterfaceHolder::InterfaceHolder( const char * name ) :
	name_( name )
{
	elements_.reserve( 256 );
}

InputMessageHandler * InterfaceHolder::handler( int index )
{
	return elements_[ index ].pHandler();
}

void InterfaceHolder::handler( int index, InputMessageHandler * pHandler )
{
	elements_[ index ].pHandler( pHandler );
}

const InterfaceElement * InterfaceHolder::interfaceElement( uint8 id ) const
{
	if(id == COMP_MESSAGE_ID)
		return &InterfaceElement::s_compIE;
	if(id>= elements_.size())
		return NULL;
	return &elements_[ id ];
}

InterfaceElement & InterfaceHolder::add( const char * name,
	int8 lengthStyle, int lengthParam, InputMessageHandler * pHandler )
{
	printf("add InterfaceElement,id:%ld\n",elements_.size());
	InterfaceElement element( name, elements_.size(), lengthStyle, lengthParam,
		pHandler );

	elements_.push_back( element );
	return elements_.back();
}

void InterfaceHolder::registerWithInterface( NetworkInterface & networkInterface )
{
	for (uint i=0; i < elements_.size(); ++i)
	{
		//InterfaceElement & element = elements_[i];
		//networkInterface.interfaceTable().serve( element, i, element.pHandler() );
	}
}

} // dove
