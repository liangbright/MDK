#ifndef __mdkLinearAlgebra_Part_2_h
#define __mdkLinearAlgebra_Part_2_h

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <initializer_list>

#include "mdkObject.h"
#include "mdkMatrix.h"

namespace mdk
{

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

#include "mdkLinearAlgebra_Part_2.hpp"

#endif