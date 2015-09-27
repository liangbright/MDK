#ifndef mdk_RotationTransform3D_h
#define mdk_RotationTransform3D_h

#include "mdkCoordinateTransform3D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
// TargetPoint = RotationTransform(SourcePoint)= RotationMatrix*SourcePoint
// RotationMatrix = Rz*Ry*Rx;
// Rx = [1,      0,  0
//       0, cos(a), -sin(a)
//       0, sin(a), cos(a)]
//
// Ry = [cos(b),   0,  sin(b)
//            0,   1,  0
//       -sin(b),  0,  cos(b)]
//
// Rx = [cos(c),  -sin(c),  0
//       sin(c),   cos(c),  0
//            0,        0,  1]
//
// use right-hand coordinate system
// Reference: Estimating 3-D rigid body transformations: a comparison of four major algorithms

template<typename Scalar_Type>
class RotationTransform3D : public CoordinateTransform3D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceLandmarkPointSet and m_TargetLandmarkPointSet must be valid when EstimateParameter() is called
	const DenseMatrix<ScalarType>* m_SourceLandmarkPointSet; // 3 x N
	const DenseMatrix<ScalarType>* m_TargetLandmarkPointSet; // 3 x N

	DenseMatrix<ScalarType> m_Rotation;
	DenseVector<ScalarType, 3> m_RotationCenter;

public:
	RotationTransform3D();
	~RotationTransform3D();

	void Clear();

	void SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet);
	void SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet);
	void EstimateParameter();

	void SetRotationMatrix(const DenseMatrix<ScalarType>& Rotation);
	DenseMatrix<ScalarType> GetRotationMatrix() const;

	void SetRotationCenter(const DenseVector<ScalarType, 3>& Center);
	DenseVector<ScalarType, 3> GetRotationCenter() const;

	DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const;
	using CoordinateTransform3D::TransformPoint;

private:
	bool CheckLandmarkPointSet();

public:
	static DenseMatrix<ScalarType> ComputeRotationMatrix_around_X_axis(ScalarType Angle);
	static DenseMatrix<ScalarType> ComputeRotationMatrix_around_Y_axis(ScalarType Angle);
	static DenseMatrix<ScalarType> ComputeRotationMatrix_around_Z_axis(ScalarType Angle);
	static DenseMatrix<ScalarType> ComputeRotationMatrix_around_Direction(ScalarType Angle, const DenseVector<ScalarType, 3>& Direction);

	static DenseVector<ScalarType, 3> RotatePoint(const DenseVector<ScalarType, 3>& PointPosition,											      
												  const DenseMatrix<ScalarType>& RotationMatrix,
												  const DenseVector<ScalarType, 3>& RotationCenter);

	//RotationCenter is [0, 0, 0]
	static DenseVector<ScalarType, 3> RotatePoint(const DenseVector<ScalarType, 3>& PointPosition,
												  const DenseMatrix<ScalarType>& RotationMatrix);
private:
	RotationTransform3D(const RotationTransform3D&) = delete;
	void operator=(const RotationTransform3D&) = delete;
};

}//namespace mdk

#include "mdkRotationTransform3D.hpp"

#endif
