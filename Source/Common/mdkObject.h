#ifndef __mdkObject_h
#define __mdkObject_h

#include "mdkType.h"

namespace mdk
{

class mdkObject
{

public:		
	mdkObject();
	~mdkObject();

protected:
	mdkObject(const mdkObject&);       // Empty function
	void operator=(const mdkObject&);  // Empty function
};

}//end namespace mdk

#endif