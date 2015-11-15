#ifndef mdk_DenseVectorWithVariableLength_h
#define mdk_DenseVectorWithVariableLength_h

#include <utility>
#include <vector>
#include <limits>

#include "mdkDenseVectorWithFixedLength.h"
#include "mdkLinearAlgebra_Function_DenseVector.h"

// this is a memory efficient implementation of Dense Vector
//
// for example, use it as a voxel in a 3D vector image

//------------ forward declare ----------------------
// "ambiguous symbol"
//template<typename ElementType>
//class DenseMatrix;
//---------------------------------------------

namespace mdk
{
  
template<typename Element_Type>
class DenseVector<Element_Type, -1> : public Object
{
public:
	typedef Element_Type ElementType;

private:
    std::vector<ElementType> m_StdVector;

public:
    
    inline DenseVector();

    inline DenseVector(const ElementType& Element);

    inline DenseVector(const std::initializer_list<ElementType>& InputVector);

	inline DenseVector(std::vector<ElementType> InputVector);
	
	inline DenseVector(StdObjectVector<ElementType> InputVector);

	inline DenseVector(const ObjectArray<ElementType>& InputVector);

	inline DenseVector(const DenseMatrix<ElementType>& InputMatrix);

	template<int_max LengthParameter>
	inline DenseVector(const DenseVector<ElementType, LengthParameter>& InputVector);

    inline DenseVector(DenseVector<ElementType>&& InputVector);

	inline DenseVector(const std::initializer_list<const DenseVector<ElementType>*>& InputList);

    inline ~DenseVector();

    //------------------------------------------------------------

	inline void operator=(const ElementType& Element);

    inline void operator=(const DenseVector<ElementType>& InputVector);

	template<int_max LengthParameter>
	inline void operator=(const DenseVector<ElementType, LengthParameter>& InputVector);

    inline void operator=(DenseVector<ElementType>&& InputVector);

	// this cause compiler internal error: VS2013 C1001
	//template<int_max LengthParameter>
	//inline void operator=(const std::initializer_list<const DenseVector<ElementType, LengthParameter>*>& InputList);

	inline void operator=(const std::initializer_list<const DenseVector<ElementType>*>& InputList);

    inline void operator=(const std::initializer_list<ElementType>& InputVector);

	inline void operator=(std::vector<ElementType> InputVector);

	inline void operator=(StdObjectVector<ElementType> InputVector);

	inline void operator=(const ObjectArray<ElementType>& InputVector);

    inline void operator=(const DenseMatrix<ElementType>& InputVector);

    //-------------------------------------------//

    template<typename ElementType_input>
    inline void Copy(const DenseVector<ElementType_input>& InputVector);

    template<typename ElementType_input>
    inline void Copy(const ElementType_input* InputVector, int_max InputLength);

    //------------------------------------------------------------

    inline void Clear(); // set to empty (length = 0)

	inline bool Resize(int_max Length);

	inline bool FastResize(int_max Length);

	inline bool SetCapacity(int_max Length);

    inline void ReleaseUnusedCapacity();

    inline void Fill(const ElementType& Element);

    inline int_max GetLength() const;

    inline int_max GetElementCount() const;

    inline bool IsEmpty() const;

    inline bool IsLengthFixed() const;

    inline bool IsSizeFixed() const;

    //-----------element access------------------//

    inline ElementType* GetElementPointer();
    inline const ElementType* GetElementPointer() const;

    inline ElementType* GetPointer();
    inline const ElementType* GetPointer() const;

    inline ElementType* begin();
    inline const ElementType* begin() const;

    inline ElementType* end();
    inline const ElementType* end() const;

    inline ElementType& operator[](int_max Index);
    inline const ElementType& operator[](int_max Index) const;

    inline ElementType& operator()(int_max Index);
    inline const ElementType& operator()(int_max Index) const;

    inline ElementType& at(int_max Index);
    inline const ElementType& at(int_max Index) const;

    //----------------------- get/set std vector -------------------------------

	std::vector<ElementType>& StdVector();

	const std::vector<ElementType>& StdVector() const;

    //----------------------- Create a new std vector -------------------------------

    std::vector<ElementType> CreateStdVector() const;

    //---------- convert to regular vector represented by DenseMatrix ----------------//

    DenseMatrix<ElementType> ConvertToDenseMatrixAsRowVector() const;

    void ConvertToDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const;

    DenseMatrix<ElementType> ConvertToDenseMatrixAsColVector() const;

    void ConvertToDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const;

    //---------------------- GetSubSet --------------------------------------

    inline DenseVector<ElementType> GetSubSet(int_max Index_start, int_max Index_end) const;

    inline DenseVector<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList) const;

    inline DenseVector<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList) const;

	template<int_max LengthParameter>
	inline DenseVector<ElementType> GetSubSet(const DenseVector<int_max, LengthParameter>& IndexList) const;

    //---------------------- SetSubSet --------------------------------------

	// SetSubSet(Index_start, Index_end, SubVector),  Index_start >= or <= Index_end

    inline bool SetSubSet(int_max Index_start, int_max Index_end, const std::initializer_list<ElementType>& SubVector);

	inline bool SetSubSet(int_max Index_start, int_max Index_end, const DenseMatrix<ElementType>& SubVector);

	template<int_max LengthParameter>
	inline bool SetSubSet(int_max Index_start, int_max Index_end, const DenseVector<ElementType, LengthParameter>& SubVector);

	inline bool SetSubSet(int_max Index_start, int_max Index_end, const ElementType* SubVector);

	// SetSubSet(IndexList, SubVector)

    inline bool SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubVector);

	inline bool SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubVector);

	template<int_max LengthParameterA, int_max LengthParameterB>
	inline bool SetSubSet(const DenseVector<int_max, LengthParameterA>& IndexList, const DenseVector<ElementType, LengthParameterB>& SubVector);

	// SetSubSet(span(a, b), SubVector)
	
	inline bool SetSubSet(const DenseVector<int_max>& IndexList, const DenseMatrix<ElementType>& SubVector);

	template<int_max LengthParameter>
	inline bool SetSubSet(const DenseVector<int_max>& IndexList, const DenseVector<ElementType, LengthParameter>& SubVector);

	// base function
	inline bool SetSubSet(const int_max* IndexList, const ElementType* SubVector, int_max SubVectorLength);

    //-------------------------------------------------------------------------------

    inline bool Append(ElementType Element);

	template<int_max LengthParameter>
	inline bool Append(const DenseVector<ElementType, LengthParameter>& InputData);

    inline bool Append(const ElementType* InputData, int_max InputLength);

    inline bool Delete(int_max Index);

    inline bool Delete(const std::initializer_list<int_max>& IndexList);

    inline bool Delete(const DenseMatrix<int_max>& IndexList);

	template<int_max LengthParameter>
	inline bool Delete(const DenseVector<int_max, LengthParameter>& IndexList);

    inline bool Delete(const int_max* ColIndexList, int_max ListLength);

    inline bool Delete(int_max Index_start, int_max Index_end);

    inline bool Insert(int_max Index, const ElementType& Element);

    inline bool Insert(int_max Index, const std::initializer_list<ElementType>& InputData);

    inline bool Insert(int_max Index, const DenseMatrix<ElementType>& InputData);

	template<int_max LengthParameter>
	inline bool Insert(int_max Index, const DenseVector<ElementType, LengthParameter>& InputData);

    inline bool Insert(int_max Index, const ElementType* InputData, int_max InputLength);

    //------------------- push/ pop the last element ----------------------//

    inline bool PushBack(ElementType Element);

    inline ElementType PopBack();

    //-------------------- find ---------------------------------------//
    // return index list

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(MatchFunctionType MatchFunction) const;

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction) const;

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction) const;

	// find the first/last that match the condition
	template<typename MatchFunctionType>
	inline int_max Find(const std::string& first_or_last, MatchFunctionType MatchFunction) const;

	//-------------------- ExactMatch (use operator == ) ---------------------------------------//

	inline DenseVector<int_max> ExactMatch(const ElementType& InputElement) const;

	inline int_max ExactMatch(const std::string& first_or_last, const ElementType& InputElement) const;

    //--------------------- sort ---------------------------------------//
    // return index list

    template<typename CompareFunctionType>
    inline DenseVector<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline DenseVector<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

	// Order: ascend or descend
	inline DenseVector<int_max> Sort(const char* ascend_or_descend);
	inline DenseVector<int_max> Sort(const std::string& ascend_or_descend);

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

	// Order: ascend or descend
	inline void SortInPlace(const char* ascend_or_descend);
	inline void SortInPlace(const std::string& ascend_or_descend);

    //------------------------- unique --------------------------------//
    // return index list of unique element
    inline DenseVector<int_max> FindUnique() const;

    //SpecialCompareFunction(a, b) return {-1, 0, 1} as {a < b, a = b, a > b}
    template<typename SpecialCompareFunctionType>
    inline DenseVector<int_max> FindUnique(SpecialCompareFunctionType SpecialCompareFunction) const;

    // -------------------------------------- Self {+= -= *= /=} Vector -----------------------------------------------------//

	template<int_max LengthParameter>
	inline void operator+=(const DenseVector<ElementType, LengthParameter>& InputVector);

	template<int_max LengthParameter>
	inline void operator-=(const DenseVector<ElementType, LengthParameter>& InputVector);

	// vector * vector is ambiguous
	//template<int_max LengthParameter>
	//inline void operator*=(const DenseVector<ElementType, LengthParameter>& InputVector);

	template<int_max LengthParameter>
	inline void operator/=(const DenseVector<ElementType, LengthParameter>& InputVector);

    //---------------------------------------- Self {+= -= *= /=} Element ---------------------------------------------------//

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //-------------------------------------------------------------------------------------------------------------------------//

    inline ElementType Sum() const;

    inline ElementType Mean() const;

    inline ElementType Std() const;

    inline ElementType Max() const;

    inline int_max IndexOfMax() const;

    inline ElementType Min() const;

    inline int_max IndexOfMin() const;

    inline ElementType L1Norm() const;

    inline ElementType L2Norm() const;
};


}// namespace mdk

#include "mdkDenseVectorWithVariableLength.hpp"
#include "mdkDenseVectorWithVariableLength_Operator.h"

#endif