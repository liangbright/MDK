#pragma once

#include "mdkCoordinateTransform3D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
// TargetPoint = AffineTransform(SourcePoint)
// 
//	x_new = a0 + a1 * x + a2 * y + a3 * z;
//	y_new = b0 + b1 * x + b2 * y + b3 * z;
//	z_new = c0 + c1 * x + c2 * y + c3 * z;
//
// TransformationMatrix =  [a1 a2 a3 a0
//                          b1 b2 b3 b0
//                          c1 c2 c3 c0
//                          0   0  0  1];
//
// Parameter = [a1 a2 a3 a0
//              b1 b2 b3 b0
//              c1 c2 c3 c0];  3x4
//
// SourceData = [x_1 x_2 ... x_N
//               y_1 y_2 ... y_N
//               z_1 z_2 ... z_N
//               1   1   ... 1]; 4xN
//
// TargetData = Parameter * SourceData;
// TargetData = [XData; YData; ZData];  3xN
//
// Parameter = TargetData*pinv(SourceData); SourceData has full row rank => pinv(SourceData) is right inverse
//------------------------------------------------------------------------------------------------------------

// ScalarType is float or double
template<typename Scalar_Type>
class AffineTransform3D : public CoordinateTransform3D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceLandmarkPointSet and m_TargetLandmarkPointSet must be valid when EstimateParameter() is called
	const DenseMatrix<ScalarType>* m_SourceLandmarkPointSet; // 3 x N
	const DenseMatrix<ScalarType>* m_TargetLandmarkPointSet; // 3 x N

	DenseMatrix<ScalarType> m_Parameter;  // 3 x 4

public:
	AffineTransform3D();
	~AffineTransform3D();

	void Clear();

	void SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet);
	void SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet);
	void EstimateParameter();

	void SetTransformationMatrix(const DenseMatrix<ScalarType>& TransformationMatrix);
	DenseMatrix<ScalarType> GetTransformationMatrix() const;

	inline DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const;
	using CoordinateTransform3D::TransformPoint;

private:
	bool CheckLandmarkPointSet();

private:
	AffineTransform3D(const AffineTransform3D&) = delete;
	void operator=(const AffineTransform3D&) = delete;
};

}//namespace mdk

#include "mdkAffineTransform3D.hpp"

