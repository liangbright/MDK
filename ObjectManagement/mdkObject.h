#ifndef __mdkObject_h
#define __mdkObject_h

#include <memory>
#include <mutex>

namespace mdk
{

template<typename T>
class Object
{

public:	
	static std::shared_ptr<T> New();

	static T* NewRaw();
	
	static bool Delete(T*);

	static bool CollectGarbage();

	static std::shared_ptr<T>  CreateSharedPointer(T*);

	static std::weak_ptr<T>  CreateWeakPointer(T*);

	static bool IsInHeap(T*);
	
	static bool IsBeingShared(T*);

	static bool IsValid(T*);
	
	Object(void);
	~Object(void);		
};

}//end namespace mdk

#include "mdkObject.hpp"

#endif