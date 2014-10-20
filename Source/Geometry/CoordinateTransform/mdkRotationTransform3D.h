#ifndef __mdkRotationTransform3D_h
#define __mdkRotationTransform3D_h

#include "mdkCoordinateTransform3D.h"
#include "mdkDenseMatrix.h"
#include "mdkLinearLeastSquaresProblemSolver.h"

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
//  ref: Estimating 3-D rigid body transformations: a comparison of four major algorithms
// use right-hand coordinate system

template<typename Scalar_Type>
class RotationTransform3D : public CoordinateTransform3D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceControlPointSet and m_TargetControlPointSet must be valid when UpdateParameter() is called
	const DenseMatrix<ScalarType>* m_SourceControlPointSet; // 3 x N
	const DenseMatrix<ScalarType>* m_TargetControlPointSet; // 3 x N

	DenseMatrix<ScalarType> m_Rotation;

public:
	RotationTransform3D();
	~RotationTransform3D();

	void Clear();

	void SetSourceControlPointSet(const DenseMatrix<ScalarType>* SourcePointSet);
	void SetTargetControlPointSet(const DenseMatrix<ScalarType>* TargetPointSet);

	void SetRotationMatrixByAngle(const DenseVector<ScalarType, 3>& AngleList);
	void SetRotationMatrixByAngle(ScalarType AngleX, ScalarType AngleY, ScalarType AngleZ);
		
	void SetRotationMatrix(const DenseMatrix<ScalarType>& Rotation);

	const DenseMatrix<ScalarType>& GetRotationMatrix() const;

	bool Update();

	void UpdateParameter();

	DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const;
	DenseVector<ScalarType, 3> TransformPoint(const DenseVector<ScalarType, 3>& SourcePosition) const;

private:
	bool CheckInput();

public:
	static DenseMatrix<ScalarType> ComputeRotationMatrix_Rx_ByAngle(ScalarType AngleX);
	static DenseMatrix<ScalarType> ComputeRotationMatrix_Ry_ByAngle(ScalarType AngleY);
	static DenseMatrix<ScalarType> ComputeRotationMatrix_Rz_ByAngle(ScalarType AngleZ);
	static DenseMatrix<ScalarType> ComputeRotationMatrixByAngle(const DenseVector<ScalarType, 3>& AngleList);
	static DenseMatrix<ScalarType> ComputeRotationMatrixByAngle(ScalarType AngleX, ScalarType AngleY, ScalarType AngleZ);
	static DenseMatrix<ScalarType> ComputeRotationMatrixByAngleAndAxis(ScalarType Angle, const DenseVector<ScalarType, 3>& Axis);

private:
	RotationTransform3D(const RotationTransform3D&) = delete;
	void operator=(const RotationTransform3D&) = delete;
};

}//namespace mdk

#include "mdkRotationTransform3D.hpp"

#endif
