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
private:
	//-------------------- input -------------------------------//
	PolygonMesh<MeshAttribute> m_InputMesh;
	DenseMatrix<ScalarType> m_InputDisplacementField;
	DenseMatrix<ScalarType> m_ConfidenceOfInputDisplacementField;
	DenseMatrix<ScalarType> m_ConfidenceOfSmoothness;
	WeightTypeEnum m_WeigthType;

	//----------------- internal ----------------------------//
	ObjectArray<SparseVector<ScalarType>> m_WeightMatrix;
	
    //--------------- output ------------------------------//
	DenseMatrix<ScalarType> m_OutputDisplacementField;
	PolygonMesh<MeshAttribute> m_OutputMesh;

public:
	DisplacementBasedPolygonMeshDeformer();
	~DisplacementBasedPolygonMeshDeformer();
	void Clear();
	PolygonMesh<MeshAttribute>& InputMesh() { return m_InputMesh; }
	DenseMatrix<ScalarType>& InputDisplacementField() { return m_InputDisplacementField; }
	DenseMatrix<ScalarType>& ConfidenceOfInputDisplacementField() { return m_ConfidenceOfInputDisplacementField; }
	DenseMatrix<ScalarType>& ConfidenceOfSmoothness() { return m_ConfidenceOfSmoothness; }
	
	void Update();
	PolygonMesh<MeshAttribute>& OutputMesh() { return m_OutputMesh; }
	DenseMatrix<ScalarType>& OutputDisplacementField() { return m_OutputDisplacementField; }

private:
	bool CheckInput();
	void ComputeWeightMatrix();
	void ComputeDisplacementField();

private:
	DisplacementBasedPolygonMeshDeformer(const DisplacementBasedPolygonMeshDeformer&) = delete;
	void operator=(const DisplacementBasedPolygonMeshDeformer&) = delete;
};

}//namespace mdk

#include "mdkDisplacementBasedPolygonMeshDeformer.hpp"

