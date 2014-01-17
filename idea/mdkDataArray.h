#ifndef __mdkDataArray_h
#define __mdkDataArray_h

#include "mdkAbstractDataArray.h"

namespace mdk
{

// Element = [Component0, Component1, Component2, ...]
// ScalarNumberInElement: number of components in an element
// all the Elements in DataArray have the same ScalarNumberInElement
//
// DataArray = [Element0, Element1, Element2, ElementX, ElementY0, ElementY1...]
// [Element0, ..., ElementX] are effective elements
// [ElementY0, ElementY1, ...] are element-holders after DataArray expantion.

// From mdkDataArray, wen can derive mdkMatrix(2D), mdkImage (3D+t)
 
	
class mdkDataArray : public mdkAbstractDataArray
{

private:
	mdkAbstractDataArray* m_DataArray;

	mdkScalarTypeEnum m_ScalarType;

	mdkScalarTypeEnum m_ScalarType_Input;

	uint64 m_ScalarNumberInElement_Input;

	uint64 m_ElementNumber_Input;

public:		
	mdkDataArray();
	~mdkDataArray();

	bool SetScalarType(mdkScalarTypeEnum ScalarType);

	bool SetScalarNumberInElement(uint64 Length);

	bool SetElementNumber(uint64 Number);

	bool IsEmpty();

	bool Allocate();

	bool CopyData(void* SourceData, mdkScalarTypeEnum SourceDataType,
		          uint64 ScalarNumberInElement, uint64 ElementNumber);

	bool SetElement(uint64 ElementIndex, const double* Element);

	bool InsertElement(uint64 ElementIndex, const double* Element);

	bool Expand();

	bool Squeeze();

	bool Cut(uint64 StartElementIndex, uint64 EndElementIndex);

	bool GetElement(uint64 ElementIndex, double* Element);

	uint64 GetScalarNumberInElement();

	uint64 GetElementNumber();

	uint64 GetEffectiveElementNumber();

	mdkScalarTypeEnum GetScalarType();

	void* GetScalarPointer();

	void Clear();

private:
	bool Initialize();

private:
	mdkDataArray(const mdkDataArray&);          // Not implemented.
	void operator= (const mdkDataArray&);  // Not implemented.

};

}//end namespace mdk

#endif