#pragma once

#include "mdkCoordinateTransform2D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
// TargetPoint = TPSTransform(SourcePoint)
// TPS2D can be done by (1) standard TPS2D (2) TPS3D by setting z=0
//
// reference MDK_Document
// Thin-Plate Spline Transformation.doc and TPS3D.m
// "A physics based coordinate transform for 3D image match" at TMI 1997
// "Principal warps thin-plate splines and the decomposition"

// ScalarType is float or double
template<typename Scalar_Type>
class ThinPlateSplineTransform2D : public CoordinateTransform2D<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	// m_SourceLandmarkPointSet must be valid during EstimateParameter() and TransformPoint()
	const DenseMatrix<ScalarType>* m_SourceLandmarkPointSet; // 2 x N

	// m_TargetLandmarkPointSet must be valid during EstimateParameter()
	const DenseMatrix<ScalarType>* m_TargetLandmarkPointSet; // 2 x N
	
	DenseMatrix<ScalarType> m_Parameter;  // (N+4) x 2
	
	bool m_Flag_Use_TPS3D;
	// true: use TPS3D and set z=0
	// false: use standard TPS2D

public:
	ThinPlateSplineTransform2D();
	~ThinPlateSplineTransform2D();

	void Clear();

	void SetSourceLandmarkPointSet(const DenseMatrix<ScalarType>* SourceLandmarkPointSet);
	void SetTargetLandmarkPointSet(const DenseMatrix<ScalarType>* TargetLandmarkPointSet);
	void UseTPS3D();
	void UseStandardTPS2D();
	void EstimateParameter();

	bool Is_TPS3D_used() const;
	bool Is_StandardTPS2D_used() const;

	void SetParameter(const DenseMatrix<ScalarType>& Parameter);
	DenseMatrix<ScalarType> GetParameter() const;

	inline DenseVector<ScalarType, 2> TransformPoint(ScalarType x, ScalarType y) const;
	using CoordinateTransform2D::TransformPoint;

private:
	bool CheckLandmarkPointSet();

private:
	ThinPlateSplineTransform2D(const ThinPlateSplineTransform2D&) = delete;
	void operator=(const ThinPlateSplineTransform2D&) = delete;
};

}//namespace mdk

#include "mdkThinPlateSplineTransform2D.hpp"
