#pragma once

#include <vtkSmartPointer.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include <vtkCardinalSpline.h>
#include <vtkSplineFilter.h>


namespace mdk
{
// see http://mathworld.wolfram.com/Point-PlaneDistance.html
// PlaneNormal must have unit length
// Output Distance may be < 0

template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const DenseMatrix<ScalarType>& Point, const DenseMatrix<ScalarType>& PlaneOrigin, const DenseMatrix<ScalarType>& PlaneNormal);

template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const DenseVector<ScalarType, 3>& Point, const DenseVector<ScalarType, 3>& PlaneOrigin, const DenseVector<ScalarType, 3>& PlaneNormal);

template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const ScalarType* Point, const ScalarType* PlaneOrigin, const ScalarType* PlaneNormal);


template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToPlane(const DenseVector<ScalarType, 3>& Point, const DenseMatrix<ScalarType>& PlaneOrigin, const DenseMatrix<ScalarType>& PlaneNormal);

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToPlane(const DenseVector<ScalarType, 3>& Point, const DenseVector<ScalarType, 3>& PlaneOrigin, const DenseVector<ScalarType, 3>& PlaneNormal);

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToPlane(const ScalarType* Point, const ScalarType* PlaneOrigin, const ScalarType* PlaneNormal);

//find index of the nearest point on curve, no curve-interpolation
template<typename ScalarType>
DenseVector<int_max> FindNearestPointOnCurve(const DenseMatrix<ScalarType>& Curve, const DenseMatrix<ScalarType>& PointSet_ref);
template<typename ScalarType>
int_max FindNearestPointOnCurve(const DenseMatrix<ScalarType>& Curve, const DenseVector<ScalarType, 3>& Point_ref);
template<typename ScalarType>
int_max FindNearestPointOnCurve(const DenseMatrix<ScalarType>& Curve, ScalarType x_ref, ScalarType y_ref, ScalarType z_ref);

template<typename ScalarType>
ScalarType Compute3DCurveLength(const DenseMatrix<ScalarType>& Curve);

template<typename ScalarType>
DenseVector<ScalarType> ComputeCumulative3DCurveLengthList(const DenseMatrix<ScalarType>& Curve);

//--------------  resample curve -------------------------------------------------------------------------------

template<typename ScalarType>
DenseMatrix<ScalarType> Subdivide3DCurve_Linear(const DenseMatrix<ScalarType>& Curve, int_max SubdivisionCount = 1);

template<typename ScalarType>
DenseMatrix<ScalarType> ResampleOpen3DCurveByCardinalSpline(const DenseMatrix<ScalarType>& Curve, int_max PointCount);

template<typename ScalarType>
DenseMatrix<ScalarType> ResampleClosed3DCurveByCardinalSpline(const DenseMatrix<ScalarType>& Curve, int_max PointCount);

}// namespace mdk

#include "mdkGeometry3D_Function_Part_0.hpp"
