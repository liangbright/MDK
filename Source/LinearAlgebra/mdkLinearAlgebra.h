#ifndef __mdkLinearAlgebra_h
#define __mdkLinearAlgebra_h

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <initializer_list>

#include "mdkObject.h"
#include "mdkMatrix.h"

namespace mdk
{

//------------------------- compensate performancel loss in these simple cases when using mdkGlueMatrix -------------------------//
template<typename ElementType>
inline mdkMatrix<ElementType> MatrixAdd(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixDiff(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixMultiply(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixAdd(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixDiff(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixMultiply(const ElementType& Element, const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixAdd(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixDiff(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixMultiply(const mdkMatrix<ElementType>& Matrix, const ElementType& Element);

//----------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> MatrixLinearCombine(const std::vector<double>& AlphaList, const std::vector<mdkMatrix<ElementType>*>& MatrixList);

template<typename ElementType, uint64 Length>
inline std::array<ElementType, Length> MatrixLinearCombine(const std::vector<double>& CoefList, const std::vector<std::array<ElementType, Length>*>& ArrayList);

template<typename ElementType>
inline std::vector<ElementType> MatrixLinearCombine(const std::vector<double>& CoefList, const std::vector<std::vector<ElementType>*>& ArrayList);

//----------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
struct mdkMatrixPCAResult
{
    // Matrix = M+U*S*U';

    mdkMatrix<ElementType> M;    // mean
    mdkMatrix<ElementType> U;    // eigenvector
    mdkMatrix<ElementType> S;    // matrix  : full or vector ?
};

template<typename ElementType>
inline mdkMatrixPCAResult<ElementType> PCA(const mdkMatrix<ElementType>& Matrix);

template<typename ElementType>
inline mdkMatrix<ElementType> Convolution(const mdkMatrix<ElementType>& Matrix, const mdkMatrix<ElementType>& Mask, const char* Option);

}//end namespace mdk

#include "mdkLinearAlgebra.hpp"

#endif