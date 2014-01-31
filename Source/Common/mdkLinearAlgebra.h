#ifndef __mdkLinearAlgebra_h
#define __mdkLinearAlgebra_h

#include <vector>
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

	mdkMatrix<ScalarType> M; // mean
	mdkMatrix<ScalarType> U; // eigenvector
	mdkMatrix<ScalarType> S;  // matrix  : change to vector?
};

template<typename ScalarType>
inline static mdkMatrix<ScalarType> Transpose(mdkMatrix<ScalarType>& Matrix);

template<typename ScalarType>
inline static mdkMatrix<ScalarType> ElementOperation(std::string Operator,
		                                                 mdkMatrix<ScalarType>& MatrixA,
		                                                 mdkMatrix<ScalarType>& MatrixB);

template<typename OperatorType, typename ScalarType>
inline static mdkMatrix<ScalarType> ElementOperation(OperatorType Operator,
	                                                 mdkMatrix<ScalarType>& MatrixA,
	                                                 mdkMatrix<ScalarType>& MatrixB);

template<typename ScalarType>
inline static mdkMatrix<ScalarType> Convolution(mdkMatrix<ScalarType>& MatrixA, mdkMatrix<ScalarType>& MatrixB);

template<typename ScalarType>
inline	static mdkMatrix<ScalarType> LinearCombine(double alpha1, mdkMatrix<ScalarType>& Matrix1, double alpha2, mdkMatrix<ScalarType>& Matrix2);

template<typename ScalarType>
inline	static mdkMatrix<ScalarType> LinearCombine(double alpha1, mdkMatrix<ScalarType>& Matrix1, double alpha2, mdkMatrix<ScalarType>& Matrix2,
	                                               double alpha3, mdkMatrix<ScalarType>& Matrix3);

template<typename ScalarType>
inline	static mdkMatrix<ScalarType> LinearCombine(double alpha1, mdkMatrix<ScalarType>& Matrix1, double alpha2, mdkMatrix<ScalarType>& Matrix2,
	                                               double alpha3, mdkMatrix<ScalarType>& Matrix3, double alpha4, mdkMatrix<ScalarType>& Matrix4);

template<typename ScalarType>
inline static mdkMatrix<ScalarType> LinearCombine(std::vector<double> AlphaList, std::vector<mdkMatrix<ScalarType>*> MatrixList);

template<typename ScalarType>
inline static std::vector<ScalarType> LinearCombine(std::vector<double> AlphaList, std::vector<std::vector<ScalarType>*> ArrayList);

template<typename ScalarType>
inline static std::vector<ScalarType> LinearCombine(std::vector<double> AlphaList, std::vector<ScalarType> BetaList);


template<typename ScalarType>
inline mdkMatrixPCAResult<ScalarType> PCA(mdkMatrix<ScalarType>& Matrix);

template<typename ScalarType>
inline mdkMatrix<ScalarType> Convolution(mdkMatrix<ScalarType> Matrix, mdkMatrix<ScalarType> Mask, std::string Option);

}//end namespace mdk

#include "mdkLinearAlgebra.hpp"

#endif