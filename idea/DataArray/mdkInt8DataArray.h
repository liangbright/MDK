#ifndef __mdkInt8DataArray_h
#define __mdkInt8DataArray_h

#include "mdkType.h"
#include "mdkDataArray.h"

namespace mdk
{

class mdkInt8DataArray : public mdkDataArray
{
public:		
	mdkInt8DataArray();
	~mdkInt8DataArray();

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
	mdkInt8DataArray(const mdkInt8DataArray&);        // Not implemented.
	void operator=(const mdkInt8DataArray&);  // Not implemented.

};

}//end namespace mdk

#endif