#ifndef __VALUEORNULL__H__
#define __VALUEORNULL__H__

#include "../stdinc/stdinc.h"

template <class T>
class ValueOrNull
{
public:
	ValueOrNull() : isNull_( true ) {}
	explicit ValueOrNull( const T & x ) : value_( x ), isNull_( false ) {}

	void setNull()				{ isNull_ = true; }
	void setValue( const T& x ) { value_ = x; isNull_ = 0; }

	bool isNull() const			{ return isNull_; }

	const T * get() const		{ return isNull_ ? NULL : &value_; }

protected:
	T value_;
	bool isNull_;
};



#endif //__VALUEORNULL__H__