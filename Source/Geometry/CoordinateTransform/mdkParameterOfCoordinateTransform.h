#pragma once

#include "mdkDenseMatrix.h"

namespace mdk
{

template<typename Scalar_Type>
struct Parameter_Of_SimilarityTransform
{
	typedef Scalar_Type ScalarType;

	ScalarType Scale;
	DenseMatrix<ScalarType> Rotation;
	DenseMatrix<ScalarType> Translation;
};

template<typename Scalar_Type>
struct Parameter_Of_RigidTransform
{
	typedef Scalar_Type ScalarType;

	DenseMatrix<ScalarType> Rotation;
	DenseMatrix<ScalarType> Translation;
};

}