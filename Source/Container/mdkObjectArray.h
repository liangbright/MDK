#pragma once

#include <vector>
#include <memory>

#include "mdkCommonType.h"
#include "mdkDebugConfig.h"
#include "mdkConstant.h"
#include "mdkObject.h"
#include "mdkDenseVector_ForwardDeclare.h"
#include "mdkString.h"

namespace mdk
{

#if defined MDK_DEBUG_MODE
    #define MDK_DEBUG_ObjectArray_Operator_CheckBound
#endif

// forward declare ------------------------------
template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class StdObjectVector;

template<typename ElementType>
class ObjectArray;

// another ClassName : ObjectVector
template<typename ElementType>
using ObjectVector = ObjectArray<ElementType>;
//--------------------------------------------------

// ----------------------------- ObjectArrayData struct -------------------------------------------------------------//

template<typename ElementType>
struct ObjectArrayData
{
	// data is stored in StdVector or external data array pointed by ElementPointer
    std::vector<ElementType> StdVector;

	int_max Length; // array length
	ElementType* ElementPointer; // ElementPointer point to external data array or StdVector.data()

	ElementType ErrorElement;

    bool IsSizeFixed;

//-------------------------------------------------------------
    ObjectArrayData() 
    {
        Length = 0;
        ElementPointer = nullptr;
        IsSizeFixed = false;
        ErrorElement = GetNaNElement(ErrorElement);
    };

	ObjectArrayData(const ObjectArrayData& InputData)
	{
		this->Copy(InputData);
	}

	ObjectArrayData(ObjectArrayData&& InputData)
	{
		this->Copy(std::move(InputData));
	}

    ~ObjectArrayData() {};
	//-------------------------------------------------------------
	void operator=(const ObjectArrayData& InputData)
	{
		this->Copy(InputData);
	}
	//-------------------------------------------------------------
	void operator=(ObjectArrayData&& InputData)
	{
		this->Copy(std::move(InputData));
	}
	//-------------------------------------------------------------
	void operator=(const MDK_Symbol_Empty&)
	{
		this->Copy(MDK_EMPTY_DATA_OBJECT);
	}
	//-------------------------------------------------------------
	void Copy(const MDK_Symbol_Empty&)
	{
		if (IsSizeFixed == true)
		{
			if (Length > 0)
			{
				MDK_Error("Can not change size @ ObjectArrayData::Copy(MDK_EMPTY_DATA_OBJECT)")
				return;
			}
		}
		this->Clear();
	}
	//-------------------------------------------------------------
	void Copy(const ObjectArrayData& InputData)
	{
		this->Copy(InputData.ElementPointer, InputData.Length);
	}
	//-------------------------------------------------------------
	void Copy(const ElementType* InputElementPointer, int_max InputLength)
	{
		if (InputElementPointer == nullptr || InputLength <= 0)
		{
			if (this->IsEmpty() == true)
			{
				return;
			}
			else
			{
				if (IsSizeFixed == true)
				{
					MDK_Error("Can not change size @ ObjectArrayData::Copy(...)")
					return;
				}

				this->Clear();
				return;
			}
		}

		if (this->IsEmpty() == false)
		{
			if (InputElementPointer == ElementPointer)
			{// self copy
				return;
			}
		}
		//------------------------------------------------------------------
		if (IsSizeFixed == true)
		{
			if (InputLength != Length)
			{
				MDK_Error("Can not change size @ ObjectArrayData::Copy(...)")
				return;
			}
		}
		else
		{
			this->FastResize(InputLength);
		}

		//copy data ----------------------------------------------------------
		auto tempPtr = InputElementPointer;
		for (auto Ptr = ElementPointer; Ptr < ElementPointer + Length; ++Ptr, ++tempPtr)
		{
			Ptr[0] = tempPtr[0];
		}
	}
	//-------------------------------------------------------------
	void Copy(ObjectArrayData&& InputData)
	{
		if (this == &InputData)
		{
			MDK_Warning("Self copy itself @ ObjectArrayData::Copy(...)")
			return;
		}

		if (IsSizeFixed == true)
		{
			if (InputData.Length != Length)
			{
				MDK_Error("Size does not match @ ObjectArray::Copy(...)")
				return;
			}
		}

		if (InputData.IsEmpty() == true)
		{
			if (Length > 0)
			{
				this->Clear();
			}
			return;
		}

		if (ElementPointer == InputData.ElementPointer)
		{// self == self
			return;
		}

		// now, InputData is not empty, and is not self

		StdVector = std::move(InputData.StdVector);
		ElementPointer = InputData.ElementPointer;
		Length = InputData.Length;

		// Clear InputData to be empty
		InputData.Clear();
	}
	//-------------------------------------------------------------
	void Clear()
	{
		IsSizeFixed = false;
		Length = 0;
		ElementPointer = nullptr;
		StdVector.clear();         // change size
		StdVector.shrink_to_fit(); // release memory
	}
	//-------------------------------------------------------------
	bool IsEmpty() const
	{
		return (Length <= 0);
	}
	//-------------------------------------------------------------
	void Resize(int_max InputLength)
	{
		if (InputLength < 0)
		{
			MDK_Error("Invalid Input @ ObjectArrayData::Resize(...)")
			return;
		}

		try
		{
			if (InputLength == Length)
			{
				return;
			}

			if (IsSizeFixed == true)
			{
				MDK_Error("Size can not be changed @ ObjectArrayData::Resize(...)")
				return;
			}

			this->CopyDataToStdVectorIfNecessary();

			StdVector.resize(InputLength);
			ElementPointer = StdVector.data();
			Length = InputLength;
		}
		catch (...)
		{
			MDK_Error("Out of Memory @ ObjectArrayData::Resize(...)")
			ElementPointer = StdVector.data();
			Length = int_max(StdVector.size());
		}
	}
	//-------------------------------------------------------------
	void FastResize(int_max InputLength)
	{
		if (InputLength < 0)
		{
			MDK_Error("Invalid input @ ObjectArrayData::FastResize(...)")
			return;
		}

		if (InputLength == Length)
		{
			return;
		}

		if (IsSizeFixed == true)
		{
			MDK_Error("Can not change size @ ObjectArrayData::FastResize(...)")
			return;
		}

		try
		{
			if (InputLength != Length)
			{
				if (InputLength > int_max(StdVector.capacity()))
				{
					StdVector.clear();
				}

				StdVector.resize(InputLength);
			}

			ElementPointer = StdVector.data();
			Length = int_max(StdVector.size());//==InputLength
		}
		catch (...)
		{
			MDK_Error("Out of Memory @ ObjectArrayData::FastResize(...)")

			ElementPointer = StdVector.data();
			Length = int_max(StdVector.size());
		}
	}
    //-------------------------------------------------------------
    void CopyDataToStdVectorIfNecessary()
    {
		if (ElementPointer != StdVector.data())
        {
			if (ElementPointer == nullptr)
            {
				if (Length != 0 || StdVector.data() != nullptr || StdVector.size() != 0)
				{
					MDK_Error("ElementPointer is nullptr but Self is not empty @ ObjectArrayData::CopyDataToStdVectorIfNecessary()")
				}
				return;
            }

			StdVector.resize(Length);
            for (int_max i = 0; i < Length; ++i)
            {
				StdVector[i] = ElementPointer[i];
            }
			ElementPointer = StdVector.data();
        }
    }
	//-------------------------------------------------------------
    ElementType& operator[](int_max Index)
    {
        return ElementPointer[Index];
    }

    const ElementType& operator[](int_max Index) const
    {
        return ElementPointer[Index];
    }

    ElementType& operator()(int_max Index)
    {
        return ElementPointer[Index];
    }

    const ElementType& operator()(int_max Index) const
    {
        return ElementPointer[Index];
    }
};

//----------------------------------------------------------------------------------------------------------------------------//

template<typename Element_Type>
class ObjectArray : public Object
{
public:
	typedef Element_Type ElementType;
	
protected:
	std::shared_ptr<ObjectArrayData<ElementType>> m_Data;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline ObjectArray();
    inline ObjectArray(const ObjectArray<ElementType>& InputArray);
    inline ObjectArray(ObjectArray<ElementType>&& InputArray);

	inline ObjectArray(const StdObjectVector<ElementType>& InputArray);
	inline ObjectArray(const std::initializer_list<ElementType>& InputArray);
	inline ObjectArray(const std::initializer_list<const ObjectArray<ElementType>*>& InputList);

	inline ~ObjectArray();

	//-------------------- StdVector -------------------------------------------//
	// std::vector<ElementType>& StdVector() {return m_Data->StdVector;}
	// const std::vector<ElementType>& StdVector() const {return m_Data->StdVector;}
	// Do NOT implement the two function:
	// if data is in external array, then StdVector() should move data to internal array: user may not want this

	//----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const ObjectArray<ElementType>& InputArray);
    inline void operator=(ObjectArray<ElementType>&& InputArray);

	inline void operator=(const StdObjectVector<ElementType>& InputArray);
	inline void operator=(const std::initializer_list<ElementType>& InputList);
	inline void operator=(const std::initializer_list<const ObjectArray<ElementType>*>& InputList);

    //----------------------  Copy  ----------------------------------------//

	inline void Copy(const ObjectArray<ElementType>& InputArray);
	inline void Copy(ObjectArray<ElementType>&& InputArray);

	inline void Copy(const StdObjectVector<ElementType>& InputArray);
	inline void Copy(const ElementType* InputElementPointer, int_max InputLength);

	inline void Fill(const ElementType& Element);

    //-------------------------- Shared, ForceShare  ------------------------------------------ //

    inline void Share(ObjectArray<ElementType>& InputArray);
    inline void ForceShare(const ObjectArray<ElementType>& InputArray);

    //special Share : external data
	inline void Share(ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed = true);
	inline void ForceShare(const ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed = true);

    //------------------------- Swap shared_ptr m_Data -------------------------------------------//

    inline void Swap(ObjectArray<ElementType>& InputArray);

    //------------------------- Clear -------------------------------------------//

    inline void Clear();

	//------------------------- Recreate -------------------------------------------//
	inline void Recreate();

	//---------------------- Set/get Size ----------------------------------------//

	inline void Resize(int_max InputLength); // try to keep the old data
	inline void FastResize(int_max InputLength); // do not care about old data

	inline void SetCapacity(int_max InputElementNumber); // reserve memory, current Length does not change
    inline void ReleaseUnusedCapacity();

    inline void FixSize();

    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;

    inline bool IsShared() const;
	inline bool IsSharedWith(const ObjectArray& InputArray) const;

	inline bool IsDataInInternalArray() const;
	//attention: error if self is pure_empty, always use IsDataInInternalArray() before the following two funciton
	inline std::vector<ElementType>& InternalArray();
	inline const std::vector<ElementType>& InternalArray() const;

    inline int_max GetLength() const;
    inline int_max GetElementCount() const; // the same as GetLength();

    //------------------------ Error Element -----------------------------//

	void SetErrorElement(ElementType Error);
    ElementType GetErrorElement()  const;

    //--------------------- Get Data Pointer -----------------------------//

    inline ElementType* GetElementPointer(); //  the pointer of the first element
    inline const ElementType* GetElementPointer() const;

    inline ElementType* GetPointer(); //  the pointer of the first element
    inline const ElementType* GetPointer() const;

    inline ElementType* begin();
    inline const ElementType* begin() const;

    inline ElementType* end(); // 1 + pointer of the last element
    inline const ElementType* end() const; // 1 + pointer of the last element

	//----------- Get/Set by Index -----------------------------------//

    // operator[] or () : no bound check in release mode

    inline ElementType& operator[](int_max Index);
    inline const ElementType& operator[](int_max Index) const;

    inline ElementType& operator()(int_max Index);
    inline const ElementType& operator()(int_max Index) const;
    
    // at(): bound check

    inline ElementType& at(int_max Index);
    inline const ElementType& at(int_max Index) const;

    //-------------------------------------------------------------------------------

	inline void Append(ElementType Element);
	
	//confusion
	//ElementType is DeseVector<int_max>
	//A.append({1,2}) may be A = {1, 2} or A = {{1}, {2}}	
	//inline void Append(const std::initializer_list<ElementType>& InputArray);
	//inline void Append(const StdObjectVector<ElementType>& InputArray);
	//inline void Append(const ObjectArray<ElementType>& InputArray);
	//inline void Append(const DenseMatrix<ElementType>& InputArray);
	//template<int_max TemplateLength>
	//inline void Append(const DenseVector<ElementType, TemplateLength>& InputArray);	

	inline void Append(const ElementType* InputArray, int_max InputLength);

	inline void Delete(int_max Index);
	inline void Delete(const std::initializer_list<int_max>& IndexList);
	inline void Delete(const StdObjectVector<int_max>& IndexList);
	inline void Delete(const ObjectArray<int_max>& IndexList);
	inline void Delete(const DenseMatrix<int_max>& IndexList);

	template<int_max TemplateLength>
	inline void Delete(const DenseVector<int_max, TemplateLength>& IndexList);

	inline void Delete(const int_max* ColIndexList, int_max ListLength);

	inline void Delete(int_max Index_start, int_max Index_end);//Index_start<=Index_end

	inline void Insert(int_max Index, ElementType Element);

	//confusion
	//ElementType is DeseVector<int_max>
	//A.Insert(0, {1,2}) may be A = {1, 2} or A = {{1}, {2}}	
	//inline void Insert(int_max Index, const std::initializer_list<ElementType>& InputArray);
	//inline void Insert(int_max Index, const StdObjectVector<ElementType>& InputArray);
	//inline void Insert(int_max Index, const ObjectArray<ElementType>& InputArray);		
	//template<int_max TemplateLength>
	//inline void Insert(int_max Index, const DenseVector<ElementType, TemplateLength>& InputArray);
	
	inline void Insert(int_max Index, const ElementType* InputArray, int_max InputLength);

    //----------------------- Get a subset ------------------------------//

    inline ObjectArray<ElementType> GetSubSet(int_max Index_start, int_max Index_end);
    inline ObjectArray<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList);
	inline ObjectArray<ElementType> GetSubSet(const ObjectArray<int_max>& IndexList);
	inline ObjectArray<ElementType> GetSubSet(const StdObjectVector<int_max>& IndexList);
    inline ObjectArray<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList);	

	template<int_max TemplateLength>
	inline StdObjectVector<ElementType> GetSubSet(const DenseVector<int_max, TemplateLength>& IndexList);

    inline ObjectArray<ElementType> GetSubSet(const int_max* IndexList, int_max ListLength);

	//----------------------- Set subset ------------------------------//

	inline void SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubSet);
	inline void SetSubSet(const ObjectArray<int_max>& IndexList, const ObjectArray<ElementType>& SubSet);
	inline void SetSubSet(const StdObjectVector<int_max>& IndexList, const ObjectArray<ElementType>& SubSet);
	inline void SetSubSet(const DenseMatrix<int_max>& IndexList, const ObjectArray<ElementType>& SubSet);

	template<int_max TemplateLength>
	inline void SetSubSet(const DenseVector<int_max, TemplateLength>& IndexList, const ObjectArray<ElementType>& SubSet);

	inline void SetSubSet(const int_max* IndexList, const ElementType* SubSet, int_max SubSetElementNumber);

    //-------------------- find ---------------------------------------//

    template<typename MatchFunctionType>
    inline ObjectArray<int_max> Find(MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline ObjectArray<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline ObjectArray<int_max> Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction);

	//-------------------- ExactMatch (use operator == ) ---------------------------------------//

	inline ObjectArray<int_max> ExactMatch(const ElementType& InputElement) const;

	inline int_max ExactMatch(const String& first_or_last, const ElementType& InputElement) const;

    //--------------------- sort ---------------------------------------//

    template<typename CompareFunctionType>
    inline ObjectArray<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline ObjectArray<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

	inline ObjectArray<int_max> Sort(const char* Order) const;
	// Order: ascend or descend
	inline ObjectArray<int_max> Sort(const std::string& Order) const;

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

	inline void SortInPlace(const char* Order);
	// Order: ascend or descend
	inline void SortInPlace(const std::string& Order);

private:

};


}//end namespace mdk

#include "mdkObjectArray.hpp"

