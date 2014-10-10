#ifndef __mdkProcessObject_h
#define __mdkProcessObject_h

#include "mdkObject.h"

namespace mdk
{
// ProcessObject can be a SISO function y=f_p_(x), x is input, y is output, p is parameter
// ProcessObject can be a MIMO function [y1, y2]=f_p_(x1, x2), or SIMO, MISO
// MIMO example: ImageFilter3D
//
// Parameter Estimator can be a member function of the object
// e.g.: AffineTransform
//
// Each Input x must NOT be member variable, and must be pointer
// Each Output y must be member variable
// Parameter p may or may not be member variable 

class ProcessObject : public Object
{
protected:
    ProcessObject() {}
    virtual ~ProcessObject() {}

public:
    virtual void Clear() = 0;         // set to default/initial state
    virtual bool Update() = 0;        // run the process and update everything
	
//suggestion:
	// CheckInput() : called in the first step of Update()
	// this may have different names in different operation modes, e.g., CheckInput_Mode_1(), CheckInput_Mode_2()
	//
	// void ClearOutput() : called in Clear()

private:
    ProcessObject(const ProcessObject&) = delete;  
    void operator=(const ProcessObject&) = delete; 
};

}//end namespace mdk

#endif

