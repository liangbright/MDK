#ifndef __mdkGeometry3D_Function_Part_0_h
#define __mdkGeometry3D_Function_Part_0_h

namespace mdk
{
// compute cross product: VectorA x VectorB, right hand coordinate system
// ref: http://mathworld.wolfram.com/CrossProduct.html
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeVectorCrossProductIn3D(const DenseMatrix<ScalarType>& VectorA, const DenseMatrix<ScalarType>& VectorB);

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeVectorCrossProductIn3D(const DenseVector<ScalarType, 3>& VectorA, const DenseVector<ScalarType, 3>& VectorB);

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeVectorCrossProductIn3D(const ScalarType* VectorA,  const ScalarType* VectorB);

template<typename ScalarType>
inline
ScalarType ComputeAngleBetweenTwoVectorIn3D(const DenseMatrix<ScalarType>& VectorA, const DenseMatrix<ScalarType>& VectorB);

template<typename ScalarType>
inline
ScalarType ComputeAngleBetweenTwoVectorIn3D(const DenseVector<ScalarType, 3>& VectorA, const DenseVector<ScalarType, 3>& VectorB);

template<typename ScalarType>
inline
ScalarType ComputeAngleBetweenTwoVectorIn3D(const ScalarType* VectorA, const ScalarType* VectorB);


// compute normal vector of triangle in 3D
// PointA -> PointB -> PointC : right hand coordinate system
// PointPosition: (x, y, z)
// ref: http://mathworld.wolfram.com/CrossProduct.html
template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeTriangleNormalIn3D(const DenseMatrix<ScalarType>& PointPositionA,
                                                      const DenseMatrix<ScalarType>& PointPositionB, 
                                                      const DenseMatrix<ScalarType>& PointPositionC);

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeTriangleNormalIn3D(const DenseVector<ScalarType, 3>& PointPositionA,
                                                      const DenseVector<ScalarType, 3>& PointPositionB, 
                                                      const DenseVector<ScalarType, 3>& PointPositionC);

template<typename ScalarType>
inline
DenseVector<ScalarType, 3> ComputeTriangleNormalIn3D(const ScalarType* PointPositionA,
                                                      const ScalarType* PointPositionB, 
                                                      const ScalarType* PointPositionC);

template<typename ScalarType>
inline
ScalarType ComputeTriangleAreaIn3D(const DenseMatrix<ScalarType>& PointPositionA,
                                    const DenseMatrix<ScalarType>& PointPositionB,
                                    const DenseMatrix<ScalarType>& PointPositionC);

template<typename ScalarType>
inline
ScalarType ComputeTriangleAreaIn3D(const DenseVector<ScalarType, 3>& PointPositionA,
                                    const DenseVector<ScalarType, 3>& PointPositionB,
                                    const DenseVector<ScalarType, 3>& PointPositionC);

template<typename ScalarType>
inline
ScalarType ComputeTriangleAreaIn3D(const ScalarType* PointPositionA, const ScalarType* PointPositionB, const ScalarType* PointPositionC);


}// namespace mdk

#include "mdkGeometry3D_Function_Part_0.hpp"

#endif