#pragma once

#include "mdkDenseMatrix.h"
#include "mdkCoordinateTransform3D.h"
#include "mdkParameterOfCoordinateTransform.h"

namespace mdk
{
// TargetPoint = SimilarityTransform(SourcePoint) = Scale*RotationMatrix*SourcePoint+Translation
// Y = sRX+T : s is scaling, R is rotation, T is translation
//
// use right-hand coordinate system
// Reference: Least-squares Estimation of Transformation Parameters Between Two Point Patterns, 1991

template<typename Scalar_Type>
class SimilarityTransform3D : public CoordinateTransform3D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceLandmarkPointSet and m_TargetLandmarkPointSet must be valid when EstimateParameter() is called
	const DenseMatrix<ScalarType>* m_SourceLandmarkPointSet; // 3 x N
	const DenseMatrix<ScalarType>* m_TargetLandmarkPointSet; // 3 x N

	ScalarType m_Scale;
	DenseMatrix<ScalarType> m_Rotation; // 3x3
	DenseMatrix<ScalarType> m_Translation_After_Scale_Rotation;//3x1

public:
	SimilarityTransform3D();
	~SimilarityTransform3D();

	void Clear();

	void SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet) { m_SourceLandmarkPointSet = SourceLandmarkPointSet; }
	void SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet) { m_TargetLandmarkPointSet = TargetLandmarkPointSet; }
	void EstimateParameter();

	void SetScale(ScalarType Scale) { m_Scale = Scale; }
	ScalarType GetScale() const { return m_Scale; }

	void SetRotationMatrix(const DenseMatrix<ScalarType>& Rotation) { m_Rotation = Rotation; }
	DenseMatrix<ScalarType> GetRotationMatrix() const { return m_Rotation; }

	void SetTranslation_After_Scale_Rotation(const DenseMatrix<ScalarType>& Translation) { m_Translation_After_Scale_Rotation = Translation; }
	DenseMatrix<ScalarType> GetTranslation_After_Scale_Rotation() const { return m_Translation_After_Scale_Rotation; }

	inline DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const;
	using CoordinateTransform3D::TransformPoint;

private:
	bool CheckLandmarkPointSet();

private:
	SimilarityTransform3D(const SimilarityTransform3D&) = delete;
	void operator=(const SimilarityTransform3D&) = delete;
};

}//namespace mdk

#include "mdkSimilarityTransform3D.hpp"

