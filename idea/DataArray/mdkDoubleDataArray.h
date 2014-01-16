#ifndef __mdkDoubleDataArray_h
#define __mdkDoubleDataArray_h

#include "mdkType.h"
#include "mdkDataArray.h"

namespace mdk
{

class mdkDoubleDataArray : public mdkDataArray
{
public:		
	mdkDoubleDataArray();
	~mdkDoubleDataArray();

	bool Allocate();

	bool CopyDataFrom(void* DataSource, mdkDataTypeEnum DataType, uint64 ElementNumber, int32 ElementDimension);

	void CopyDataFrom(double* DataSource, uint64 ElementNumber, int32 ElementDimension);

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
	mdkDoubleDataArray(const mdkDoubleDataArray&);        // Not implemented.
	void operator=(const mdkDoubleDataArray&);  // Not implemented.

};

}//end namespace mdk

#endif