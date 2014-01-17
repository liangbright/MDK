#ifndef __mdkDataArrayTemplate_h
#define __mdkDataArrayTemplate_h

#include "mdkAbstractDataArray.h"

namespace mdk
{

// Element = [Scalar0, Scalar1, Scalar2, ...]
// ScalarNumberInElement: number of Scalars in an element
// all the Elements in DataArrayTemplate have the same ScalarNumberInElement
//
// DataArrayTemplate = [Element0, Element1, Element2, ElementX, ElementY0, ElementY1...]
// [Element0, ..., ElementX] are effective elements
// [ElementY0, ElementY1, ...] are element-holders after DataArrayTemplate expantion.

// From mdkDataArrayTemplate, wen can derive mdkMatrix(2D), mdkImage (3D+t)
 
template<typename ScalarType>	
class mdkDataArrayTemplate : public mdkAbstractDataArray
{

protected:

	ScalarType* m_ScalarPointer;

	uint64 m_ScalarNumberInElement;

	uint64 m_ElementNumber;

	uint64 m_EffectiveElementNumber;

	uint64 m_ExpantionNumber;

private:
	ScalarType m_ScalarTypeData;

public:		
	mdkDataArrayTemplate();
	~mdkDataArrayTemplate();

	bool SetScalarNumberInElement(uint64 Number);

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
	mdkDataArrayTemplate(const mdkDataArrayTemplate&);          // Not implemented.
	void operator= (const mdkDataArrayTemplate&);  // Not implemented.

};

}//end namespace mdk

#include "mdkDataArrayTemplate.hpp"

#endif