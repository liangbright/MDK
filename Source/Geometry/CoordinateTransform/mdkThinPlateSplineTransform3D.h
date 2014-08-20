#ifndef __mdkThinPlateSplineTransform3D_h
#define __mdkThinPlateSplineTransform3D_h

#include "mdkDenseMatrix.h"

namespace mdk
{
// reference MDK_Document: Thin-Plate Spline Transformation.doc and TPS3D.m

// ScalarType is float or double
template<typename Scalar_Type>
class ThinPlateSplineTransform3D : public Object
{
public:
	typedef Scalar_Type ScalarType;

private:
	DenseMatrix<ScalarType> m_SourceControlPointSet; // 3 x N
	DenseMatrix<ScalarType> m_TargetControlPointSet; // 3 x N
	DenseMatrix<ScalarType> m_Parameter;  // (N+4) x 3

public:
	ThinPlateSplineTransform3D();
	~ThinPlateSplineTransform3D();

	void Clear();

	void SetSourceControlPointSet(const DenseMatrix<ScalarType>& SourcePointSet);
	DenseMatrix<ScalarType>* GetSourceControlPointSet();

	void SetTargetControlPointSet(const DenseMatrix<ScalarType>& TargetPointSet);
	DenseMatrix<ScalarType>* GetTargetControlPointSet();

	void SetParameter(const DenseMatrix<ScalarType>& Parameter);
	DenseMatrix<ScalarType> CopyParameter();

	void UpdateParameter();

	DenseVector<ScalarType, 3> TransformPoint(ScalarType x, ScalarType y, ScalarType z);

private:
	ThinPlateSplineTransform3D(const ThinPlateSplineTransform3D&) = delete;
	void operator=(const ThinPlateSplineTransform3D&) = delete;
};

}//namespace mdk

#include "mdkThinPlateSplineTransform3D.hpp"

#endif
