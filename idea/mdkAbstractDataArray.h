#ifndef __mdkAbstractDataArray_h
#define __mdkAbstractDataArray_h

#include "mdkObject.h"

namespace mdk
{

// Element = [Component0, Component1, Component2, ...]
// ScalarNumberInElement: number of components in an element
// all the Elements in AbstractDataArray have the same ElementLength
//
// AbstractDataArray = [Element0, Element1, Element2, ElementX, ElementY0, ElementY1...]
// [Element0, ..., ElementX] are effective elements
// [ElementY0, ElementY1, ...] are element-holders after AbstractDataArray expantion.

// From mdkAbstractDataArray, wen can derive mdkMatrix(2D), mdkImage (3D+t)
 
class mdkAbstractDataArray : public mdkObject
{

public:		
	mdkAbstractDataArray();
	~mdkAbstractDataArray();

	virtual bool SetScalarNumberInElement(uint64 Number) = 0;

	virtual bool SetElementNumber(uint64 Number) = 0;

	virtual bool IsEmpty() = 0;

	virtual bool Allocate() = 0;

	virtual bool CopyData(void* SourceData, mdkScalarTypeEnum SourceDataType,
		                  uint64 ScalarNumberInElement, uint64 ElementNumber) = 0;

	virtual bool SetElement(uint64 ElementIndex, const double* Element) = 0;

	virtual bool InsertElement(uint64 ElementIndex, const double* Element) = 0;

	virtual bool Expand() = 0;

	virtual bool Squeeze() = 0;

	virtual bool Cut(uint64 StartElementIndex, uint64 EndElementIndex) = 0;

	virtual bool GetElement(uint64 ElementIndex, double* Element) = 0;

	virtual uint64 GetScalarNumberInElement() = 0;

	virtual uint64 GetElementNumber() = 0;

	virtual uint64 GetEffectiveElementNumber() = 0;

	virtual mdkScalarTypeEnum GetScalarType() = 0;

	virtual void* GetScalarPointer() = 0;

	virtual void Clear() = 0;

private:
	mdkAbstractDataArray(const mdkAbstractDataArray&);          // Not implemented.
	void operator= (const mdkAbstractDataArray&);  // Not implemented.

};

}//end namespace mdk

#endif