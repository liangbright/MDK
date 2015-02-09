#ifndef mdk_ObjectArray_h
#define mdk_ObjectArray_h

#include <vector>
#include <memory>

#include "mdkCommonType.h"
#include "mdkDebugConfig.h"
#include "mdkConstant.h"
#include "mdkSharedDataObject.h"
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
	bool Copy(const MDK_Symbol_Empty&)
	{
		if (IsSizeFixed == true)
		{
			if (Length > 0)
			{
				MDK_Error("Can not change size @ ObjectArrayData::Copy(...)")
				return false;
			}
		}
		this->Clear();
		return true;
	}
	//-------------------------------------------------------------
	bool Copy(const ObjectArrayData& InputData)
	{
		return this->Copy(InputData.ElementPointer, InputData.Length);
	}
	//-------------------------------------------------------------
	bool Copy(const ElementType* InputElementPointer, int_max InputLength)
	{
		if (InputElementPointer == nullptr || InputLength <= 0)
		{
			if (this->IsEmpty() == true)
			{
				return true;
			}
			else
			{
				if (IsSizeFixed == true)
				{
					MDK_Error("Can not change size @ ObjectArrayData::Copy(...)")
					return false;
				}

				this->Clear();
				return true;
			}
		}

		if (this->IsEmpty() == false)
		{
			if (InputElementPointer == ElementPointer)
			{// self copy
				return true;
			}
		}
		//------------------------------------------------------------------
		if (IsSizeFixed == true)
		{
			if (InputLength != Length)
			{
				MDK_Error("Can not change size @ ObjectArrayData::Copy(...)")
				return false;
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

		return true;
	}
	//-------------------------------------------------------------
	bool Copy(ObjectArrayData&& InputData)
	{
		if (this == &InputData)
		{
			//MDK_Warning("Self take itself @ ObjectArrayData::Take(...)")
			return true;
		}

		if (IsSizeFixed == true)
		{
			if (InputData.Length != Length)
			{
				MDK_Error("Size does not match @ ObjectArray::Take(InputArray)")
				return false;
			}
		}

		if (InputData.IsEmpty() == true)
		{
			if (Length > 0)
			{
				this->Clear();
			}
			return true;
		}

		if (ElementPointer == InputData.ElementPointer)
		{// self == self
			return true;
		}

		// now, InputData is not empty, and is not self

		StdVector = std::move(InputData.StdVector);
		ElementPointer = InputData.ElementPointer;
		Length = InputData.Length;

		// Clear InputData to be empty
		InputData.Clear();

		return true;
	}
	//-------------------------------------------------------------
	bool IsReadyToShare(const ObjectArrayData& InputData) const
    {
		if (IsSizeFixed == true)
		{
			return (Length == InputData.Length);
		}
		else
		{
			return true;
		}
	}
	//-------------------------------------------------------------
	bool IsReadyToShare(const MDK_Symbol_Empty&) const
	{
		if (IsSizeFixed == true)
		{
			return (Length == 0);
		}
		else
		{
			return true;
		}
	}
	//-------------------------------------------------------------
	bool IsSharedWith(const ObjectArrayData& InputData) const
	{
		if (ElementPointer != nullptr && InputData.ElementPointer != nullptr)
		{
			return ElementPointer == InputData.ElementPointer;
		}
		return false;
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
	bool Resize(int_max InputLength)
	{
		if (InputLength < 0)
		{
			MDK_Error("Invalid Input @ ObjectArrayData::Resize(...)")
			return false;
		}

		try
		{
			if (InputLength == Length)
			{
				return true;
			}

			if (IsSizeFixed == true)
			{
				MDK_Error("Size can not be changed @ ObjectArrayData::Resize(...)")
				return false;
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
			return false;
		}
		return true;
	}
	//-------------------------------------------------------------
	bool FastResize(int_max InputLength)
	{
		if (InputLength < 0)
		{
			MDK_Error("Invalid input @ ObjectArrayData::FastResize(...)")
			return false;
		}

		if (InputLength == Length)
		{
			return true;
		}

		if (IsSizeFixed == true)
		{
			MDK_Error("Can not change size @ ObjectArrayData::FastResize(...)")
			return false;
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
			Length = InputLength;
		}
		catch (...)
		{
			MDK_Error("Out of Memory @ ObjectArrayData::FastResize(...)")

			ElementPointer = StdVector.data();
			Length = int_max(StdVector.size());
			return false;
		}
		return true;
	}
    //-------------------------------------------------------------
    void CopyDataToStdVectorIfNecessary()
    {
		if (ElementPointer != StdVector.data())
        {
			if (ElementPointer == nullptr || StdVector.data() != nullptr)
            {
                MDK_Error("ElementPointer is nullptr || StdVector.data() != nullptr @ ObjectArrayData::CopyDataToStdVectorIfNecessary()")
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
class ObjectArray : private SharedDataObject<ObjectArrayData<Element_Type>>
{
public:
	typedef Element_Type ElementType;
	
public:			
	//------------------- constructor and destructor ------------------------------------//

    inline ObjectArray();

    inline ObjectArray(const std::initializer_list<ElementType>& InputArray);

    inline ObjectArray(const std::vector<ElementType>& InputArray);

	inline ObjectArray(const StdObjectVector<ElementType>& InputArray);

    // copy constructor
    inline ObjectArray(const ObjectArray<ElementType>& InputArray);

    // move constructor
    inline ObjectArray(ObjectArray<ElementType>&& InputArray) noexcept;

	inline ~ObjectArray();

	//-------------------- StdVector -------------------------------------------//
	// std::vector<ElementType>& StdVector() {return m_Data->StdVector;}
	// const std::vector<ElementType>& StdVector() const {return m_Data->StdVector;}
	// Do NOT implement the two function:
	// if data is in external array, then StdVector() const will give wrong answer

	//----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const ObjectArray<ElementType>& InputArray);

    inline void operator=(ObjectArray<ElementType>&& InputArray);

	inline void operator=(const StdObjectVector<ElementType>& InputArray);

    inline void operator=(const std::initializer_list<ElementType>& InputList);

    inline void operator=(const std::vector<ElementType>& InputList);

    //----------------------  Copy  ----------------------------------------//
	inline bool Copy(const ObjectArray<ElementType>* InputArray);
	inline bool Copy(const ObjectArray<ElementType>& InputArray);

	inline bool Copy(ObjectArray<ElementType>&& InputArray);

	inline bool Copy(const std::vector<ElementType>& InputArray);

	inline bool Copy(const StdObjectVector<ElementType>& InputArray);

	inline bool Copy(const StdObjectVector<ElementType>* InputArray);

    inline bool Copy(const ElementType* InputElementPointer, int_max InputLength);

    inline bool Fill(const ElementType& Element);

    //-------------------------- Shared, ForceShare  ------------------------------------------ //

    inline bool Share(ObjectArray<ElementType>& InputArray);
    inline bool Share(ObjectArray<ElementType>* InputArray);

    inline void ForceShare(const ObjectArray<ElementType>& InputArray);
    inline bool ForceShare(const ObjectArray<ElementType>* InputArray);

    //-------------------------- special Share  ---------------------------------------------- //

    inline bool Share(ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed = true);

    inline bool ForceShare(const ElementType* InputElementPointer, int_max InputLength, bool IsSizeFixed = true);

    //------------------------- Swap shared_ptr m_Data -------------------------------------------//

    inline void Swap(ObjectArray<ElementType>& InputArray);

	//------------------------- Move shared_ptr m_Data -------------------------------------------//

	inline void Move(ObjectArray<ElementType>& InputArray);
	inline void Move(ObjectArray<ElementType>&& InputArray);

    //------------------------- Clear -------------------------------------------//

    inline void Clear();

	//---------------------- Set/get Size ----------------------------------------//

    inline bool Resize(int_max InputLength); // try to keep the old data

    inline bool FastResize(int_max InputLength); // do not care about old data

    inline bool ReserveCapacity(int_max InputElementNumber); // reserve memory, current Length does not change

    inline void ReleaseUnusedCapacity();

    inline void FixSize();

    inline bool IsSizeFixed() const;

    inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;

    inline bool IsShared() const;
	inline bool IsSharedWith(const ObjectArray& InputArray) const;

	inline bool IsDataInInternalArray() const;

    inline int_max GetLength() const;
    inline int_max GetElementNumber() const; // the same as GetLength();

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

    inline bool Append(ElementType Element);

	// and Append({1, 2, 3}} error if ElementType is DenseVector<int_max>
    //inline bool Append(const std::initializer_list<ElementType>& InputArray);
    //inline bool Append(const std::vector<ElementType>& InputArray);    
    //inline bool Append(const DenseMatrix<ElementType>& InputArray);
	//inline bool Append(const StdObjectVector<ElementType>& InputArray);
    //inline bool Append(const ObjectArray<ElementType>& InputArray);

    inline bool Append(const ElementType* InputArray, int_max InputLength);

    inline bool Delete(int_max Index);

    inline bool Delete(const std::initializer_list<int_max>& IndexList);

    inline bool Delete(const std::vector<int_max>& IndexList);

	inline bool Delete(const DenseVector<int_max>& IndexList);

    inline bool Delete(const DenseMatrix<int_max>& IndexList);

    inline bool Delete(const ObjectArray<int_max>& IndexList);

	inline bool Delete(const StdObjectVector<int_max>& IndexList);

    inline bool Delete(const int_max* ColIndexList, int_max ListLength);

    inline bool Delete(int_max Index_start, int_max Index_end);

    inline bool Insert(int_max Index, const ElementType& Element);

    inline bool Insert(int_max Index, const std::initializer_list<ElementType>& InputArray);

    inline bool Insert(int_max Index, const std::vector<ElementType>& InputArray);

	inline bool Insert(int_max Index, const StdObjectVector<ElementType>& InputArray);

	inline bool Insert(int_max Index, const ObjectArray<ElementType>& InputArray);
	
    inline bool Insert(int_max Index, const ElementType* InputArray, int_max InputLength);

    //------------- use ObjectArray as a stack ----------------------------//

	inline bool PushBack(ElementType Element);

    inline ElementType PopBack();

    //----------------------- Get a subset ------------------------------//

    inline ObjectArray<ElementType> GetSubSet(int_max Index_start, int_max Index_end);

    inline ObjectArray<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList);

    inline ObjectArray<ElementType> GetSubSet(const std::vector<int_max>& IndexList);

	inline ObjectArray<ElementType> GetSubSet(const StdObjectVector<int_max>& IndexList);

	inline ObjectArray<ElementType> GetSubSet(const ObjectArray<int_max>& IndexList);

	inline ObjectArray<ElementType> GetSubSet(const DenseVector<int_max>& IndexList);

    inline ObjectArray<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList);	

    inline ObjectArray<ElementType> GetSubSet(const int_max* IndexList, int_max ListLength);

	//----------------------- Set subset ------------------------------//

	inline bool SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubSetData);

	inline bool SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubSetData);

	inline bool SetSubSet(const std::initializer_list<int_max>& IndexList, const ObjectArray<ElementType>& SubSetData);

	inline bool SetSubSet(const std::vector<int_max>& IndexList, const ObjectArray<ElementType>& SubSetData);

	inline bool SetSubSet(const StdObjectVector<int_max>& IndexList, const ObjectArray<ElementType>& SubSetData);

	inline bool SetSubSet(const ObjectArray<int_max>& IndexList, const ObjectArray<ElementType>& SubSetData);

	inline bool SetSubSet(const DenseMatrix<int_max>& IndexList, const ObjectArray<ElementType>& SubSetData);

	inline bool SetSubSet(const DenseVector<int_max>& IndexList, const ObjectArray<ElementType>& SubSetData);

	inline bool SetSubSet(const int_max* IndexList, const ElementType* SubSetData, int_max DataNumber);

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

	inline ObjectArray<int_max> Sort(const char* Order);
	// Order: ascend or descend
	inline ObjectArray<int_max> Sort(const String& Order);

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

	inline void SortInPlace(const char* Order);
	// Order: ascend or descend
	inline void SortInPlace(const String& Order);


private:

};


}//end namespace mdk

#include "mdkObjectArray.hpp"

#endif
