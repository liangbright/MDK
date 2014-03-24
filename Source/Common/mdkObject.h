#ifndef __mdkObject_h
#define __mdkObject_h

#include "mdkType.h"

namespace mdk
{
class Object
{

protected:
	Object();
	~Object();

    // do not use any virtual function (such as virtual void Clear)

protected:
	Object(const Object&);       // Empty function
	void operator=(const Object&);  // Empty function
};

}//end namespace mdk

#endif
