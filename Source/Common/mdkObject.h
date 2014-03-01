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

    // re-initialize the object, e.g., to be an empty object
    virtual void Clear();

protected:
	mdkObject(const mdkObject&);       // Empty function
	void operator=(const mdkObject&);  // Empty function
};

}//end namespace mdk

#endif