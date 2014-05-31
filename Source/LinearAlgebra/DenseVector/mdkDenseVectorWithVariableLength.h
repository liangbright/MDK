#ifndef __mdkDenseVectorWithVariableLength_h
#define __mdkDenseVectorWithVariableLength_h

#include <utility>
#include <vector>

#include "mdkType.h"
#include "mdkObject.h"

// this is a memory efficient implementation of Dense Vector
//
// for example, use it as a voxel in a 3D vector image

//------------ forward declare ----------------------
//template<typename ElementType>
//class DenseMatrix;

//template<typename ElementType>
//class SimpleDataContainer;
//---------------------------------------------

namespace mdk
{
  
template<typename ElementType>
class DenseVector<ElementType, -1> : public Object
{
private:
    std::vector<ElementType> m_DataArray;

public:
    typedef ElementType ElementType;

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

    inline void Copy(const DenseVector<ElementType>& InputVector);

    //------------------------------------------------------------

    inline void Clear(); // set to empty (length = 0)

    inline void Squeeze();

    inline void Resize(int_max Length);

    inline void FastResize(int_max Length);

    inline void ReserveCapacity(int_max Length);

    inline void Fill(const ElementType& Element);

    inline int_max GetLength() const;

    inline int_max GetElementNumber() const;

    inline bool IsEmpty() const;

    inline bool IsLengthFixed() const;

    inline bool IsSizeFixed() const;

    //-----------element access------------------//

    inline ElementType* GetElementPointer();

    inline const ElementType* GetElementPointer() const;

    inline ElementType* begin();

    inline const ElementType* begin() const;

    inline ElementType* end();

    inline const ElementType* end() const;

    inline ElementType& operator[](int_max Index);

    inline const ElementType& operator[](int_max Index) const;

    ElementType& operator()(int_max Index);

    const ElementType& operator()(int_max Index) const;

    ElementType& at(int_max Index);

    const ElementType& at(int_max Index) const;

    //-------------------Get std vector -------------------------------

    std::vector<ElementType> StdVector();

    const std::vector<ElementType> StdVector() const;

    //---------- convert to regular vector represented by DenseMatrix ----------------//

    DenseMatrix<ElementType> CreateDenseMatrixAsRowVector() const;

    void CreateDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const;

    DenseMatrix<ElementType> CreateDenseMatrixAsColVector() const;

    void CreateDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const;

    //---------------------- GetSubSet --------------------------------------

    inline DenseVector<ElementType> GetSubSet(int_max Index_start, int_max Index_end);

    inline DenseVector<ElementType> GetSubSet(const std::initializer_list<int_max>& IndexList);

    inline DenseVector<ElementType> GetSubSet(const std::vector<int_max>& IndexList);

    inline DenseVector<ElementType> GetSubSet(const DenseMatrix<int_max>& IndexList);

    inline DenseVector<ElementType> GetSubSet(const SimpleDataContainer<int_max>& IndexList);

    inline DenseVector<ElementType> GetSubSet(const DenseVector<int_max>& IndexList);

    template<int_max InputLength>
    inline DenseVector<ElementType> GetSubSet(const DenseVector<int_max, InputLength>& IndexList);

    //---------------------- SetSubSet --------------------------------------

    inline bool SetSubSet(int_max Index_start, int_max Index_end, const std::initializer_list<ElementType>& SubVector);

    inline bool SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubVector);

    inline bool SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubVector);

    inline bool SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubVector);

    inline bool SetSubSet(const SimpleDataContainer<int_max>& IndexList, const SimpleDataContainer<ElementType>& SubVector);

    inline bool SetSubSet(const DenseVector<int_max>& IndexList, const DenseVector<int_max>& SubVector);

    template<int_max InputLength>
    inline bool SetSubSet(const DenseVector<int_max, InputLength>& IndexList, const DenseVector<int_max, InputLength>& SubVector);

    //-------------------------------------------------------------------------------

    inline bool Append(ElementType Element);

    inline bool Append(const ElementType* InputData, int_max InputLength);

    inline bool Delete(int_max Index);

    inline bool Delete(const std::initializer_list<int_max>& IndexList);

    inline bool Delete(const std::vector<int_max>& IndexList);

    inline bool Delete(const DenseMatrix<int_max>& IndexList);

    inline bool Delete(const SimpleDataContainer<int_max>& IndexList);

    inline bool Delete(const DenseVector<int_max>& IndexList);

    template<int_max InputLength>
    inline bool Delete(const DenseVector<int_max, InputLength>& IndexList);

    inline bool Delete(const int_max* ColIndexList, int_max ListLength);

    inline bool Delete(int_max Index_start, int_max Index_end);

    inline bool Insert(int_max Index, const ElementType& Element);

    inline bool Insert(int_max Index, const std::initializer_list<ElementType>& InputData);

    inline bool Insert(int_max Index, const std::vector<ElementType>& InputData);

    inline bool Insert(int_max Index, const DenseMatrix<ElementType>& InputData);

    inline bool Insert(int_max Index, const SimpleDataContainer<ElementType>& InputData);

    inline bool Insert(int_max Index, const DenseVector<ElementType>& InputData);

    template<int_max InputLength>
    inline bool Insert(int_max Index, const DenseVector<ElementType, InputLength>& InputData);

    inline bool Insert(int_max Index, const ElementType* InputData, int_max InputLength);

    //------------------- push pop the last element ----------------------//

    inline bool PushBack(ElementType Element);

    inline ElementType PopBack();

    //-------------------- find ---------------------------------------//

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction);

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction);

    //--------------------- sort ---------------------------------------//

    template<typename CompareFunctionType>
    inline DenseVector<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline DenseVector<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

    //------------------------- unique --------------------------------//
    // return unique element
    inline DenseVector<ElementType> Unique();

    inline DenseVector<int_max> IndexListOfUnique();

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

    inline ElementType Sum();

    inline ElementType Mean();

    inline ElementType Std();

    inline ElementType Max();

    inline ElementType Min();

    inline ElementType L1Norm();

    inline ElementType L2Norm();
};


}// namespace mdk

#include "mdkDenseVectorWithVariableLength.hpp"
#include "mdkDenseVectorWithVariableLength_Operator.h"
#include "mdkDenseVectorWithVariableLength_Function.h"

#endif