#ifndef __mdkDenseVectorWithFixedLength_h
#define __mdkDenseVectorWithFixedLength_h

#include <utility>
#include <array>

#include "mdkDebugConfig.h"
#include "mdkType.h"
#include "mdkObject.h"
#include "mdkLinearAlgebra_Function_DenseVector.h"

// this is a memory efficient implementation of Dense Vector
// Only use it if memory is not enough
// for example, use it as a voxel in a 3D vector image

namespace mdk
{
 
//-------------------------------
//template<typename ElementType>
//class DenseMatrix;

//template<typename ElementType>
//class SimpleDataContainer;

//------------------------------


template<typename ElementType, int_max Length = -1>
class DenseVector : public Object
{
private:
    std::array<ElementType, Length> m_DataArray;

public:
    typedef ElementType ElementType;

public:
    
    inline DenseVector();
    
    inline DenseVector(const ElementType& Element);

    inline DenseVector(const std::initializer_list<ElementType>& InputVector);

    inline DenseVector(const DenseVector<ElementType, Length>& InputVector);

    inline DenseVector(DenseVector<ElementType, Length>&& InputVector);
    
    inline ~DenseVector();

    //------------------------------------------------------------

    inline void operator=(const DenseVector<ElementType, Length>& InputVector);

    inline void operator=(DenseVector<ElementType, Length>&& InputVector);

    inline void operator=(const std::initializer_list<ElementType>& InputVector);

    inline void operator=(const std::vector<ElementType>& InputVector);

    inline void operator=(const DenseVector<ElementType>& InputVector);

    inline void operator=(const DenseMatrix<ElementType>& InputVector);

    //------------------------------------------------------------

    template<typename ElementType_input>
    inline void Copy(const DenseVector<ElementType_input, Length>& InputVector);

    template<typename ElementType_input>
    inline void Copy(const ElementType_input* InputVector);

    //------------------------------------------------------------

    inline void Clear(); // set every element to 0

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

    //-------------------Get std array -------------------------------

    std::array<ElementType, Length> CreateStdArray() const;

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

    inline DenseVector<ElementType> GetSubSet(const SimpleDataContainer<int_max>& IndexList) const;

    inline DenseVector<ElementType> GetSubSet(const DenseVector<int_max>& IndexList) const;

    template<int_max InputLength>
    inline DenseVector<ElementType> GetSubSet(const DenseVector<int_max, InputLength>& IndexList) const;

    //---------------------- SetSubSet --------------------------------------

    inline bool SetSubSet(int_max Index_start, int_max Index_end, const std::initializer_list<ElementType>& SubVector);

    inline bool SetSubSet(const std::initializer_list<int_max>& IndexList, const std::initializer_list<ElementType>& SubVector);

    inline bool SetSubSet(const std::vector<int_max>& IndexList, const std::vector<ElementType>& SubVector);

    inline bool SetSubSet(const DenseMatrix<int_max>& IndexList, const DenseMatrix<ElementType>& SubVector);

    inline bool SetSubSet(const SimpleDataContainer<int_max>& IndexList, const SimpleDataContainer<ElementType>& SubVector);

    inline bool SetSubSet(const DenseVector<int_max>& IndexList, const DenseVector<int_max>& SubVector);

    template<int_max InputLength>
    inline bool SetSubSet(const DenseVector<int_max, InputLength>& IndexList, const DenseVector<int_max, InputLength>& SubVector);

    //-------------------- find ---------------------------------------//
    // return index list

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(MatchFunctionType MatchFunction) const;

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction) const;

    template<typename MatchFunctionType>
    inline DenseVector<int_max> Find(int_max MaxOutputNumber, int_max Index_start, int_max Index_end, MatchFunctionType MatchFunction) const;

    //--------------------- sort ---------------------------------------//
    // return index list

    template<typename CompareFunctionType>
    inline DenseVector<int_max> Sort(CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline DenseVector<int_max> Sort(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction) const;

    template<typename CompareFunctionType>
    inline void SortInPlace(CompareFunctionType CompareFunction);

    template<typename CompareFunctionType>
    inline void SortInPlace(int_max Index_start, int_max Index_end, CompareFunctionType CompareFunction);

    //------------------------- unique --------------------------------//
    // return index list of unique element
    inline DenseVector<int_max> FindUnique() const;

    //SpecialCompareFunction(a, b) return {-1, 0, 1} as {a < b, a = b, a > b}
    template<typename SpecialCompareFunctionType>
    inline DenseVector<int_max> FindUnique(SpecialCompareFunctionType SpecialCompareFunction) const;

    //---------------------------------------------------- Self {+= -= *= /=} Vector ---------------------------------------------// 

    inline void operator+=(const DenseVector<ElementType, Length>& InputVector);
    inline void operator+=(const DenseVector<ElementType>& InputVector);

    inline void operator-=(const DenseVector<ElementType, Length>& InputVector);
    inline void operator-=(const DenseVector<ElementType>& InputVector);

    inline void operator*=(const DenseVector<ElementType, Length>& InputVector);
    inline void operator*=(const DenseVector<ElementType>& InputVector);

    inline void operator/=(const DenseVector<ElementType, Length>& InputVector);
    inline void operator/=(const DenseVector<ElementType>& InputVector);

    //---------------------------------------------------- Self {+= -= *= /=} Element ---------------------------------------------// 

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //------------------------------------------------------------------------------------------------------------------------------//

    inline ElementType Sum() const;

    inline ElementType Mean() const;

    inline ElementType Std() const;

    inline ElementType Max() const;

    inline ElementType Min() const;

    inline ElementType L1Norm() const;

    inline ElementType L2Norm() const;
    
};


}// namespace mdk

#include "mdkDenseVectorWithFixedLength.hpp"
#include "mdkDenseVectorWithFixedLength_Operator.h"
#include "mdkDenseVectorWithFixedLength_Function.h"

#endif