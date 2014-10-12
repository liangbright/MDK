#ifndef __mdkSimpleObjectArray_h
#define __mdkSimpleObjectArray_h

#include <vector>
#include <string>
#include <memory>

#include "mdkObject.h"
#include "mdkDenseVector_ForwardDeclare.h"

namespace mdk
{

// forward declare ------------------------------
template<typename ElementType>
class DenseMatrix;

template<typename ElementType>
class SimpleObjectArray;

template<typename ElementType>
class ObjectArray;

template<typename ElementType>
using SimpleDataArray = SimpleObjectArray<ElementType>;
//------------------------------------------------

#if defined MDK_DEBUG_MODE
#define MDK_DEBUG_SimpleObjectArray_Operator_CheckBound
#endif

//----------------------------------------------------------------------------------------------------------------------------//

template<typename Element_Type>
class SimpleObjectArray : public Object
{     
public:
	typedef Element_Type  ElementType;

private:
    std::vector<ElementType> m_StdVector;

public:			
	//------------------- constructor and destructor ------------------------------------//

    inline SimpleObjectArray();

    inline SimpleObjectArray(const std::initializer_list<ElementType>& InputArray);

    inline SimpleObjectArray(const std::vector<ElementType>& InputArray);

	inline SimpleObjectArray(const ObjectArray<ElementType>& InputArray);

    inline SimpleObjectArray(const SimpleObjectArray<ElementType>& InputArray);

    // move constructor
    inline SimpleObjectArray(SimpleObjectArray<ElementType>&& InputArray) noexcept;

	inline ~SimpleObjectArray();

    //-------------------- get/set std vector -----------------------------------//

    inline std::vector<ElementType>& StdVector();

    inline const std::vector<ElementType>& StdVector() const;

    //----------------------  operator=  ----------------------------------------//

    // copy assignment operator
    // do not use function template for this function
    // otherwise, compiler will create a new one
    inline void operator=(const SimpleObjectArray<ElementType>& InputArray);

    inline void operator=(SimpleObjectArray<ElementType>&& InputArray);

	inline void operator=(const ObjectArray<ElementType>& InputArray);

    inline void operator=(const std::initializer_list<ElementType>& InputList);

    inline void operator=(const std::vector<ElementType>& InputList);

    //----------------------  Copy  ----------------------------------------//

    inline bool Copy(const SimpleObjectArray<ElementType>& InputArray);

    inline bool Copy(const SimpleObjectArray<ElementType>* InputArray);

	inline bool Copy(const ObjectArray<ElementType>& InputArray);

	inline bool Copy(const ObjectArray<ElementType>* InputArray);

    inline bool Copy(const ElementType* InputElementPointer, int_max InputLength);

    inline bool Fill(const ElementType& Element);

    //------------------------- Clear -------------------------------------------//

    inline void Clear();

	//---------------------- Set/get Size ----------------------------------------//

    inline bool Resize(int_max InputLength); // try to keep the old data

    inline bool FastResize(int_max InputLength); // do not care about old data

    inline bool ReserveCapacity(int_max InputElementNumber); // reserve memory, current Length does not change

    inline void ReleaseUnusedCapacity();

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

    inline SimpleObjectArray<ElementType> GetSubSet(int_max Index_start, int_max Index_end);

    inline SimpleObjectArray<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList);

    inline SimpleObjectArray<ElementType> GetSubSet(const std::vector<int_max>& IndexList);

    inline SimpleObjectArray<ElementType> GetSubSet(const SimpleObjectArray<int_max>& IndexList);

	inline SimpleObjectArray<ElementType> GetSubSet(const ObjectArray<int_max>& IndexList);

	inline SimpleObjectArray<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList);

	inline SimpleObjectArray<ElementType> GetSubSet(const DenseVector<int_max>& IndexList);

    inline SimpleObjectArray<ElementType> GetSubSet(const int_max* IndexList, int_max ListLength);

    //----------------------- Set subset ------------------------------//

	inline bool SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubSet);

	inline bool SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubSet);

	inline bool SetSubSet(const std::initializer_list<int_max>& IndexList, const SimpleObjectArray<ElementType>& SubSet);

	inline bool SetSubSet(const std::vector<int_max>& IndexList, const SimpleObjectArray<ElementType>& SubSet);

	inline bool SetSubSet(const SimpleObjectArray<int_max>& IndexList, const SimpleObjectArray<ElementType>& SubSet);

	inline bool SetSubSet(const ObjectArray<int_max>& IndexList, const SimpleObjectArray<ElementType>& SubSet);

	inline bool SetSubSet(const DenseMatrix<int_max>& IndexList, const SimpleObjectArray<ElementType>& SubSet);

	inline bool SetSubSet(const DenseVector<int_max>& IndexList, const SimpleObjectArray<ElementType>& SubSet);

	inline bool SetSubSet(const int_max* IndexList, const ElementType* SubSet, int_max DataNumber);

    //-------------------------------------------------------------------------------

    inline bool Append(ElementType Element);

    inline bool Append(const ElementType* InputArray, int_max InputLength);

    inline bool Delete(int_max Index);

    inline bool Delete(const std::initializer_list<int_max>& IndexList);

    inline bool Delete(const std::vector<int_max>& IndexList);

    inline bool Delete(const DenseMatrix<int_max>& IndexList);

    inline bool Delete(const SimpleObjectArray<int_max>& IndexList);

    inline bool Delete(const int_max* ColIndexList, int_max ListLength);

    inline bool Delete(int_max Index_start, int_max Index_end);

    inline bool Insert(int_max Index, const ElementType& Element);

    inline bool Insert(int_max Index, const std::initializer_list<ElementType>& InputArray);

    inline bool Insert(int_max Index, const std::vector<ElementType>& InputArray);

    inline bool Insert(int_max Index, const DenseMatrix<ElementType>& InputArray);

    inline bool Insert(int_max Index, const SimpleObjectArray<ElementType>& InputArray);

    inline bool Insert(int_max Index, const ElementType* InputArray, int_max InputLength);

    //------------- use SimpleObjectArray as a stack ----------------------------//

    inline bool PushBack(ElementType Element);

    inline ElementType PopBack();

    //-------------------- find ---------------------------------------//

    template<typename MatchFunctionType>
    inline SimpleObjectArray<int_max> Find(MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline SimpleObjectArray<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline SimpleObjectArray<int_max> Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction);

	//-------------------- ExactMatch (use operator == ) ---------------------------------------//

	inline SimpleObjectArray<int_max> ExactMatch(const ElementType& InputElement) const;

	inline int_max ExactMatch(const std::string& first_or_last, const ElementType& InputElement) const;

    //--------------------- sort ---------------------------------------//

    template<typename CompareFunctionType>
    inline SimpleObjectArray<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline SimpleObjectArray<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

private:

};


}//end namespace mdk

#include "mdkSimpleObjectArray.hpp"

#endif
