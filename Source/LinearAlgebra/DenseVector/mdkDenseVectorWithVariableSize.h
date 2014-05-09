#ifndef __mdkDenseVectorWithVariableSize_h
#define __mdkDenseVectorWithVariableSize_h

#include <utility>
#include <vector>

#include "mdkType.h"
#include "mdkObject.h"


#include "mdkDenseVectorWithVariableSize_Operator.h"
#include "mdkDenseVectorWithVariableSize_Function.h"

// this is a memory efficient implementation of Dense Vector
// Only use it if memory is not enough
// for example, use it as a voxel in a 3D image



namespace mdk
{
  
template<typename ElementType>
class DenseVectorWithVariableSize : public Object
{
private:
    std::vector<ElementType> m_DataArray;

public:
    typedef ElementType ElementType;

public:
    
    inline DenseVectorWithVariableSize();

    inline DenseVectorWithVariableSize(const ElementType& Element);

    inline DenseVectorWithVariableSize(const DenseVectorWithVariableSize<ElementType>& InputVector);

    inline DenseVectorWithVariableSize(DenseVectorWithVariableSize<ElementType, Length>&& InputVector);

    inline ~DenseVectorWithVariableSize();

    inline void operator=(const DenseVectorWithVariableSize<ElementType, Length>& InputVector);

    inline void operator=(DenseVectorWithVariableSize<ElementType, Length>&& InputVector);

    inline void Clear(); // set to empty (length = 0)

    inline void Resize(int_max Length);

    inline void Fill(const ElementType& Element);

    inline int_max GetLength();

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

    // ------------------------------------------------------------------------------------------------------------//

    inline void operator+=(const DenseVectorWithVariableSize<ElementType>& Voxel);

    inline void operator-=(const DenseVectorWithVariableSize<ElementType>& Voxel);

    inline void operator*=(const DenseVectorWithVariableSize<ElementType>& Voxel);

    inline void operator/=(const DenseVectorWithVariableSize<ElementType>& Voxel);

    //---------------------------------------------------------------------------------------------------------------//

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

};


}// namespace mdk

#include "mdkDenseVectorWithVariableSize.hpp"

#endif