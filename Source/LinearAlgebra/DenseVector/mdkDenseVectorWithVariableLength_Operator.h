#ifndef __mdkDenseVectorWithVariableLength_Operator_h
#define __mdkDenseVectorWithVariableLength_Operator_h

#include <vector>

#include "mdkType.h"

namespace mdk
{

template<typename ElementType>
inline DenseVector<ElementType> operator+(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType>& VectorB);

template<typename ElementType>
inline DenseVector<ElementType> operator-(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType>& VectorB);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline DenseVector<ElementType> operator+(const DenseVector<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVector<ElementType> operator-(const DenseVector<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVector<ElementType> operator*(const DenseVector<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVector<ElementType> operator/(const DenseVector<ElementType>& Vector, const ElementType& Element);

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline DenseVector<ElementType> operator+(const ElementType& Element, const DenseVector<ElementType>& Vector);

template<typename ElementType>
inline DenseVector<ElementType> operator-(const ElementType& Element, const DenseVector<ElementType>& Vector);

template<typename ElementType>
inline DenseVector<ElementType> operator*(const ElementType& Element, const DenseVector<ElementType>& Vector);

template<typename ElementType>
inline DenseVector<ElementType> operator/(const ElementType& Element, const DenseVector<ElementType>& Vector);

//---------------------------------------------------------------------------------------------------------------//

}

#include "mdkDenseVectorWithVariableLength_Operator.hpp"

#endif