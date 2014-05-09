#ifndef __mdkDenseVectorWithFixedLength_h
#define __mdkDenseVectorWithFixedLength_h

#include <utility>
#include <array>

#include "mdkDebugConfig.h"
#include "mdkType.h"
#include "mdkObject.h"
#include "mdkMatrix_Common.h"

#include "mdkDenseVectorWithFixedLength_Operator.h"
#include "mdkDenseVectorWithFixedLength_Function.h"

// this is a memory efficient implementation of Dense Vector
// Only use it if memory is not enough
// for example, use it as a voxel in a 3D vector image

namespace mdk
{
 
//-------------------------------
template<typename ElementType>
class DenseMatrix;
//------------------------------


template<typename ElementType, int_max Length>
class DenseVectorWithFixedLength : public Object
{
private:
    std::array<ElementType, Length> m_DataArray;

public:
    typedef ElementType ElementType;

public:
    
    inline DenseVectorWithFixedLength();

    inline DenseVectorWithFixedLength(const ElementType& Element);

    inline DenseVectorWithFixedLength(const DenseVectorWithFixedLength<ElementType, Length>& InputVector) = delete;

    inline DenseVectorWithFixedLength(DenseVectorWithFixedLength<ElementType, Length>&& InputVector);

    inline ~DenseVectorWithFixedLength();

    inline void operator=(const DenseVectorWithFixedLength<ElementType, Length>& InputVector);

    inline void operator=(DenseVectorWithFixedLength<ElementType, Length>&& InputVector);

    inline void Clear(); // set every element to 0

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

    //---------------------------------------------------- Self {+= -= *= /=} Vector ---------------------------------------------// 

    inline void operator+=(const DenseVectorWithFixedLength<ElementType, Length>& InputVector);

    inline void operator-=(const DenseVectorWithFixedLength<ElementType, Length>& InputVector);

    inline void operator*=(const DenseVectorWithFixedLength<ElementType, Length>& InputVector);

    inline void operator/=(const DenseVectorWithFixedLength<ElementType, Length>& InputVector);

    //---------------------------------------------------- Self {+= -= *= /=} Element ---------------------------------------------// 

    inline void operator+=(const ElementType& Element);

    inline void operator-=(const ElementType& Element);

    inline void operator*=(const ElementType& Element);

    inline void operator/=(const ElementType& Element);

    //------------------------------------------------------------------------------------------------------------------------------//

    ElementType Sum();

    ElementType Mean();

    ElementType Std();

    ElementType Max();

    ElementType Min();

    ElementType L1Norm();

    ElementType L2Norm();

};


}// namespace mdk

#include "mdkDenseVectorWithFixedLength.hpp"

#endif