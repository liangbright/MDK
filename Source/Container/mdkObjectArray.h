#ifndef __mdkObjectArray_h
#define __mdkObjectArray_h

#include <vector>
#include <string>
#include <memory>

#include "mdkCommonType.h"
#include "mdkConstant.h"
#include "mdkObject.h"
#include "mdkDenseVector_ForwardDeclare.h"

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
    std::vector<ElementType> StdVector;

    int_max Length;

    ElementType* ElementPointer;

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

    ~ObjectArrayData() {};

    void CopyDataToStdVectorIfNecessary()
    {
		if (ElementPointer != StdVector.data())
        {
            if (ElementPointer == nullptr)
            {
                MDK_Error("ElementPointer is nullptr @ ObjectArrayData::CopyDataToStdVectorIfNecessary()")
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

private:
//deleted: -------------------------------------------------
    ObjectArrayData(const ObjectArrayData&) = delete;

    ObjectArrayData(ObjectArrayData&&) = delete;

    void operator=(const ObjectArrayData&) = delete;

    void operator=(ObjectArrayData&&) = delete;
};

//----------------------------------------------------------------------------------------------------------------------------//

template<typename Element_Type>
class ObjectArray : public Object
{
public:
	typedef Element_Type ElementType;

private:     
    std::shared_ptr<ObjectArrayData<ElementType>> m_Data;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline ObjectArray();

    inline ObjectArray(const std::initializer_list<ElementType>& InputArray);

    inline ObjectArray(const std::vector<ElementType>& InputArray);

	inline ObjectArray(const StdObjectVector<ElementType>& InputArray);

    // deep-copy or shared-copy constructor
    inline ObjectArray(const ObjectArray<ElementType>& InputArray, ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy);

    // move constructor
    inline ObjectArray(ObjectArray<ElementType>&& InputArray) noexcept;

	inline ~ObjectArray();

    //-------------------- get/set std vector -----------------------------------//
	// caution: check: IsDataInInternalArray()
	// if Element is not stored inside internal std::vector
	// then data copy will be applied
    inline std::vector<ElementType>& StdVector();

    inline const std::vector<ElementType>& StdVector() const;

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

	inline bool Copy(const std::vector<ElementType>& InputArray);

	inline bool Copy(const StdObjectVector<ElementType>& InputArray);

	inline bool Copy(const StdObjectVector<ElementType>* InputArray);

    inline bool Copy(const ObjectArray<ElementType>& InputArray);

    inline bool Copy(const ObjectArray<ElementType>* InputArray);

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

    //-------------------- Take -----------------------------------------------------------//

    inline void Take(ObjectArray<ElementType>&& InputArray);

    inline bool Take(ObjectArray<ElementType>& InputArray);

    inline bool Take(ObjectArray<ElementType>* InputArray);

    //------------------------- Swap shared_ptr m_Data -------------------------------------------//

    inline void Swap(ObjectArray<ElementType>& InputArray);

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

    inline bool IsShared() const;

	inline bool IsDataInInternalArray() const;

    inline int_max GetLength() const;

    inline int_max GetElementNumber() const; // the same as GetLength();

    //------------------------ Error Element -----------------------------//

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

	inline int_max ExactMatch(const std::string& first_or_last, const ElementType& InputElement) const;

    //--------------------- sort ---------------------------------------//

    template<typename CompareFunctionType>
    inline ObjectArray<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline ObjectArray<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

	inline ObjectArray<int_max> Sort(const char* Order);
	// Order: ascend or descend
	inline ObjectArray<int_max> Sort(const std::string& Order);

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

#endif
