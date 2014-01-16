#ifndef __mdkFloatDataArray_h
#define __mdkFloatDataArray_h

#include "mdkType.h"
#include "mdkDataArray.h"

namespace mdk
{

class mdkFloatDataArray : public mdkDataArray
{

public:		
	mdkFloatDataArray();
	~mdkFloatDataArray();

	bool Allocate();

	bool Expand();

	bool Squeeze();

	void Clear();

	bool SetElement(uint64 ElementIndex, double Component0);
	bool SetElement(uint64 ElementIndex, double Component0, double Component1);
	bool SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2);
	bool SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3);
	bool SetElement(uint64 ElementIndex, int32 ComponentIndex, double Component);

	bool InsertElement(uint64 ElementIndex, double Component0);
	bool InsertElement(uint64 ElementIndex, double Component0, double Component1);
	bool InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2);
	bool InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3);
	bool InsertElement(uint64 ElementIndex, int32 ComponentIndex, double Component);

private:
	mdkFloatDataArray(const mdkFloatDataArray&);        // Not implemented.
	void operator=(const mdkFloatDataArray&);  // Not implemented.

};

}//end namespace mdk

#endif