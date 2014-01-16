#ifndef __mdkDataArray_h
#define __mdkDataArray_h

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

class mdkDataArray : public mdkDataObject
{

private:

	void* m_DataPointer;

	uint32 m_ElementDimension;  

	uint64 m_ElementNumber;  // I knonw this is not english (should be NumberOfElements), but this is the way I think

	uint64 m_EffectiveElementNumber;

	mdkDataTypeEnum m_DataType;

public:		
	mdkDataArray();
	~mdkDataArray();

	bool SetDataType(mdkDataTypeEnum DataType);

	bool SetElementDimension(uint32 Dim);

	bool SetElementNumber(uint64 Number);

	int32 GetElementDimension();

	uint64 GetElementNumber();

	virtual bool Allocate();

	virtual bool CopyData(void* SourceData, mdkDataTypeEnum SourceDataType, uint64 ElementNumber, uint32 ElementDimension);

	virtual bool Squeeze();

	virtual void Clear();

	virtual bool SetElement(uint64 ElementIndex, double Component0);
	virtual bool SetElement(uint64 ElementIndex, double Component0, double Component1);
	virtual bool SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2);
	virtual bool SetElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3);
	virtual bool SetElement(uint64 ElementIndex, const double* Element);

	virtual bool SetElementComponent(uint64 ElementIndex, uint32 ComponentIndex, double Component);

	virtual bool InsertElement(uint64 ElementIndex, double Component0);
	virtual bool InsertElement(uint64 ElementIndex, double Component0, double Component1);
	virtual bool InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2);
	virtual bool InsertElement(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3);
	virtual bool InsertElement(uint64 ElementIndex, const double* Element);

	virtual bool InsertElementComponent(uint64 ElementIndex, uint32 ComponentIndex, double Component);

	virtual bool GetElement(uint64 ElementIndex, double* Element);
	
	mdkDataTypeEnum GetDataType();

	void* GetDataPointer();

protected:
	//------------------------ template ------------//
	template<typename DataType>
	bool AllocateWithType();

	template<typename DataType>
	bool ExpandWithType(uint32 ElementNumber = 1000);

	template<typename DataType>
	bool SqueezeWithType();

	template<typename DataType>
	void ClearWithType();

	template<typename DataType>
	bool SetElementWithType(uint64 ElementIndex, double Component0);

	template<typename DataType>
	bool SetElementWithType(uint64 ElementIndex, double Component0, double Component1);
	
	template<typename DataType>
	bool SetElementWithType(uint64 ElementIndex, double Component0, double Component1, double Component2);

	template<typename DataType>
	bool SetElementWithType(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3);

	template<typename DataType>
	bool SetElementWithType(uint64 ElementIndex, const double* Element);

	template<typename DataType>
	bool SetElementComponentWithType(uint64 ElementIndex, uint32 ComponentIndex, double Component);

	template<typename DataType>
	bool InsertElementWithType(uint64 ElementIndex, double Component0);

	template<typename DataType>
	bool InsertElementWithType(uint64 ElementIndex, double Component0, double Component1);

	template<typename DataType>
	bool InsertElementWithType(uint64 ElementIndex, double Component0, double Component1, double Component2);

	template<typename DataType>
	bool InsertElementWithType(uint64 ElementIndex, double Component0, double Component1, double Component2, double Component3);

	template<typename DataType>
	bool InsertElementWithType(uint64 ElementIndex, const double* Element);

	template<typename DataType>
	bool InsertElementComponentWithType(uint64 ElementIndex, uint32 ComponentIndex, double Component);

	template<typename DataType>
	bool GetElementWithType(uint64 ElementIndex, double* Component0);

private:
	mdkDataArray(const mdkDataArray&);    // Not implemented.
	void operator=(const mdkDataArray&);  // Not implemented.

};

}//end namespace mdk

#endif