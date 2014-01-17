#ifndef __mdkAbstractStructuredDataArrayTemplate_h
#define __mdkAbstractStructuredDataArrayTemplate_h

#include "mdkObject.h"

namespace mdk
{

// Element = [Component0, Component1, Component2, ...]
// ScalarNumberInElement: number of components in an element
// all the Elements in DataArrayTemplate have the same ScalarNumberInElement
//
// DataArrayTemplate = [Element0, Element1, Element2, ElementX, ElementY0, ElementY1...]
// [Element0, ..., ElementX] are effective elements
// [ElementY0, ElementY1, ...] are element-holders after DataArrayTemplate expantion.

// From mdkDataArrayTemplate, wen can derive mdkMatrix(2D), mdkImage (3D+t)
 
class mdkAbstractStructuredDataArrayTemplate : public mdkObject
{

public:
	mdkAbstractStructuredDataArrayTemplate();
	~mdkAbstractStructuredDataArrayTemplate();

	static uint64 CalScalarNumberInElement(uint64 ElementDimension, const uint64* ElementSize);

	static uint64 CalElementNumberInArray(uint64 ArrayDimension, const uint64* ArraySize);

	static uint64 CalScalarNumberInArray(uint64 ElementDimension, const uint64* ElementSize,
		                                 uint64 ArrayDimension, const uint64 *ArraySize);

	static uint64 CalElementLinearIndex(const uint64* Position, uint64 ArrayDimension, const uint64* ArraySize);

	static void CalElementPosition(uint64 Index, uint64 ArrayDimension, const uint64* ArraySize, uint64* Position);

	virtual bool IsEmpty() = 0;

	virtual bool Allocate(uint64 ElementDimension, const uint64* ElementSize, uint64 ArrayDimension, const uint64* ArraySize) = 0;

	virtual bool CopyData(void* SourceData, mdkScalarTypeEnum SourceScalarType,
		                  uint64 ElementDimension, const uint64* ElementSize,
						  uint64 ArrayDimension,   const uint64* ArraySize) = 0;

	virtual bool CutToCube(const uint64* StartIndex, const uint64* EndIndex) = 0;

	virtual virtual uint64 GetElementLinearIndex(const uint64* Position) = 0;

	virtual void GetElementPosition(uint64 Index, uint64* Position) = 0;

	virtual bool SetElementByLinearIndex(uint64 Index, const double Element[]) = 0;

	virtual bool SetElementByPosition(const uint64* Position, const double Element[]) = 0;

	virtual bool GetElementByLinearIndex(uint64 Index, double Element[]) = 0;

	virtual bool GetElementByPosition(const uint64* Position, double Element[]) = 0;

	//virtual bool GetCube(const uint64* StartIndex, const uint64* EndIndex, AbstractStructuredDataArray* OutputDataArray) = 0;

	virtual uint64 GetScalarNumberInElement() = 0;

	virtual uint64 GetElementDimension() = 0;

	virtual bool GetElementSize(uint64* Size) = 0;

	virtual uint64 GetArrayDimension() = 0;

	virtual bool GetArraySize(uint64* ArraySize) = 0;

	virtual uint64 GetElementNumberInArray() = 0;

	virtual uint64 GetScalarNumberInArray() = 0;

	virtual mdkScalarTypeEnum GetScalarType() = 0;

	virtual void* GetScalarPointer() = 0;

	virtual void Clear() = 0;

	//------- functions for the operatrors--------------------//

	// <<
	virtual bool FillScalar(double Scalar) = 0;

	// +=
	virtual bool AddScalar(double Scalar) = 0;

	// -=
	virtual bool SubtractScalar(double Scalar) = 0;

	// *=
	virtual bool MultiplyScalar(double Scalar) = 0;

	// /=
	virtual bool DivideScalar(double Scalar) = 0;

	// <<
	virtual bool FillElement(const double Element[]) = 0;

	// +=
	virtual bool AddElement(const double Element[]) = 0;

	// -=
	virtual bool SubtractElement(const double Element[]) = 0;

	//
	virtual bool IsStructureSame(const mdkAbstractStructuredDataArrayTemplate& SourceArray) = 0;

	// <<
	virtual bool CopyArray(const mdkAbstractStructuredDataArrayTemplate& SourceArray) = 0;

	// +=  only valid if size is the same
	virtual bool AddArray(const mdkAbstractStructuredDataArrayTemplate& SourceArray) = 0;

	// -= only valid if size is the same 
	virtual bool SubtractArray(const mdkAbstractStructuredDataArrayTemplate& SourceArray) = 0;

	mdkAbstractStructuredDataArrayTemplate& operator<<(double Scalar);

	mdkAbstractStructuredDataArrayTemplate& operator+=(double Scalar);

	mdkAbstractStructuredDataArrayTemplate& operator-=(double Scalar);

	mdkAbstractStructuredDataArrayTemplate& operator*=(double Scalar);

	mdkAbstractStructuredDataArrayTemplate& operator/=(double Scalar);

	mdkAbstractStructuredDataArrayTemplate& operator<<(const double Element[]);

	mdkAbstractStructuredDataArrayTemplate& operator+=(const double Element[]);

	mdkAbstractStructuredDataArrayTemplate& operator-=(const double Element[]);

	mdkAbstractStructuredDataArrayTemplate& operator<<(const mdkAbstractStructuredDataArrayTemplate& SourceArray);

	mdkAbstractStructuredDataArrayTemplate& operator+=(const mdkAbstractStructuredDataArrayTemplate& SourceArray);

	mdkAbstractStructuredDataArrayTemplate& operator-=(const mdkAbstractStructuredDataArrayTemplate& SourceArray);

private:
	mdkAbstractStructuredDataArrayTemplate(const mdkAbstractStructuredDataArrayTemplate&); // Not implemented.
	void operator= (const mdkAbstractStructuredDataArrayTemplate&);                // Not implemented.

};

}//end namespace mdk

#endif