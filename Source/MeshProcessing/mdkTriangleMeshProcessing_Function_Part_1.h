#pragma once

namespace mdk
{
template<typename ScalarType>
TriangleMesh<ScalarType> ResampleMeshOpenBoundary(const TriangleMesh<ScalarType>& InputMesh, const DenseVector<int_max>& BounaryPointIndexList,
	                                              const DenseMatrix<ScalarType>& Boundary_output);


template<typename ScalarType>
TriangleMesh<ScalarType> ResampleMeshOpenBoundary(const TriangleMesh<ScalarType>& InputMesh, const DenseVector<int_max>& BounaryPointIndexList,
	                                              const int_max PointCountOfBounary_output);
}//namespace mdk

#include "mdkTriangleMeshProcessing_Function_Part_1.hpp"
