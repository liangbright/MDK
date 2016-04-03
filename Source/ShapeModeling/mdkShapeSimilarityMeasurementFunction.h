#pragma once

#include "mdkDenseMatrix.h" 
#include "mdkGeometry2D.h"
#include "mdkGeometry3D.h"

namespace mdk
{
template<typename ScalarType>
inline ScalarType ComputeSimilarityBetweenShape_By_NormalizedMeanDistance(ScalarType NMD);

//------------------------------------------ use RigidTransform or SimilarityTransform  -------------------------------------//
//2D Shape if 2xN matrix
//3D Shape if 3xN matrix
//TransformName: RigidTransform, SimilarityTransform
//Flag_Symmetry true: Similarity(A,B)=Similarity(B,A)
//
template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const std::string& TransformName, bool Flag_Symmetry);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence2D_RigidTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence2D_SimilarityTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence3D_RigidTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence3D_SimilarityTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB);

// output Similarity between ShapeList[i] and ShapeList[j]
// SimilarityMatrix(k,k)=1
template<typename ScalarType>
DenseMatrix<ScalarType> ComputeSimilarityBetweenShapeWithPointCorrespondence(const ObjectArray<DenseMatrix<ScalarType>>& ShapeList, const std::string& TransformName, bool Flag_Symmetry, int_max MaxThreadCount);

//--------------------------------- use landmark to get transform, transform can be TPS --------------------------------------------
// Landmark: point index list
// TransformName: SimilarityTransform, RigidTransform, ThinPlateSplineTransform

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const DenseVector<int_max>& Landmark, const std::string& TransformName, bool Flag_Symmetry);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence2D_RigidTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const DenseVector<int_max>& Landmark);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence2D_SimilarityTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const DenseVector<int_max>& Landmark);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence2D_ThinPlateSplineTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const DenseVector<int_max>& Landmark);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence3D_RigidTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const DenseVector<int_max>& Landmark);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence3D_SimilarityTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const DenseVector<int_max>& Landmark);

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence3D_ThinPlateSplineTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const DenseVector<int_max>& Landmark);

// output Similarity between ShapeList[i] and ShapeList[j]
// SimilarityMatrix(k,k)=1
template<typename ScalarType>
DenseMatrix<ScalarType> ComputeSimilarityBetweenShapeWithPointCorrespondence(const ObjectArray<DenseMatrix<ScalarType>>& ShapeList, const DenseVector<int_max>& Landmark, const std::string& TransformName, bool Flag_Symmetry, int_max MaxThreadCount);

}

#include "mdkShapeSimilarityMeasurementFunction.hpp"