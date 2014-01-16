#ifndef __mdkDataArray_h
#define __mdkDataArray_h

#include "mdkDataObject.h"

namespace mdk
{

// Element = [Component0, Component1, Component2, ...]
// ElementLength: number of components in an element
// all the Elements in DataArray have the same ElementLength
//
// DataArray = [Element0, Element1, Element2, ElementX, ElementY0, ElementY1...]
// [Element0, ..., ElementX] are effective elements
// [ElementY0, ElementY1, ...] are element-holders after DataArray expantion.

// From mdkDataArray, wen can derive mdkMatrix(2D), mdkImage (3D+t)
 
template<typename ScalarType>	
class mdkDataArray : public mdkDataObject
{

protected:

	void* m_ScalarPointer;

	uint64 m_ElementLength;

	uint64 m_ElementNumber;

	uint64 m_EffectiveElementNumber;

	uint64 m_ExpantionNumber;

private:
	ScalarType m_ScalarTypeData;

public:		
	mdkDataArray();
	~mdkDataArray();

	bool SetElementLength(uint64 Length);

	bool SetElementNumber(uint64 Number);

	bool IsEmpty();

	bool Allocate();

	bool CopyData(void* SourceData, mdkScalarTypeEnum SourceDataType,
		          uint64 ElementLength, uint64 ElementNumber);

	bool SetElement(uint64 ElementIndex, const double* Element);

	bool InsertElement(uint64 ElementIndex, const double* Element);

	bool Expand();

	bool Squeeze();

	bool Cut(uint64 StartElementIndex, uint64 EndElementIndex);

	bool GetElement(uint64 ElementIndex, double* Element);

	uint64 GetElementLength();

	uint64 GetElementNumber();

	uint64 GetEffectiveElementNumber();

	mdkScalarTypeEnum GetScalarType();

	void* GetScalarPointer();

	void Clear();

private:
	mdkDataArray(const mdkDataArray&);          // Not implemented.
	void operator= (const mdkDataArray&);  // Not implemented.

};

}//end namespace mdk

#include "mdkDataArray.hpp"

#endif