#ifndef __mdkProcessObject_h
#define __mdkProcessObject_h

#include "mdkObject.h"

namespace mdk
{

class ProcessObject : public Object
{
private:

    std::vector<Object**> ProcessOutputObjectPointerList;

    std::vector<Object*> ProcessOutputObjectPointerList_SharedCopy;

protected:
    ProcessObject();
    virtual ~ProcessObject();

public:
    virtual void Clear() = 0;

    virtual void Update() = 0;

protected:
    virtual void SetupPipelineOutput();
    
    void UpdatePipelineOutput();

private:
    ProcessObject(const ProcessObject&) = delete;  
    void operator=(const ProcessObject&) = delete; 

}//end namespace mdk

#endif
