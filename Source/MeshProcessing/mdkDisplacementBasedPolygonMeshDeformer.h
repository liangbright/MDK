#pragma once

#include <Eigen/Sparse>

#include "mdkLinearAlgebra.h"
#include "mdkGeometry.h"
#include "mdkPolygonMesh.h"

namespace mdk
{
template<typename MeshAttribute>
class DisplacementBasedPolygonMeshDeformer : public Object
{
public:
	typedef typename MeshAttribute::ScalarType  ScalarType;

	enum struct WeightTypeEnum { One, Distance, Unknown };
public:
	//-------------------- input -------------------------------//
	PolygonMesh<MeshAttribute> InputMesh;
	DenseMatrix<ScalarType> InputDisplacementField;
	DenseMatrix<ScalarType> ConfidenceOfInputDisplacementField;
	DenseMatrix<ScalarType> ConfidenceOfSmoothness;
	WeightTypeEnum WeigthType;
private:
	//----------------- internal ----------------------------//
	ObjectArray<SparseVector<ScalarType>> WeightMatrix;

public:	
    //--------------- output ------------------------------//
	DenseMatrix<ScalarType> OutputDisplacementField;
	PolygonMesh<MeshAttribute> OutputMesh;

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

