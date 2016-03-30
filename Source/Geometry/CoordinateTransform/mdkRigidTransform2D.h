#pragma once

#include "mdkCoordinateTransform2D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
// TargetPoint = RigidTransform(SourcePoint)= RotationMatrix*SourcePoint + TranslationVector
// Y = RX+T
//
// RotationMatrix: rotate a-angle around Z-axis
// Rx = [cos(a), -sin(a)
//       sin(a), cos(a)]
//
//
// use right-hand coordinate system
// Reference: Estimating 3-D rigid body transformations: a comparison of four major algorithms

template<typename Scalar_Type>
class RigidTransform2D : public CoordinateTransform2D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceLandmarkPointSet and m_TargetLandmarkPointSet must be valid when EstimateParameter() is called
	const DenseMatrix<ScalarType>* m_SourceLandmarkPointSet; // 2 x N
	const DenseMatrix<ScalarType>* m_TargetLandmarkPointSet; // 2 x N

	DenseMatrix<ScalarType> m_Rotation;//2x2
	DenseMatrix<ScalarType> m_Translation_AfterRotation;//2x1

public:
	RigidTransform2D();
	~RigidTransform2D();

	void Clear();

	void SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet) { m_SourceLandmarkPointSet = SourceLandmarkPointSet; }
	void SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet) { m_TargetLandmarkPointSet = TargetLandmarkPointSet; }
	void EstimateParameter();

	void SetRotationMatrix(const DenseMatrix<ScalarType>& Rotation) { m_Rotation = Rotation; }
	DenseMatrix<ScalarType> GetRotationMatrix() const { return m_Rotation; }

	void SetTranslation_AfterRotation(const DenseMatrix<ScalarType>& Translation) { m_Translation_AfterRotation = Translation; }
	DenseMatrix<ScalarType> GetTranslation_AfterRotation() const { return m_Translation_AfterRotation; }

	inline DenseVector<ScalarType, 2> TransformPoint(ScalarType x, ScalarType y) const;
	using CoordinateTransform2D::TransformPoint;

private:
	bool CheckLandmarkPointSet();

private:
	RigidTransform2D(const RigidTransform2D&) = delete;
	void operator=(const RigidTransform2D&) = delete;
};

}//namespace mdk

#include "mdkRigidTransform2D.hpp"
