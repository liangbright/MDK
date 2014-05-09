#ifndef __mdkDenseVectorWithVariableLength_Operator_h
#define __mdkDenseVectorWithVariableLength_Operator_h

#include <vector>

#include "mdkType.h"

namespace mdk
{

template<typename ElementType>
inline DenseVectorWithVariableLength<ElementType> operator+(const DenseVectorWithVariableLength<ElementType>& VectorA,
                                                            const DenseVectorWithVariableLength<ElementType>& VectorB);

template<typename ElementType>
inline DenseVectorWithVariableLength<ElementType> operator-(const DenseVectorWithVariableLength<ElementType>& VectorA,
                                                            const DenseVectorWithVariableLength<ElementType>& VectorB);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline DenseVectorWithVariableLength<ElementType> operator+(const DenseVectorWithVariableLength<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVectorWithVariableLength<ElementType> operator-(const DenseVectorWithVariableLength<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVectorWithVariableLength<ElementType> operator*(const DenseVectorWithVariableLength<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVectorWithVariableLength<ElementType> operator/(const DenseVectorWithVariableLength<ElementType>& Vector, const ElementType& Element);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline DenseVectorWithVariableLength<ElementType> operator+(const ElementType& Element, const DenseVectorWithVariableLength<ElementType>& Vector);

template<typename ElementType>
inline DenseVectorWithVariableLength<ElementType> operator-(const ElementType& Element, const DenseVectorWithVariableLength<ElementType>& Vector);

template<typename ElementType>
inline DenseVectorWithVariableLength<ElementType> operator*(const ElementType& Element, const DenseVectorWithVariableLength<ElementType>& Vector);

template<typename ElementType>
inline DenseVectorWithVariableLength<ElementType> operator/(const ElementType& Element, const DenseVectorWithVariableLength<ElementType>& Vector);

//---------------------------------------------------------------------------------------------------------------//

}

#include "mdkDenseVectorWithVariableLength_Operator.hpp"

#endif