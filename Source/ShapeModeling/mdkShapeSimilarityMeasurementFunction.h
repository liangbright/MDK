#pragma once

#include "mdkDenseMatrix.h" 
#include "mdkGeometry2D.h"
#include "mdkGeometry3D.h"

namespace mdk
{
//2D Shape if 2xN matrix
//3D Shape if 3xN matrix
//TransformName: SimilarityTransform, RigidTransform
//Flag_Symmetry true: Similarity(A,B)=Similarity(B,A)
//
template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const std::string& TransformName, bool Flag_Symmetry);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence2D_SimilarityTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence2D_RigidTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence3D_SimilarityTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence3D_RigidTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

// output Similarity between ShapeList[i] and ShapeList[j]
// SimilarityMatrix(k,k)=1
template<typename ScalarType>
DenseMatrix<ScalarType> ComputeSimilarityBetweenShapeWithPointCorrespondence(const ObjectArray<DenseMatrix<ScalarType>>& ShapeList, const std::string& TransformName, bool Flag_Symmetry, int_max MaxThreadCount);

}

#include "mdkShapeSimilarityMeasurementFunction.hpp"