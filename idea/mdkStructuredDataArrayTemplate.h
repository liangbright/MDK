#ifndef __mdkStructuredDataArrayTemplate_h
#define __mdkStructuredDataArrayTemplate_h

#include "mdkAbstractStructuredDataArrayTemplate.h"

namespace mdk
{

// Element = [Scalar0, Scalar1, Scalar2, ...]
// ScalarNumberInElement: number of Scalars in an element
// ElementDimension :  1D, 2D,    3D ...
// ElementSize :       10, 1x10,  1x1x10

// all the Elements in StructuredDataArrayTemplate have the same ScalarNumberInElement
//
// StructuredDataArray(i,j,k,.....) = Element
// ArrayDimension:  1D,  2D,    3D, ...
// ArraySize:       10,  1x10,  1x10x10 
// ElementNumber : total number of elements in the array
// ScalarNumber:   total number of Scalars in the array

// From mdkStructuredDataArrayTemplate, wen can derive mdkMatrix(2D), mdkImage (3D+t)
 
template<typename ScalarType>	
class mdkStructuredDataArrayTemplate : public mdkAbstractStructuredDataArrayTemplate
{

protected:

	ScalarType* m_ScalarPointer;    // set by the object just before allocate() returns true

	mdkScalarTypeEnum m_ScalarType; // set by the object just before allocate() returns true

	uint64 m_ElementDimension;      // set by the object just before allocate() returns true

	uint64* m_ElementSize;          // set by the object just before allocate() returns true

	uint64 m_ScalarNumberInElement; // set by the object just before allocate() returns true

	uint64 m_ArrayDimension;        // set by the object just before allocate() returns true

	uint64* m_ArraySize;            // set by the object just before allocate() returns true

	uint64 m_ElementNumberInArray;  // set by the object just before allocate() returns true

	uint64 m_ScalarNumberInArray;   // set by the object just before allocate() returns true

public:		
	mdkStructuredDataArrayTemplate();
	~mdkStructuredDataArrayTemplate();

	bool IsEmpty();

	bool Allocate(uint64 ElementDimension, const uint64* ElementSize, uint64 ArrayDimension, const uint64* ArraySize);

	bool CopyData(void* SourceData, mdkScalarTypeEnum SourceDataType,
		          uint64 ElementDimension, const uint64* ElementSize,
		          uint64 ArrayDimension, const uint64* ArraySize);

	bool CutToCube(const uint64* StartIndex, const uint64* EndIndex);

	uint64 GetElementLinearIndex(const uint64* Position);

	void GetElementPosition(uint64 Index, uint64* Position);

	bool SetElementByLinearIndex(uint64 Index, const double* Element);

	bool SetElementByPosition(const uint64* Position, const double* Element);

	bool GetElementByLinearIndex(uint64 Index, double* Element);

	bool GetElementByPosition(const uint64* Position, double* Element);

	bool GetCube(const uint64* StartIndex, const uint64* EndIndex, mdkStructuredDataArrayTemplate<ScalarType>* OutputDataArray);

	uint64 GetScalarNumberInElement();

	uint64 GetElementDimension();

	bool GetElementSize(uint64* Size);

	uint64 GetArrayDimension();

	bool GetArraySize(uint64* ArraySize);

	uint64 GetElementNumberInArray();

	uint64 GetScalarNumberInArray();

	mdkScalarTypeEnum GetScalarType();

	ScalarType* GetScalarPointerWithType();

	void* GetScalarPointer();

	void Clear();

	//------- functions for the operatrors--------------------//
	
	// <<
	bool FillScalar(double Scalar);

	// +=
	bool AddScalar(double Scalar);

	// -=
	bool SubtractScalar(double Scalar);

	// *=
	bool MultiplyScalar(double Scalar);

	// /=
	bool DivideScalar(double Scalar);
	//<<
	bool FillElement(const double* Element);

	//+=
	bool AddElement(const double* Element);

	//-=
	bool SubtractElement(const double* Element);

	//
	bool IsStructureSame(const mdkAbstractStructuredDataArrayTemplate& SourceArray);

	//<<
	bool CopyArray(const mdkAbstractStructuredDataArrayTemplate& SourceArray);

	//+=  only valid if size is the same
	bool AddArray(const mdkAbstractStructuredDataArrayTemplate& SourceArray);

	//-= only valid if size is the same 
	bool SubtractArray(const mdkAbstractStructuredDataArrayTemplate& SourceArray);

	mdkAbstractStructuredDataArrayTemplate& operator<<(const double* Element);

	mdkAbstractStructuredDataArrayTemplate& operator+=(const double* Element);

	mdkAbstractStructuredDataArrayTemplate& operator-=(const double* Element);

	mdkAbstractStructuredDataArrayTemplate& operator<<(const mdkAbstractStructuredDataArrayTemplate& SourceArray);

	mdkAbstractStructuredDataArrayTemplate& operator+=(const mdkAbstractStructuredDataArrayTemplate& SourceArray);

	mdkAbstractStructuredDataArrayTemplate& operator-=(const mdkAbstractStructuredDataArrayTemplate& SourceArray);

private:
	void SetElementDimensionAndSize(uint64 Dim, const uint64* Size);

	void SetArrayDimensionAndSize(uint64 Dim, const uint64* Size);

private:
	mdkStructuredDataArrayTemplate(const mdkStructuredDataArrayTemplate&); // Not implemented.
	void operator= (const mdkStructuredDataArrayTemplate&);                // Not implemented.

};

}//end namespace mdk

#include "mdkStructuredDataArrayTemplate.hpp"

#endif