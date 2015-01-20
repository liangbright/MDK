#ifndef mdk_ThinPlateSplineTransform3D_h
#define mdk_ThinPlateSplineTransform3D_h

#include "mdkCoordinateTransform3D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
// reference MDK_Document
// Thin-Plate Spline Transformation.doc and TPS3D.m
// "A physics based coordinate transform for 3D image match" at TMI 1997

// TargetPoint = TPSTransform(SourcePoint)

// ScalarType is float or double
template<typename Scalar_Type>
class ThinPlateSplineTransform3D : public CoordinateTransform3D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceControlPointSet must be valid during EstimateParameter() and TransformPoint()
	const DenseMatrix<ScalarType>* m_SourceControlPointSet; // 3 x N

	// m_TargetControlPointSet must be valid during EstimateParameter()
	const DenseMatrix<ScalarType>* m_TargetControlPointSet; // 3 x N
	
	DenseMatrix<ScalarType> m_Parameter;  // (N+4) x 3

public:
	ThinPlateSplineTransform3D();
	~ThinPlateSplineTransform3D();

	void Clear();

	void SetSourceControlPointSet(const DenseMatrix<ScalarType>* SourcePointSet);
	void SetTargetControlPointSet(const DenseMatrix<ScalarType>* TargetPointSet);
	void EstimateParameter();

	void SetParameter(const DenseMatrix<ScalarType>& Parameter);
	const DenseMatrix<ScalarType>& GetParameter() const;

	DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const;
	using CoordinateTransform3D::TransformPoint;

private:
	ThinPlateSplineTransform3D(const ThinPlateSplineTransform3D&) = delete;
	void operator=(const ThinPlateSplineTransform3D&) = delete;
};

}//namespace mdk

#include "mdkThinPlateSplineTransform3D.hpp"

#endif
