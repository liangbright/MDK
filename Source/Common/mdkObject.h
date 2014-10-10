#ifndef __mdkObject_h
#define __mdkObject_h

#include <iostream>

#include "mdkDebugConfig.h"

namespace mdk
{

class Object
{
protected:
	Object();
	virtual ~Object();

public:
	void _t();
};

}//end namespace mdk

#endif
