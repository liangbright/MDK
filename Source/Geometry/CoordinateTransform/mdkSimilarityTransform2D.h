#pragma once

#include "mdkCoordinateTransform2D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
// TargetPoint = SimilarityTransform(SourcePoint)
//	           = Scale*RotationMatrix*SourcePoint+Translation
//             = Scale*RotationMatrix*(SourcePoint-RotationCenter) + RotationCenter
// Y = sRX+T <=> Y=s*R(X-C)+C, s is scaling, C is rotation center and C = pinv(I-s*R)*(Yc-s*R*Xc), Yc=mean(Y), Xc=mean(X)
//
// use right-hand coordinate system
// Reference: Least-squares Estimation of Transformation Parameters Between Two Point Patterns, 1991

template<typename Scalar_Type>
class SimilarityTransform2D : public CoordinateTransform2D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceLandmarkPointSet and m_TargetLandmarkPointSet must be valid when EstimateParameter() is called
	const DenseMatrix<ScalarType>* m_SourceLandmarkPointSet; // 2 x N
	const DenseMatrix<ScalarType>* m_TargetLandmarkPointSet; // 2 x N

	ScalarType m_Scale;
	DenseMatrix<ScalarType> m_Rotation;//2x2
	DenseMatrix<ScalarType> m_Translation_After_Scale_Rotation;//2x1

public:
	SimilarityTransform2D();
	~SimilarityTransform2D();

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

	inline DenseVector<ScalarType, 2> TransformPoint(ScalarType x, ScalarType y) const;
	using CoordinateTransform2D::TransformPoint;

private:
	bool CheckLandmarkPointSet();

private:
	SimilarityTransform2D(const SimilarityTransform2D&) = delete;
	void operator=(const SimilarityTransform2D&) = delete;
};

}//namespace mdk

#include "mdkSimilarityTransform2D.hpp"
