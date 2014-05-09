#ifndef __mdkDenseVectorWithVariableLength_h
#define __mdkDenseVectorWithVariableLength_h

#include <utility>
#include <vector>

#include "mdkType.h"
#include "mdkObject.h"


#include "mdkDenseVectorWithVariableLength_Operator.h"
#include "mdkDenseVectorWithVariableLength_Function.h"

// this is a memory efficient implementation of Dense Vector
// Only use it if memory is not enough
// for example, use it as a voxel in a 3D vector image


namespace mdk
{
  
template<typename ElementType>
class DenseVectorWithVariableLength : public Object
{
private:
    std::vector<ElementType> m_DataArray;

public:
    typedef ElementType ElementType;

public:
    
    inline DenseVectorWithVariableLength();

    inline DenseVectorWithVariableLength(const ElementType& Element);

    inline DenseVectorWithVariableLength(const DenseVectorWithVariableLength<ElementType>& InputVector);

    inline DenseVectorWithVariableLength(DenseVectorWithVariableLength<ElementType, Length>&& InputVector);

    inline ~DenseVectorWithVariableLength();

    inline void operator=(const DenseVectorWithVariableLength<ElementType, Length>& InputVector);

    inline void operator=(DenseVectorWithVariableLength<ElementType, Length>&& InputVector);

    inline void Clear(); // set to empty (length = 0)

    inline void Resize(int_max Length);

    inline void Fill(const ElementType& Element);

    inline int_max GetLength() const;

    inline bool IsLengthFixed() const;

    inline bool IsLengthVariable() const;

    //-----------element access------------------//

    inline ElementType* GetElementPointer();

    inline const ElementType* GetElementPointer() const;

    inline ElementType& operator[](int_max Index);

    inline const ElementType& operator[](int_max Index) const;

    ElementType& operator()(int_max Index);

    const ElementType& operator()(int_max Index) const;

    ElementType& at(int_max Index);

    const ElementType& at(int_max Index) const;

    //---------- convert to regular vector represented by DenseMatrix ----------------//

    DenseMatrix<ElementType> CreateDenseMatrixAsRowVector() const;

    void CreateDenseMatrixAsRowVector(DenseMatrix<ElementType>& OutputVector) const;

    DenseMatrix<ElementType> CreateDenseMatrixAsColVector() const;

    void CreateDenseMatrixAsColVector(DenseMatrix<ElementType>& OutputVector) const;

    // -------------------------------------- Self {+= -= *= /=} Vector -----------------------------------------------------//

    inline void operator+=(const DenseVectorWithVariableLength<ElementType>& InputVector);

    inline void operator-=(const DenseVectorWithVariableLength<ElementType>& InputVector);

    inline void operator*=(const DenseVectorWithVariableLength<ElementType>& InputVector);

    inline void operator/=(const DenseVectorWithVariableLength<ElementType>& InputVector);

    //---------------------------------------- Self {+= -= *= /=} Element ---------------------------------------------------//

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //-------------------------------------------------------------------------------------------------------------------------//

    ElementType Sum();

    ElementType Mean();

    ElementType Std();

    ElementType Max();

    ElementType Min();

    ElementType L1Norm();

    ElementType L2Norm();
};


}// namespace mdk

#include "mdkDenseVectorWithVariableLength.hpp"

#endif