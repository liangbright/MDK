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
int_max FindNearestPointOnCurve(const DenseMatrix<ScalarType>& Curve, const DenseVector<ScalarType, 3>& Point_ref);

template<typename ScalarType>
ScalarType Compute3DCurveLength(const DenseMatrix<ScalarType>& Curve);

template<typename ScalarType>
DenseVector<ScalarType> ComputeCumulative3DCurveLengthList(const DenseMatrix<ScalarType>& Curve);

template<typename ScalarType>
DenseMatrix<ScalarType> Subdivide3DCurve_Linear(const DenseMatrix<ScalarType>& Curve, int_max SubdivisionNumber = 1);

template<typename ScalarType>
DenseMatrix<ScalarType> Resample3DCurveWithBoundedSegmentLength(const DenseMatrix<ScalarType>& Curve, ScalarType MaxSegmentLength);

template<typename ScalarType>
DenseMatrix<ScalarType> Resample3DCurveWithEqualSegmentLength(const DenseMatrix<ScalarType>& Curve, int_max PointCount_resampled, int_max MaxIterCount = 0, double Tolerance = 0.01);

template<typename ScalarType>
DenseMatrix<ScalarType> Resample3DCurve(const DenseMatrix<ScalarType>& Curve, const DenseVector<ScalarType>& SegmentRelativeLengthList_resampled, int_max MaxIterCount = 10, double Tolerance = 0.01);

template<typename ScalarType>
DenseMatrix<ScalarType> FitSplineToOpenCurve(const DenseMatrix<ScalarType>& Curve, int_max PointCount);

template<typename ScalarType>
DenseMatrix<ScalarType> FitSplineToClosedCurve(const DenseMatrix<ScalarType>& Curve, int_max PointCount);

}// namespace mdk

#include "mdkGeometry3D_Function_Part_0.hpp"
