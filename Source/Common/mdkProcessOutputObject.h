#ifndef __mdkProcessOutputObject_h
#define __mdkProcessOutputObject_h

#include "mdkObject.h"

namespace mdk
{
class ProcessOutputObject : Object
{

protected:
    ProcessOutputObject();
    virtual ~ProcessOutputObject();

public:

    virtual void ShallowCopy(const ProcessOutputObject* Input);
};

}//end namespace mdk

#endif
