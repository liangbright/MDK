#pragma once

#include <Eigen/Sparse>

#include "mdkLinearAlgebra.h"
#include "mdkGeometry.h"
#include "mdkPolygonMesh.h"

namespace mdk
{
enum struct WeightTypeEnum_of_DisplacementBasedPolygonMeshDeformer { One, Distance, Unknown };

template<typename MeshAttribute>
struct Input_of_DisplacementBasedPolygonMeshDeformer
{
	typedef WeightTypeEnum_of_DisplacementBasedPolygonMeshDeformer WeightTypeEnum;

	PolygonMesh<MeshAttribute> SourceMesh;
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

template<typename MeshAttribute>
struct Output_of_DisplacementBasedPolygonMeshDeformer
{
	typedef typename MeshAttribute::ScalarType  ScalarType;
	DenseMatrix<ScalarType> DisplacementField;
	PolygonMesh<MeshAttribute> DeformedMesh;
};

template<typename MeshAttribute>
class DisplacementBasedPolygonMeshDeformer : public Object
{
public:
	typedef typename MeshAttribute::ScalarType  ScalarType;
	typedef WeightTypeEnum_of_DisplacementBasedPolygonMeshDeformer WeightTypeEnum;
public:	
	Input_of_DisplacementBasedPolygonMeshDeformer<MeshAttribute> Input;	
private:	
	Internal_of_DisplacementBasedPolygonMeshDeformer<ScalarType> Internal;
public:    
	Output_of_DisplacementBasedPolygonMeshDeformer<MeshAttribute> Output;	

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

