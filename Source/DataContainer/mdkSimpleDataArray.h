#ifndef __mdkSimpleDataArray_h
#define __mdkSimpleDataArray_h

#include <vector>
#include <string>
#include <memory>

#include "mdkObject.h"

namespace mdk
{

//------------------------------
template<typename ElementType>
class DenseMatrix;
//------------------------------

#if defined MDK_DEBUG_MODE
#define MDK_DEBUG_SimpleDataArray_Operator_CheckBound
#endif

//----------------------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
class SimpleDataArray : public Object
{     
    std::vector<ElementType> m_DataArray;

public:
    typedef ElementType  ElementType;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline SimpleDataArray();

    inline SimpleDataArray(const std::initializer_list<ElementType>& InputData);

    inline SimpleDataArray(const std::vector<ElementType>& InputData);

    // deep-copy or shared-copy constructor
    inline SimpleDataArray(const SimpleDataArray<ElementType>& InputData);

    // move constructor
    inline SimpleDataArray(SimpleDataArray<ElementType>&& InputData) noexcept;

	inline ~SimpleDataArray();

    //-------------------- get/set std vector -----------------------------------//

    inline std::vector<ElementType>& StdVector();

    inline const std::vector<ElementType>& StdVector() const;

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const SimpleDataArray<ElementType>& InputData);

    inline void operator=(SimpleDataArray<ElementType>&& InputData);

    inline void operator=(const std::initializer_list<ElementType>& InputList);

    inline void operator=(const std::vector<ElementType>& InputList);

    inline void operator=(const DenseMatrix<ElementType>& InputList);

    //----------------------  Copy  ----------------------------------------//

    inline bool Copy(const SimpleDataArray<ElementType>& InputData);

    inline bool Copy(const SimpleDataArray<ElementType>* InputData);

    inline bool Copy(const ElementType* InputElementPointer, int_max InputLength);

    inline bool Fill(const ElementType& Element);

    //------------------------- Clear -------------------------------------------//

    inline void Clear();

	//---------------------- Set/get Size ----------------------------------------//

    inline bool Resize(int_max InputLength); // try to keep the old data

    inline bool FastResize(int_max InputLength); // do not care about old data

    inline bool ReserveCapacity(int_max InputElementNumber); // reserve memory, current Length does not change

    inline void ReleaseUnusedCapacity();

    inline void Squeeze(); // same as ReleaseUnusedCapacity()

    inline bool IsEmpty() const;

    inline int_max GetLength() const;

    inline int_max GetElementNumber() const; // the same as GetLength();

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

    // operator[] or () : no bound check

    inline ElementType& operator[](int_max Index);
    inline const ElementType& operator[](int_max Index) const;

    inline ElementType& operator()(int_max Index);
    inline const ElementType& operator()(int_max Index) const;
    
    // at(): bound check

    inline ElementType& at(int_max Index);
    inline const ElementType& at(int_max Index) const;

    //----------------------- Get subset ------------------------------//

    inline SimpleDataArray<ElementType> GetSubSet(int_max Index_start, int_max Index_end);

    inline SimpleDataArray<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList);

    inline SimpleDataArray<ElementType> GetSubSet(const std::vector<int_max>& IndexList);

    inline SimpleDataArray<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList);

    inline SimpleDataArray<ElementType> GetSubSet(const SimpleDataArray<int_max>& IndexList);

    inline SimpleDataArray<ElementType> GetSubSet(const int_max* IndexList, int_max ListLength);

    //----------------------- Set subset ------------------------------//

    inline bool SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubSetData);

    inline bool SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubSetData);

    inline bool SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubSetData);

    inline bool SetSubSet(const SimpleDataArray<int_max>& IndexList, const SimpleDataArray<ElementType>& SubSetData);

    inline bool SetSubSet(const int_max* IndexList, const ElementType* SubSetData, int_max DataNumber);

    //-------------------------------------------------------------------------------

    inline bool Append(ElementType Element);

    inline bool Append(const ElementType* InputData, int_max InputLength);

    inline bool Delete(int_max Index);

    inline bool Delete(const std::initializer_list<int_max>& IndexList);

    inline bool Delete(const std::vector<int_max>& IndexList);

    inline bool Delete(const DenseMatrix<int_max>& IndexList);

    inline bool Delete(const SimpleDataArray<int_max>& IndexList);

    inline bool Delete(const int_max* ColIndexList, int_max ListLength);

    inline bool Delete(int_max Index_start, int_max Index_end);

    inline bool Insert(int_max Index, const ElementType& Element);

    inline bool Insert(int_max Index, const std::initializer_list<ElementType>& InputData);

    inline bool Insert(int_max Index, const std::vector<ElementType>& InputData);

    inline bool Insert(int_max Index, const DenseMatrix<ElementType>& InputData);

    inline bool Insert(int_max Index, const SimpleDataArray<ElementType>& InputData);

    inline bool Insert(int_max Index, const ElementType* InputData, int_max InputLength);

    //------------- use SimpleDataArray as a stack ----------------------------//

    inline bool PushBack(ElementType Element);

    inline ElementType PopBack();

    //-------------------- find ---------------------------------------//

    template<typename MatchFunctionType>
    inline SimpleDataArray<int_max> Find(MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline SimpleDataArray<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline SimpleDataArray<int_max> Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction);

    //--------------------- sort ---------------------------------------//

    template<typename CompareFunctionType>
    inline SimpleDataArray<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline SimpleDataArray<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

private:

};


}//end namespace mdk

#include "mdkSimpleDataArray.hpp"

#endif
