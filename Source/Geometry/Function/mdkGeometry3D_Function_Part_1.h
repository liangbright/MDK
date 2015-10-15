﻿#ifndef mdk_Geometry3D_Function_Part_1_h
#define mdk_Geometry3D_Function_Part_1_h

namespace mdk
{
// see http://mathworld.wolfram.com/Point-PlaneDistance.html
// PlaneNormal must have unit length
// Output Distance may be < 0

template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const DenseMatrix<ScalarType>& Point,
													 const DenseMatrix<ScalarType>& PlaneOrigin,
													 const DenseMatrix<ScalarType>& PlaneNormal);

template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const DenseVector<ScalarType, 3>& Point,
											         const DenseVector<ScalarType, 3>& PlaneOrigin,
											         const DenseVector<ScalarType, 3>& PlaneNormal);

template<typename ScalarType>
inline
ScalarType ComputeSignedDistanceBetweenPointAndPlane(const ScalarType* Point, const ScalarType* PlaneOrigin, const ScalarType* PlaneNormal);


template<typename ScalarType>
DenseMatrix<ScalarType> ProjectPointToPlane(const DenseVector<ScalarType, 3>& Point, 
											const DenseMatrix<ScalarType>& PlaneOrigin, 
											const DenseMatrix<ScalarType>& PlaneNormal);

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToPlane(const DenseVector<ScalarType, 3>& Point, 
											   const DenseVector<ScalarType, 3>& PlaneOrigin,
											   const DenseVector<ScalarType, 3>& PlaneNormal);

template<typename ScalarType>
DenseVector<ScalarType, 3> ProjectPointToPlane(const ScalarType* Point, const ScalarType* PlaneOrigin, const ScalarType* PlaneNormal);

template<typename ScalarType>
int_max FindNearestPointOnCurve(const DenseMatrix<ScalarType>& Curve, const DenseVector<ScalarType, 3>& Point_ref);

}// namespace mdk

#include "mdkGeometry3D_Function_Part_1.hpp"

#endif