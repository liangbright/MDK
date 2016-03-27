#pragma once

//#include <vector>

#include "mdkCommonType.h"

namespace mdk
{
//------------------------------- Vector {+ - * /} Vector ----------------------------------------------------------------//
// * : element by element, i.e., dot product
// / : element by element

template<typename ElementType>
inline DenseVector<ElementType> operator+(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType>& VectorB);

template<typename ElementType>
inline DenseVector<ElementType> operator-(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType>& VectorB);

//template<typename ElementType>
//inline DenseVector<ElementType> operator*(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType>& VectorB);

template<typename ElementType>
inline DenseVector<ElementType> operator/(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType>& VectorB);

//------------------------------- Vector {+ - * /} Element ----------------------------------------------------------------//

template<typename ElementType>
inline DenseVector<ElementType> operator+(const DenseVector<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVector<ElementType> operator-(const DenseVector<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVector<ElementType> operator*(const DenseVector<ElementType>& Vector, const ElementType& Element);

template<typename ElementType>
inline DenseVector<ElementType> operator/(const DenseVector<ElementType>& Vector, const ElementType& Element);

//---------------------------------- Element {+ - * /} Vector ------------------------------------------------//

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
