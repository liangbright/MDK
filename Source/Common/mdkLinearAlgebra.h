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

template<typename ScalarType>
struct mdkMatrixPCAResult
{
	// Matrix = M+U*S*U';

	mdkMatrix<ScalarType> M;    // mean
	mdkMatrix<ScalarType> U;    // eigenvector
	mdkMatrix<ScalarType> S;    // matrix  : change to vector?
};

template<typename ScalarType>
inline mdkMatrix<ScalarType> LinearCombine(const std::vector<double>& AlphaList, const std::vector<mdkMatrix<ScalarType>*>& MatrixList);

template<typename ScalarType, uint64 Length>
inline std::array<ScalarType, Length> LinearCombine(const std::vector<double>& CoefList, const std::vector<std::array<ScalarType, Length>*>& ArrayList);

template<typename ScalarType>
inline std::vector<ScalarType> LinearCombine(const std::vector<double>& CoefList, const std::vector<std::vector<ScalarType>*>& ArrayList);

template<typename ScalarType>
inline mdkMatrixPCAResult<ScalarType> PCA(const mdkMatrix<ScalarType>& Matrix);

template<typename ScalarType>
inline mdkMatrix<ScalarType> Convolution(const mdkMatrix<ScalarType>& Matrix, const mdkMatrix<ScalarType>& Mask, const char* Option);

}//end namespace mdk

#include "mdkLinearAlgebra.hpp"

#endif