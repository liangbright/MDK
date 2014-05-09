#ifndef __mdkDenseVectorWithVariableSize_Operator_h
#define __mdkDenseVectorWithVariableSize_Operator_h

#include <vector>

#include "mdkType.h"

namespace mdk
{

template<typename ElementType>
inline DenseVectorWithVariableSize<ElementType> operator+(const DenseVectorWithVariableSize<ElementType>& VectorA,
                                                          const DenseVectorWithVariableSize<ElementType>& VectorB);

template<typename ElementType>
inline DenseVectorWithVariableSize<ElementType> operator-(const DenseVectorWithVariableSize<ElementType>& VectorA,
                                                          const DenseVectorWithVariableSize<ElementType>& VectorB);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline DenseVectorWithVariableSize<ElementType> operator+(const DenseVectorWithVariableSize<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVectorWithVariableSize<ElementType> operator-(const DenseVectorWithVariableSize<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVectorWithVariableSize<ElementType> operator*(const DenseVectorWithVariableSize<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVectorWithVariableSize<ElementType> operator/(const DenseVectorWithVariableSize<ElementType>& Vector, const ElementType& Element);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline DenseVectorWithVariableSize<ElementType> operator+(const ElementType& Element, const DenseVectorWithVariableSize<ElementType>& Vector);

template<typename ElementType>
inline DenseVectorWithVariableSize<ElementType> operator-(const ElementType& Element, const DenseVectorWithVariableSize<ElementType>& Vector);

template<typename ElementType>
inline DenseVectorWithVariableSize<ElementType> operator*(const ElementType& Element, const DenseVectorWithVariableSize<ElementType>& Vector);

template<typename ElementType>
inline DenseVectorWithVariableSize<ElementType> operator/(const ElementType& Element, const DenseVectorWithVariableSize<ElementType>& Vector);

//---------------------------------------------------------------------------------------------------------------//

}

#include "mdkDenseVectorWithVariableSize_Operator.hpp"

#endif