#pragma once

#include "mdkCoordinateTransform2D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
// TargetPoint = RotationTransform(SourcePoint)= RotationMatrix*(SourcePoint-RotationCenter)+RotationCenter
// Y = RX+T => Y=R(X-C)+C if pure rotation , C is rotation center and C = pinv(I-R)*(Yc-R*Xc), Yc=mean(Y), Xc=mean(X)
//
// RotationMatrix: rotate a-angle around Z-axis
// Rx = [cos(a), -sin(a)
//       sin(a), cos(a)]
//
//
// use right-hand coordinate system
// Reference: Estimating 3-D rigid body transformations: a comparison of four major algorithms

template<typename Scalar_Type>
class RotationTransform2D : public CoordinateTransform2D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceLandmarkPointSet and m_TargetLandmarkPointSet must be valid when EstimateParameter() is called
	const DenseMatrix<ScalarType>* m_SourceLandmarkPointSet; // 2 x N
	const DenseMatrix<ScalarType>* m_TargetLandmarkPointSet; // 2 x N

	DenseMatrix<ScalarType> m_Rotation;
	DenseVector<ScalarType, 2> m_RotationCenter;
	DenseVector<ScalarType, 2> m_Translation_AfterRotation;

public:
	RotationTransform2D();
	~RotationTransform2D();

	void Clear();

	void SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet);
	void SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet);
	void EstimateParameter();

	void SetRotationMatrix(const DenseMatrix<ScalarType>& Rotation);
	DenseMatrix<ScalarType> GetRotationMatrix() const;

	void SetRotationCenter(const DenseVector<ScalarType, 2>& Center);
	DenseVector<ScalarType, 2> GetRotationCenter() const;

	inline DenseVector<ScalarType, 2> TransformPoint(ScalarType x, ScalarType y) const;
	using CoordinateTransform2D::TransformPoint;

private:
	bool CheckLandmarkPointSet();

public:
	static DenseMatrix<ScalarType> ComputeRotationMatrix_around_Z_axis(ScalarType Angle);

	static DenseVector<ScalarType, 2> RotatePoint(const DenseVector<ScalarType, 2>& PointPosition,											      
												  const DenseMatrix<ScalarType>&    RotationMatrix,
												  const DenseVector<ScalarType, 2>& RotationCenter);

	//RotationCenter is [0, 0, 0]
	static DenseVector<ScalarType, 2> RotatePoint(const DenseVector<ScalarType, 2>& PointPosition,
												  const DenseMatrix<ScalarType>& RotationMatrix);
private:
	RotationTransform2D(const RotationTransform2D&) = delete;
	void operator=(const RotationTransform2D&) = delete;
};

}//namespace mdk

#include "mdkRotationTransform2D.hpp"
