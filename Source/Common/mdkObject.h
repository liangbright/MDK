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

    virtual void ShallowCopy(const Object* Input);

protected:
	Object(const Object&) ;
	void operator=(const Object&);
};

}//end namespace mdk

#endif
