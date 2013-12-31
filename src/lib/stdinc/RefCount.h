#ifndef __REFCOUNT__H__
#define __REFCOUNT__H__

#include "stdinc.h"

#ifndef __DISABLE__ARM__
#include <atomic>
#endif

class RefCount
{
protected:
	volatile mutable int count_;
	RefCount & operator=( const RefCount & other );
	
public:
	RefCount() : count_(0)
	{}
	
	virtual ~RefCount() {}
	
	inline void addRef() const
	{
		++count_;
	}
	inline void decRef() const
	{
		if(--count_ == 0)
			delete const_cast<RefCount*>(this);
	}
	
	int getCount() const
	{
		return count_;
	}
};


class SafeRefCount : public RefCount
{
#ifndef __DISABLE__ARM__
	mutable std::atomic_int x_;
#endif
protected:
	inline int intDecRef() const
	{
        int ret;
#ifdef __DISABLE__ARM__
		__asm__ volatile (
			"mov $-1, %0  \n\t"
			"lock xadd %0, %1"
			: "=&a"	(ret)
			: "m"	(this->count_)
			: "memory"
		);
#else
		int x = std::atomic_fetch_sub(&x_,1);
		ret = --x;
#endif
		return ret;
	}


public:
#ifndef __DISABLE__ARM__
	SafeRefCount() : RefCount()
	{
		x_ = ATOMIC_VAR_INIT(0);
	}
	int getCount() const
	{
		return x_;
	}
#endif
  void addRef() const
  {
#ifdef __DISABLE__ARM__
     __asm__ volatile (
       "lock addl $1, %0"
       :
       : "m" (this->count_)
       : "memory"
      );
#else
    std::atomic_fetch_add(&x_,1);
#endif
  }

	inline void decRef() const
	{
		if (this->intDecRef() == 1)
			delete const_cast<SafeRefCount*>(this);
	}
		
};

template <class Ty>
inline void incrementReferenceCount( const Ty &Q )
{
	Q.addRef();
}

template <class Ty>
inline bool isZeroReferenceCount( const Ty &Q )
{
	return (Q.getCount() == 0);
}

template <class Ty>
inline void decrementReferenceCount( const Ty &Q )
{
	Q.decRef();
}


//---------  SmartPointer
template<class Ty> class ConstSmartPointer
{
public:
	static const bool STEAL_REFERENCE = true;
	static const bool NEW_REFERENCE = false;

	typedef Ty Object;
	typedef ConstSmartPointer<Ty> This;

public:
	ConstSmartPointer( const Object *P = 0, bool alreadyIncremented = false )
	{
		object_ = P;
		if (object_)
		{
			if (!alreadyIncremented)
			{
				incrementReferenceCount( *object_ );
			}
			else
			{
				if (isZeroReferenceCount( *object_ ))
				{
					JG_ASSERT( 0 &&
						"ConstSmartPointer::ConstSmartPointer refCount == 0 error ");
				}
			}
		}
	}


	ConstSmartPointer( const This& P )
	{
		object_ = P.get();
		if (object_) incrementReferenceCount( *object_ );
	}

	This& operator=( const This& X )
	{
		if (object_ != X.get())
		{
			const Object* pOldObj = object_;
			object_ = X.get();
			if (object_) incrementReferenceCount( *object_ );
			if (pOldObj) decrementReferenceCount( *pOldObj );
		}
		return *this;
	}

	~ConstSmartPointer()
	{
		if (object_) decrementReferenceCount( *object_ );
		object_ = 0;
	}

	const Object * get() const
	{
		return object_;
	}

	const Object * getObject() const
	{
		return this->get();
	}

	bool hasObject() const
	{
		return object_ != 0;
	}

	bool exists() const
	{
		return object_ != 0;
	}

	const Object& operator*() const
	{
		return *object_;
	}

	const Object* operator->() const
	{
		return object_;
	}

	friend bool operator==( const ConstSmartPointer<Ty>& A,
		const ConstSmartPointer<Ty>& B )
	{
		return A.object_ == B.object_;
	}

	friend bool operator==( const ConstSmartPointer<Ty>& A,
		const Ty* B )
	{
		return A.object_ == B;
	}

	friend bool operator==( const Ty* A,
		const ConstSmartPointer<Ty>& B )
	{
		return A == B.object_;
	}

	friend bool operator!=( const ConstSmartPointer<Ty>& A,
		const ConstSmartPointer<Ty>& B )
	{
		return A.object_ != B.object_;
	}

	friend bool operator!=( const ConstSmartPointer<Ty>& A,
		const Ty* B )
	{
		return A.object_ != B;
	}

	friend bool operator!=( const Ty* A,
		const ConstSmartPointer<Ty>& B )
	{
		return A != B.object_;
	}

	friend bool operator<( const ConstSmartPointer<Ty>& A,
		const ConstSmartPointer<Ty>& B )
	{
		return A.object_ < B.object_;
	}

	friend bool operator<( const ConstSmartPointer<Ty>& A,
		const Ty* B )
	{
		return A.object_ < B;
	}

	friend bool operator<( const Ty* A,
		const ConstSmartPointer<Ty>& B )
	{
		return A < B.object_;
	}

	friend bool operator>( const ConstSmartPointer<Ty>& A,
		const ConstSmartPointer<Ty>& B )
	{
		return A.object_ > B.object_;
	}

	friend bool operator>( const ConstSmartPointer<Ty>& A,
		const Ty* B )
	{
		return A.object_ > B;
	}

	friend bool operator>( const Ty* A,
		const ConstSmartPointer<Ty>& B )
	{
		return A > B.object_;
	}

protected:
	const Object *object_;
	
};


template <class Ty>
class SmartPointer : public ConstSmartPointer<Ty>
{
public:
	typedef SmartPointer<Ty> This;
	typedef ConstSmartPointer<Ty> ConstProxy;
	typedef Ty Object;

public:
	SmartPointer( Object *P = 0, bool alreadyIncremented = false ) :
		ConstProxy( P, alreadyIncremented ) { }

	SmartPointer( const This& P ) : ConstProxy( P ) { }

	template<class DerivedType>
	SmartPointer( ConstSmartPointer<DerivedType>& dt ) :
		ConstProxy( dt.get() )
	{
	}

	This& operator=( const This& P )
	{
		ConstProxy::operator=(P);
		return *this;
	}

	template<class DerivedType>
	This& operator=( ConstSmartPointer<DerivedType>& dt )
	{
		ConstProxy::operator=(dt.get());
		return *this;
	}

	Object * get() const
	{
		return const_cast<Object *>( this->object_ );
	}

	Object& operator*() const
	{
		return *const_cast<Object *>( this->object_ );
	}

	Object* operator->() const
	{
		return const_cast<Object *>( this->object_ );
	}
};
#endif //__REFCOUNT__H__