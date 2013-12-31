#ifndef __INTERFACE_ELEMENT_H__
#define __INTERFACE_ELEMENT_H__

#include "stdinc/stdinc.h"
#include "network_base_type.h"

#include <vector>


class BufferOStream;
class BufferIStream;

namespace dove
{
	
const char FIXED_LENGTH_MESSAGE = 0;
const char VARIABLE_LENGTH_MESSAGE = 1;
const char INVALID_MESSAGE = 2;

class InputMessageHandler;

class InterfaceElement
{
public:
	InterfaceElement( const char * name = "", MessageID id = 0,
			int8 lengthStyle = INVALID_MESSAGE, int lengthParam = 0,
			dove::InputMessageHandler * pHandler = NULL );

   	void set( const char * name, MessageID id, int8 lengthStyle,
		int lengthParam )
	{
		id_ = id;
		lengthStyle_ = lengthStyle;
		lengthParam_ = lengthParam;
		name_ = name;
	}

	void packLength(int length, BufferOStream& stream) const;	
	void packLength(int length, char *) const;
	int unpackLength(BufferIStream& stream) const;
		
	int headerSize() const;
	int nominalBodySize() const;

	MessageID id() const				{ return id_; }
	void id( MessageID id )				{ id_ = id; }

	int8 lengthStyle() const			{ return lengthStyle_; }
	int lengthParam() const				{ return lengthParam_; }

	const char * name() const			{ return name_; }

	const char * c_str() const;

	InputMessageHandler * pHandler() const { return pHandler_; }
	void pHandler( InputMessageHandler * pHandler ) { pHandler_ = pHandler; }
	
	static InterfaceElement s_compIE;

protected:
	MessageID			id_; 			///< Unique message ID
	int8				lengthStyle_;	///< Fixed or variable length
	int					lengthParam_;	///< This depends on lengthStyle
	const char *		name_;			///< The name of the interface method
	InputMessageHandler * pHandler_;
};

typedef std::vector<InterfaceElement> InterfaceElements;

} //namespace dove

#endif //__INTERFACE_ELEMENT_H__