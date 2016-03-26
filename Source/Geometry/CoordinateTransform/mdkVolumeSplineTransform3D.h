#pragma once

#include "mdkCoordinateTransform3D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
// reference MDK_Document
// Volume Spline Transformation.doc and TPS3D.m
// "A physics based coordinate transform for 3D image match" at TMI 1997

// TargetPoint = VolumeSplineTransform(SourcePoint)

// ScalarType is float or double
template<typename Scalar_Type>
class VolumeSplineTransform3D : public CoordinateTransform3D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceLandmarkPointSet must be valid during EstimateParameter() and TransformPoint()
	const DenseMatrix<ScalarType>* m_SourceLandmarkPointSet; // 3 x N

	// m_TargetLandmarkPointSet must be valid during EstimateParameter()
	const DenseMatrix<ScalarType>* m_TargetLandmarkPointSet; // 3 x N
	
	DenseMatrix<ScalarType> m_Parameter;  // (N+4) x 3

public:
	VolumeSplineTransform3D();
	~VolumeSplineTransform3D();

	void Clear();

	void SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet);
	void SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet);
	void EstimateParameter();

	void SetParameter(const DenseMatrix<ScalarType>& Parameter);
	DenseMatrix<ScalarType> GetParameter() const;

	inline DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z) const;
	using CoordinateTransform3D::TransformPoint;

private:
	bool CheckLandmarkPointSet();

private:
	VolumeSplineTransform3D(const VolumeSplineTransform3D&) = delete;
	void operator=(const VolumeSplineTransform3D&) = delete;
};

}//namespace mdk

#include "mdkVolumeSplineTransform3D.hpp"
