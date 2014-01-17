#ifndef __mdkStructuredDataArray_h
#define __mdkStructuredDataArray_h

#include "mdkAbstractStructuredDataArrayTemplate.h"

namespace mdk
{

// Element = [Scalar0, Scalar1, Scalar2, ...]
// ScalarNumberInElement: number of Scalars in an element
// ElementDimension :  1D, 2D,    3D ...
// ElementSize :       10, 1x10,  1x1x10

// all the Elements in StructuredDataArray have the same ScalarNumberInElement
//
// StructuredDataArray(i,j,k,.....) = Element
// ArrayDimension:  1D,  2D,    3D, ...
// ArraySize:       10,  1x10,  1x10x10 
// ElementNumber : total number of elements in the array
// ScalarNumber:   total number of Scalars in the array

// From mdkStructuredDataArray, wen can derive mdkMatrix(2D), mdkImage (3D+t)

class mdkStructuredDataArray : public mdkObject
{

protected:
	mdkAbstractStructuredDataArrayTemplate* m_DataArray;

	mdkScalarTypeEnum m_ScalarType;

public:		
	mdkStructuredDataArray();
	~mdkStructuredDataArray();

	bool Allocate(mdkScalarTypeEnum ScalarType, 
		          uint64 ElementDimension, const uint64 ElementSize[], 
				  uint64 ArrayDimension,   const uint64 ArraySize[]);

	bool CopyData(void* SourceData, mdkScalarTypeEnum SourceScalarType,
		          uint64 ElementDimension, const uint64 ElementSize[],
		          uint64 ArrayDimension,   const uint64 ArraySize[]);

	bool CutToCube(const uint64 StartIndex[], const uint64 EndIndex[]);

	bool SetElementByLinearIndex(uint64 Index, const double Element[]);

	bool SetElementByPosition(const uint64 Position[], const double Element[]);

	bool GetCube(const uint64 StartIndex[], const uint64 EndIndex[], mdkStructuredDataArray* OutputDataArray) const;

	bool GetElementByLinearIndex(uint64 Index, double Element[]) const;

	bool GetElementByPosition(const uint64 Position[], double Element[]) const;

	uint64 GetScalarNumberInElement() const;

	uint64 GetElementDimension() const;

	bool GetElementSize(uint64 Size[]) const;

	uint64 GetArrayDimension() const;

	bool GetArraySize(uint64 ArraySize[]) const;

	uint64 GetElementNumberInArray() const;

	uint64 GetScalarNumberInArray() const;

	mdkScalarTypeEnum GetScalarType() const;

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
	bool FillElement(const double Element[]);

	//+=
	bool AddElement(const double Element[]);

	//-=
	bool SubtractElement(const double Element[]);

	// the following 4 functions are not compatible 
	// with similar operators in mdkAbstractStructuredDataArrayTemplate
	// Therefore, this class should not be derived from mdkAbstractStructuredDataArrayTemplate
	// Then, should not use mdkStructuredDataArray and mdkStructuredDataArrayTemplate together
	// Only use mdkStructuredDataArray and its derived classes

	bool IsStructureSame(mdkStructuredDataArray& SourceArray) const;

	//<<  only valid if size is the same
	bool CopyArray(mdkStructuredDataArray& SourceArray);

	//+=  only valid if size is the same
	bool AddArray(mdkStructuredDataArray& SourceArray);

	//-=  only valid if size is the same
	bool SubtractArray(mdkStructuredDataArray& SourceArray);

public:
	mdkStructuredDataArray& operator<<(double Scalar);

	mdkStructuredDataArray& operator+=(double Scalar);

	mdkStructuredDataArray& operator-=(double Scalar);

	mdkStructuredDataArray& operator*=(double Scalar);

	mdkStructuredDataArray& operator/=(double Scalar);

	mdkStructuredDataArray& operator<<(const double Element[]);

	mdkStructuredDataArray& operator+=(const double Element[]);

	mdkStructuredDataArray& operator-=(const double Element[]);

	// the following 3 operators are not compatible 
	// with similar operators in mdkAbstractStructuredDataArrayTemplate
	// Therefore, this class should not be derived from mdkAbstractStructuredDataArrayTemplate
	// Then, should not use mdkStructuredDataArray and mdkStructuredDataArrayTemplate together
	// Only use mdkStructuredDataArray and its derived classes

	mdkStructuredDataArray& operator<<(mdkStructuredDataArray& SourceArray);

	mdkStructuredDataArray& operator+=(mdkStructuredDataArray& SourceArray);

	mdkStructuredDataArray& operator-=(mdkStructuredDataArray& SourceArray);

protected:
	mdkAbstractStructuredDataArrayTemplate* GetDataArrayPointer();

private:
	bool InitializeDataArrayWithType(mdkScalarTypeEnum ScalarType);

private:
	mdkStructuredDataArray(const mdkStructuredDataArray&); // Not implemented.
	void operator= (const mdkStructuredDataArray&);                // Not implemented.

};

}//end namespace mdk

#endif