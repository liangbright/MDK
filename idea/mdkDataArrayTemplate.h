#ifndef __mdkDataArrayTemplate_h
#define __mdkDataArrayTemplate_h

#include "mdkType.h"
#include "mdkDataObject.h"

namespace mdk
{

// Element = [Component0, Component1, Component2, ...]
// ElementDimension: number of components in an element
// all the Elements in DataArray have the same ElementDimension
//
// DataArray = [Element0, Element1, Element2, ElementX, ElementY0, ElementY1...]
// [Element0, ..., ElementX] are effective elements
// [ElementY0, ElementY1, ...] are element-holders after DataArray expantion.


template<typename ScalarType>	
class mdkDataArrayTemplate : public mdkDataObject
{

private:

	void* m_ScalarPointer;

	uint64 m_ElementDimension;

	uint64* m_ElementSize;

	uint64 m_ArrayDimension;

	uint64* m_ArraySize;

	ScalarType m_ScalarTypeData;

public:		
	mdkDataArrayTemplate();
	~mdkDataArrayTemplate();

	uint64 GetElementDimension(); // 1, 2, 3, ...

	bool GetElementSize(uint64* Size);     // 1x2, 3x3, ...

	uint64 GetElementNumber();

	uint64 GetArrayDimension();

	bool GetArraySize(uint64* Size);

	mdkScalarTypeEnum GetScalarType();

	uint64 GetElementScalarNumber();

	uint64 GetScalarNumber();

	void* GetScalarPointer();
	
	bool IsEmpty();

	uint64 CalScalarNumber(uint64 ElementDimension, const uint64* ElementSize, uint64 ArrayDimension, const uint64* ArraySize);

	bool Allocate(uint64 ElementDimension, const uint64* ElementSize, uint64 ArrayDimension, const uint64* ArraySize);

	bool CopyData(void* SourceData, mdkScalarTypeEnum SourceDataType,
		          uint64 ElementDimension, const uint64* ElementSize,
				  uint64 ArrayDimension,   const uint64* ArraySize);

	void Clear();

	uint64 CalElementLinearIndex(const uint64* ElementPosition);

	bool SetElementByPosition(const uint64* ElementPosition, const double* Element);

	bool SetElementByLinearIndex(uint64 ElementLinearIndex, const double* Element);

	bool GetElementByPosition(const uint64* ElementPosition, double* Element);

	bool GetElementByLinearIndex(uint64 ElementLinearIndex, double* Element);

private:
	
	void SetElementDimensionAndSize(uint64 ElementDimension, const uint64* ElementSize);

	void SetArrayDimensionAndSize(uint64 ArrayDimension, const uint64* ArraySize);

private:
	mdkDataArrayTemplate(const mdkDataArrayTemplate&);          // Not implemented.
	void operator= (const mdkDataArrayTemplate&);  // Not implemented.

};

}//end namespace mdk

#include "mdkDataArrayTemplate.hpp"

#endif