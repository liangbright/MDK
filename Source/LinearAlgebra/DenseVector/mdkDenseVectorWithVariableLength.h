#ifndef __mdkDenseVectorWithVariableLength_h
#define __mdkDenseVectorWithVariableLength_h

#include <utility>
#include <vector>

#include "mdkDebugConfig.h"
#include "mdkType.h"
#include "mdkObject.h"
#include "mdkDenseVectorWithFixedLength.h"
#include "mdkLinearAlgebra_Function_DenseVector.h"

// this is a memory efficient implementation of Dense Vector
//
// for example, use it as a voxel in a 3D vector image

//------------ forward declare ----------------------
//template<typename ElementType>
//class DenseMatrix;

//template<typename ElementType>
//class SimpleDataArray;
//---------------------------------------------

namespace mdk
{
  
template<typename Element_Type>
class DenseVector<Element_Type, -1> : public Object
{
public:
	typedef Element_Type ElementType;

private:
    std::vector<ElementType> m_DataArray;

public:
    
    inline DenseVector();

    inline DenseVector(const ElementType& Element);

    inline DenseVector(const std::initializer_list<ElementType>& InputVector);

    inline DenseVector(const DenseVector<ElementType>& InputVector);

    template<int_max InputLength>
    inline DenseVector(const DenseVector<ElementType, InputLength>& InputVector);

    inline DenseVector(DenseVector<ElementType>&& InputVector);

    inline ~DenseVector();

    //------------------------------------------------------------

    inline void operator=(const DenseVector<ElementType>& InputVector);

    template<int_max Length>
    inline void operator=(const DenseVector<ElementType, Length>& InputVector);

    inline void operator=(DenseVector<ElementType>&& InputVector);

    inline void operator=(const std::initializer_list<const DenseVector<ElementType>*>& InputList);

    inline void operator=(const std::initializer_list<ElementType>& InputVector);

    inline void operator=(const std::vector<ElementType>& InputVector);

    inline void operator=(const DenseMatrix<ElementType>& InputVector);

    //-------------------------------------------//

    template<typename ElementType_input>
    inline void Copy(const DenseVector<ElementType_input>& InputVector);

    template<typename ElementType_input>
    inline void Copy(const ElementType_input* InputVector, int_max InputLength);

    //------------------------------------------------------------

    inline void Clear(); // set to empty (length = 0)

    inline void Resize(int_max Length);

    inline void FastResize(int_max Length);

    inline void ReserveCapacity(int_max Length);

    inline void ReleaseUnusedCapacity();

    inline void Fill(const ElementType& Element);

    inline int_max GetLength() const;

    inline int_max GetElementNumber() const;

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

    //----------------------- Convert to std vector -------------------------------

    std::vector<ElementType> CreateStdVector() const;

    //---------- convert to regular vector represented by DenseMatrix ----------------//

    DenseMatrix<ElementType> CreateDenseMatrixAsRowVector() const;

    void CreateDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const;

    DenseMatrix<ElementType> CreateDenseMatrixAsColVector() const;

    void CreateDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const;

    //---------------------- GetSubSet --------------------------------------

    inline DenseVector<ElementType> GetSubSet(int_max Index_start, int_max Index_end) const;

    inline DenseVector<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList) const;

    inline DenseVector<ElementType> GetSubSet(const std::vector<int_max>& IndexList) const;

    inline DenseVector<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList) const;

    inline DenseVector<ElementType> GetSubSet(const SimpleDataArray<int_max>& IndexList) const;

    inline DenseVector<ElementType> GetSubSet(const DenseVector<int_max>& IndexList) const;

    template<int_max InputLength>
    inline DenseVector<ElementType> GetSubSet(const DenseVector<int_max, InputLength>& IndexList) const;

    //---------------------- SetSubSet --------------------------------------

    inline bool SetSubSet(int_max Index_start, int_max Index_end, const std::initializer_list<ElementType>& SubVector);

    inline bool SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubVector);

    inline bool SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubVector);

    inline bool SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubVector);

    inline bool SetSubSet(const SimpleDataArray<int_max>& IndexList, const SimpleDataArray<ElementType>& SubVector);

    inline bool SetSubSet(const DenseVector<int_max>& IndexList, const DenseVector<int_max>& SubVector);

    template<int_max InputLength>
    inline bool SetSubSet(const DenseVector<int_max, InputLength>& IndexList, const DenseVector<int_max, InputLength>& SubVector);

    //-------------------------------------------------------------------------------

    inline bool Append(ElementType Element);

    inline bool Append(const DenseVector<ElementType>& InputData);

    inline bool Append(const ElementType* InputData, int_max InputLength);

    inline bool Delete(int_max Index);

    inline bool Delete(const std::initializer_list<int_max>& IndexList);

    inline bool Delete(const std::vector<int_max>& IndexList);

    inline bool Delete(const DenseMatrix<int_max>& IndexList);

    inline bool Delete(const SimpleDataArray<int_max>& IndexList);

    inline bool Delete(const DenseVector<int_max>& IndexList);

    template<int_max InputLength>
    inline bool Delete(const DenseVector<int_max, InputLength>& IndexList);

    inline bool Delete(const int_max* ColIndexList, int_max ListLength);

    inline bool Delete(int_max Index_start, int_max Index_end);

    inline bool Insert(int_max Index, const ElementType& Element);

    inline bool Insert(int_max Index, const std::initializer_list<ElementType>& InputData);

    inline bool Insert(int_max Index, const std::vector<ElementType>& InputData);

    inline bool Insert(int_max Index, const DenseMatrix<ElementType>& InputData);

    inline bool Insert(int_max Index, const SimpleDataArray<ElementType>& InputData);

    inline bool Insert(int_max Index, const DenseVector<ElementType>& InputData);

    template<int_max InputLength>
    inline bool Insert(int_max Index, const DenseVector<ElementType, InputLength>& InputData);

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

    //--------------------- sort ---------------------------------------//
    // return index list

    template<typename CompareFunctionType>
    inline DenseVector<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline DenseVector<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

    inline DenseVector<int_max> Sort(const char* Order);
    // Order: ascend or descend
    inline DenseVector<int_max> Sort(const std::string& Order);

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

    inline void SortInPlace(const char* Order);
    // Order: ascend or descend
    inline void SortInPlace(const std::string& Order);

    //------------------------- unique --------------------------------//
    // return index list of unique element
    inline DenseVector<int_max> FindUnique() const;

    //SpecialCompareFunction(a, b) return {-1, 0, 1} as {a < b, a = b, a > b}
    template<typename SpecialCompareFunctionType>
    inline DenseVector<int_max> FindUnique(SpecialCompareFunctionType SpecialCompareFunction) const;

    // -------------------------------------- Self {+= -= *= /=} Vector -----------------------------------------------------//

    inline void operator+=(const DenseVector<ElementType>& InputVector);

    template<int_max InputLength>
    inline void operator+=(const DenseVector<ElementType, InputLength>& InputVector);

    inline void operator-=(const DenseVector<ElementType>& InputVector);

    template<int_max InputLength>
    inline void operator-=(const DenseVector<ElementType, InputLength>& InputVector);

    inline void operator*=(const DenseVector<ElementType>& InputVector);

    template<int_max InputLength>
    inline void operator*=(const DenseVector<ElementType, InputLength>& InputVector);

    inline void operator/=(const DenseVector<ElementType>& InputVector);

    template<int_max InputLength>
    inline void operator/=(const DenseVector<ElementType, InputLength>& InputVector);

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