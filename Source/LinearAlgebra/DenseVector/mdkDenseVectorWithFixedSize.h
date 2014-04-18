#ifndef __mdkDenseVectorWithFixedSize_h
#define __mdkDenseVectorWithFixedSize_h

#include <utility>
#include <array>

#include "mdkDebugConfig.h"
#include "mdkType.h"
#include "mdkObject.h"
#include "mdkMatrix_Common.h"

#include "mdkDenseVectorWithFixedSize_Operator.h"
#include "mdkDenseVectorWithFixedSize_Function.h"

// this is a memory efficient implementation of Dense Vector
// Only use it if memory is not enough
// for example, use it as a voxel in a 3D image

namespace mdk
{
 
//-------------------------------
template<typename ElementType>
class DenseMatrix;
//------------------------------


template<typename ElementType, int_max Length>
class DenseVectorWithFixedSize : public Object
{
private:
    std::array<ElementType, Length> m_DataArray;

public:
    
    inline DenseVectorWithFixedSize();

    inline DenseVectorWithFixedSize(const ElementType& Element);

    inline DenseVectorWithFixedSize(const DenseVectorWithFixedSize<ElementType, Length>& InputVector) = delete;

    inline DenseVectorWithFixedSize(DenseVectorWithFixedSize<ElementType, Length>&& InputVector);

    inline ~DenseVectorWithFixedSize();

    inline void operator=(const DenseVectorWithFixedSize<ElementType, Length>& InputVector);

    inline void operator=(DenseVectorWithFixedSize<ElementType, Length>&& InputVector);

    inline void Clear();

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

    //------------ operator += -= *= /= Vector -------------------//

    //----------------------------------------------------  {+= -= *= /=}  ---------------------------------------------// 

    inline void operator+=(const DenseVectorWithFixedSize<ElementType, Length>& InputVector);

    inline void operator-=(const DenseVectorWithFixedSize<ElementType, Length>& InputVector);

    inline void operator*=(const DenseVectorWithFixedSize<ElementType, Length>& InputVector);

    inline void operator/=(const DenseVectorWithFixedSize<ElementType, Length>& InputVector);

    //----------------------------------------------------  {+= -= *= /=} Element ---------------------------------------------// 

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

};


}// namespace mdk

#include "mdkDenseVectorWithFixedSize.hpp"

#endif