#ifndef __mdkGeometry3D_Function_Part_0_h
#define __mdkGeometry3D_Function_Part_0_h

namespace mdk
{
// compute cross product: VectorA x VectorB, right hand coordinate system
// ref: http://mathworld.wolfram.com/CrossProduct.html
template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeVectorCrossProductIn3D(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeVectorCrossProductIn3D(const DenseVector<ElementType, 3>& VectorA, const DenseVector<ElementType, 3>& VectorB);

template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeVectorCrossProductIn3D(const ElementType* VectorA,  const ElementType* VectorB);

template<typename ElementType>
inline
ElementType ComputeAngleBetweenTwoVectorIn3D(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB);

template<typename ElementType>
inline
ElementType ComputeAngleBetweenTwoVectorIn3D(const DenseVector<ElementType, 3>& VectorA, const DenseVector<ElementType, 3>& VectorB);

template<typename ElementType>
inline
ElementType ComputeAngleBetweenTwoVectorIn3D(const ElementType* VectorA, const ElementType* VectorB);


// compute normal vector of triangle in 3D
// PointA -> PointB -> PointC : right hand coordinate system
// PointPosition: (x, y, z)
// ref: http://mathworld.wolfram.com/CrossProduct.html
template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeTriangleNormalIn3D(const DenseMatrix<ElementType>& PointPositionA,
                                                      const DenseMatrix<ElementType>& PointPositionB, 
                                                      const DenseMatrix<ElementType>& PointPositionC);

template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeTriangleNormalIn3D(const DenseVector<ElementType, 3>& PointPositionA,
                                                      const DenseVector<ElementType, 3>& PointPositionB, 
                                                      const DenseVector<ElementType, 3>& PointPositionC);

template<typename ElementType>
inline
DenseVector<ElementType, 3> ComputeTriangleNormalIn3D(const ElementType* PointPositionA,
                                                      const ElementType* PointPositionB, 
                                                      const ElementType* PointPositionC);

template<typename ElementType>
inline
ElementType ComputeTriangleAreaIn3D(const DenseMatrix<ElementType>& PointPositionA,
                                    const DenseMatrix<ElementType>& PointPositionB,
                                    const DenseMatrix<ElementType>& PointPositionC);

template<typename ElementType>
inline
ElementType ComputeTriangleAreaIn3D(const DenseVector<ElementType, 3>& PointPositionA,
                                    const DenseVector<ElementType, 3>& PointPositionB,
                                    const DenseVector<ElementType, 3>& PointPositionC);

template<typename ElementType>
inline
ElementType ComputeTriangleAreaIn3D(const ElementType* PointPositionA, const ElementType* PointPositionB, const ElementType* PointPositionC);


}// namespace mdk

#include "mdkGeometry3D_Function_Part_0.hpp"

#endif