#pragma once

namespace mdk
{
// compute cross product: VectorA x VectorB, right hand coordinate system
// ref: http://mathworld.wolfram.com/CrossProduct.html
template<typename ScalarType>
inline DenseVector<ScalarType, 3> ComputeVectorCrossProductIn3D(const DenseMatrix<ScalarType>& VectorA, const DenseMatrix<ScalarType>& VectorB);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ComputeVectorCrossProductIn3D(const DenseVector<ScalarType, 3>& VectorA, const DenseVector<ScalarType, 3>& VectorB);

template<typename ScalarType>
inline DenseVector<ScalarType, 3> ComputeVectorCrossProductIn3D(const ScalarType* VectorA,  const ScalarType* VectorB);

template<typename ScalarType>
inline ScalarType ComputeVectorDotProductIn3D(const DenseMatrix<ScalarType>& VectorA, const DenseMatrix<ScalarType>& VectorB);

template<typename ScalarType>
inline ScalarType ComputeVectorDotProductIn3D(const DenseVector<ScalarType, 3>& VectorA, const DenseVector<ScalarType, 3>& VectorB);

template<typename ScalarType>
inline ScalarType ComputeVectorDotProductIn3D(const ScalarType* VectorA, const ScalarType* VectorB);

template<typename ScalarType>
inline ScalarType ComputeAngleBetweenTwoVectorIn3D(const DenseMatrix<ScalarType>& VectorA, const DenseMatrix<ScalarType>& VectorB);

template<typename ScalarType>
inline ScalarType ComputeAngleBetweenTwoVectorIn3D(const DenseVector<ScalarType, 3>& VectorA, const DenseVector<ScalarType, 3>& VectorB);

template<typename ScalarType>
inline ScalarType ComputeAngleBetweenTwoVectorIn3D(const ScalarType* VectorA, const ScalarType* VectorB);


// compute normal vector of triangle in 3D
// PointA -> PointB -> PointC : right hand coordinate system
// PointPosition: (x, y, z)
// ref: http://mathworld.wolfram.com/CrossProduct.html
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeTriangleNormalIn3D(const DenseMatrix<ScalarType>& PointA, const DenseMatrix<ScalarType>& PointB, const DenseMatrix<ScalarType>& PointC);

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeTriangleNormalIn3D(const DenseVector<ScalarType, 3>& PointA, const DenseVector<ScalarType, 3>& PointB, const DenseVector<ScalarType, 3>& PointC);

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeTriangleNormalIn3D(const ScalarType* PointA, const ScalarType* PointB, const ScalarType* PointC);

template<typename ScalarType>
inline
ScalarType ComputeTriangleAreaIn3D(const DenseMatrix<ScalarType>& PointA, const DenseMatrix<ScalarType>& PointB, const DenseMatrix<ScalarType>& PointC);

template<typename ScalarType>
inline
ScalarType ComputeTriangleAreaIn3D(const DenseVector<ScalarType, 3>& PointA, const DenseVector<ScalarType, 3>& PointB, const DenseVector<ScalarType, 3>& PointC);

template<typename ScalarType>
inline
ScalarType ComputeTriangleAreaIn3D(const ScalarType* PointA, const ScalarType* PointB, const ScalarType* PointC);

template<typename ScalarType>
DenseVector<ScalarType, 3> ComputeCenterOfCircumcircleOfTriangleIn3D(const DenseMatrix<ScalarType>& PointA, const DenseMatrix<ScalarType>& PointB, const DenseMatrix<ScalarType>& PointC);

template<typename ScalarType>
DenseVector<ScalarType, 3> ComputeCenterOfCircumcircleOfTriangleIn3D(const DenseVector<ScalarType, 3>& PointA, const DenseVector<ScalarType, 3>& PointB, const DenseVector<ScalarType, 3>& PointC);

template<typename ScalarType>
DenseVector<ScalarType, 3> ComputeCenterOfCircumcircleOfTriangleIn3D(const ScalarType* PointA, const ScalarType* PointB, const ScalarType* PointC);

}// namespace mdk

#include "mdkGeometry3D_Function_Part_1.hpp"
