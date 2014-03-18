#ifndef __mdkObject_h
#define __mdkObject_h

#include "mdkType.h"

namespace mdk
{
class mdkObject
{

protected:
	mdkObject();
	~mdkObject();

    // do not use any virtual function (such as virtual void Clear)

protected:
	mdkObject(const mdkObject&);       // Empty function
	void operator=(const mdkObject&);  // Empty function
};

}//end namespace mdk

#endif
