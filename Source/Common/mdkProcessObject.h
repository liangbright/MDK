#ifndef __mdkProcessObject_h
#define __mdkProcessObject_h

#include "mdkObject.h"
#include "mdkDebugConfig.h"

namespace mdk
{
// two types of ProcessObject
// type-1 : output data is NOT stored in ProcessObject, and shared copy of output is used
// type-2 : output data is stored in ProcessObject, and shared copy of output is NOT used

class ProcessObject : public Object
{
protected:
    ProcessObject() {}
    virtual ~ProcessObject() {}

public:
    virtual void Clear() = 0;  // set to default/initial state

    virtual bool Update() = 0; // run the process and update everything (output or parameter)

protected:
    // this may have different names in different operation modes, e.g., CheckInput_Mode_1(), CheckInput_Mode_2()
    // put it here just for reminder
    //virtual bool CheckInput() = 0; // check the inputs to the process
                                     // not only "input" data, but also "input" variables that store output results
                                     // this is called in the first step of Update()

	virtual void ClearOutput() {}      // this should be called in Clear()

	virtual void UpdateOutputPort() {} // this should be called in the last step of Update() for type-2 ProcessObject only

private:
    ProcessObject(const ProcessObject&) = delete;  
    void operator=(const ProcessObject&) = delete; 
};

}//end namespace mdk

#endif

