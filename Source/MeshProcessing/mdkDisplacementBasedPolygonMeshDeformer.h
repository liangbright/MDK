#pragma once

#include <Eigen/Sparse>

#include "mdkLinearAlgebra.h"
#include "mdkGeometry.h"
#include "mdkPolygonMesh.h"

namespace mdk
{
enum struct WeightTypeEnum_of_DisplacementBasedPolygonMeshDeformer { One, Distance, Unknown };

template<typename ScalarType>
struct Input_of_DisplacementBasedPolygonMeshDeformer
{
	typedef WeightTypeEnum_of_DisplacementBasedPolygonMeshDeformer WeightTypeEnum;

	PolygonMesh<ScalarType> SourceMesh;
	DenseMatrix<ScalarType> DisplacementField;
	DenseMatrix<ScalarType> ConfidenceOfDisplacementField;
	DenseMatrix<ScalarType> ConfidenceOfSmoothness;
	WeightTypeEnum WeigthType;
};

template<typename ScalarType>
struct Internal_of_DisplacementBasedPolygonMeshDeformer
{
	ObjectArray<SparseVector<ScalarType>> WeightMatrix;
};

template<typename ScalarType>
struct Output_of_DisplacementBasedPolygonMeshDeformer
{
	DenseMatrix<ScalarType> DisplacementField;
	PolygonMesh<ScalarType> DeformedMesh;
};

template<typename Scalar_Type>
class DisplacementBasedPolygonMeshDeformer : public Object
{
public:
	typedef Scalar_Type  ScalarType;
	typedef WeightTypeEnum_of_DisplacementBasedPolygonMeshDeformer WeightTypeEnum;
public:	
	Input_of_DisplacementBasedPolygonMeshDeformer<ScalarType> Input;
private:	
	Internal_of_DisplacementBasedPolygonMeshDeformer<ScalarType> Internal;
public:    
	Output_of_DisplacementBasedPolygonMeshDeformer<ScalarType> Output;

public:
	DisplacementBasedPolygonMeshDeformer();
	~DisplacementBasedPolygonMeshDeformer();
	void Clear();
	void Update();

private:
	bool CheckInput();
	void ComputeWeightMatrix();
	void ComputeWeightMatrix_Type_One();
	void ComputeWeightMatrix_Type_Distance();
	void ComputeDisplacementField();

private:
	DisplacementBasedPolygonMeshDeformer(const DisplacementBasedPolygonMeshDeformer&) = delete;
	void operator=(const DisplacementBasedPolygonMeshDeformer&) = delete;
};

}//namespace mdk

#include "mdkDisplacementBasedPolygonMeshDeformer.hpp"

