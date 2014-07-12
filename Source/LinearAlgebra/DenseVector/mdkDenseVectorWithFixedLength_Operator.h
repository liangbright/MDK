#ifndef __mdkDenseVectorWithFixedLength_Operator_h
#define __mdkDenseVectorWithFixedLength_Operator_h

#include <array>

#include "mdkType.h"

namespace mdk
{

//---------------------------------------------------- Vector {+ - * /} Vector ---------------------------------------------// 
// * : element by element, i.e., dot product
// / : element by element

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator+(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType, Length>& VectorB);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator-(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType, Length>& VectorB);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator*(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType, Length>& VectorB);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator/(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType, Length>& VectorB);

//-------------------- fixed + -/ * unfixed-----------------------------

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator+(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType>& VectorB);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator-(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType>& VectorB);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator*(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType>& VectorB);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator/(const DenseVector<ElementType, Length>& VectorA, const DenseVector<ElementType>& VectorB);

//-------------------- unfixed + -/ * fixed -----------------------------

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator+(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, Length>& VectorB);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator-(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, Length>& VectorB);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator*(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, Length>& VectorB);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator/(const DenseVector<ElementType>& VectorA, const DenseVector<ElementType, Length>& VectorB);

//---------------------------------------------------- Vector {+ - * /} Element ---------------------------------------------// 

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator+(const DenseVector<ElementType, Length>& Vector, const ElementType& Element);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator-(const DenseVector<ElementType, Length>& Vector, const ElementType& Element);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator*(const DenseVector<ElementType, Length>& Vector, const ElementType& Element);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator/(const DenseVector<ElementType, Length>& Vector, const ElementType& Element);

//----------------------------------------------------  Element {+ - * /} Vector ---------------------------------------------// 

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator+(const ElementType& Element, const DenseVector<ElementType, Length>& Vector);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator-(const ElementType& Element, const DenseVector<ElementType, Length>& Vector);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator*(const ElementType& Element, const DenseVector<ElementType, Length>& Vector);

template<typename ElementType, int_max Length>
inline DenseVector<ElementType, Length> operator/(const ElementType& Element, const DenseVector<ElementType, Length>& Vector);

}

#include "mdkDenseVectorWithFixedLength_Operator.hpp"

#endif