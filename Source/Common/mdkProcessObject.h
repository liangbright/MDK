#ifndef __mdkProcessObject_h
#define __mdkProcessObject_h

#include "mdkObject.h"
#include "mdkDebugConfig.h"


namespace mdk
{

class ProcessObject : public Object
{
protected:
    ProcessObject() {}
    virtual ~ProcessObject() {}

public:
    virtual void Clear() = 0; // set to default/initial state

    virtual bool Update() = 0; // run the process and update output

protected:
    virtual void SetupDefaultPipelineOutput() = 0; // this is usually called in Clear()
    virtual void UpdatePipelineOutput() = 0;       // this is usually called in Update()

private:
    ProcessObject(const ProcessObject&) = delete;  
    void operator=(const ProcessObject&) = delete; 
};

}//end namespace mdk

#endif
