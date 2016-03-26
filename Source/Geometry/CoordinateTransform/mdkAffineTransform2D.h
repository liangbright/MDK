#pragma once

#include "mdkCoordinateTransform2D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
// TargetPoint = AffineTransform(SourcePoint)
// 
//	x_new = a0 + a1 * x + a2 * y;
//	y_new = b0 + b1 * x + b2 * y;
//
// TransformationMatrix =  [a1 a2 a0
//                          b1 b2 b0
//                          0   0  1];
//
// Parameter = [a1 a2 a0
//              b1 b2 b0];  2x3
//
// SourceData = [x_1 x_2 ... x_N
//               y_1 y_2 ... y_N
//               1   1   ... 1]; 3xN
//
// TargetData = Parameter * SourceData;
// TargetData = [XData; YData];  2xN
//
// Parameter = TargetData*pinv(SourceData); SourceData has full row rank => pinv(SourceData) is right inverse
//------------------------------------------------------------------------------------------------------------

// ScalarType is float or double
template<typename Scalar_Type>
class AffineTransform2D : public CoordinateTransform2D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceLandmarkPointSet and m_TargetLandmarkPointSet must be valid when EstimateParameter() is called
	const DenseMatrix<ScalarType>* m_SourceLandmarkPointSet; // 2 x N
	const DenseMatrix<ScalarType>* m_TargetLandmarkPointSet; // 2 x N

	DenseMatrix<ScalarType> m_Parameter;  // 2 x 3

public:
	AffineTransform2D();
	~AffineTransform2D();

	void Clear();

	void SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet);
	void SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet);
	void EstimateParameter();

	void SetTransformationMatrix(const DenseMatrix<ScalarType>& TransformationMatrix);
	DenseMatrix<ScalarType> GetTransformationMatrix() const;

	inline DenseVector<ScalarType, 2> TransformPoint(ScalarType x, ScalarType y) const;
	using CoordinateTransform2D::TransformPoint;

private:
	bool CheckLandmarkPointSet();

private:
	AffineTransform2D(const AffineTransform2D&) = delete;
	void operator=(const AffineTransform2D&) = delete;
};

}//namespace mdk

#include "mdkAffineTransform2D.hpp"

