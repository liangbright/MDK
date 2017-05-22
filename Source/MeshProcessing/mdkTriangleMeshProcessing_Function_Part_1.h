#pragma once

namespace mdk
{
template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> ResampleMeshOpenBoundary(const TriangleMesh<MeshAttributeType>& InputMesh, const DenseVector<int_max>& BounaryPointIndexList,
	                                                     const DenseMatrix<typename MeshAttributeType::ScalarType>& Boundary_output);


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> ResampleMeshOpenBoundary(const TriangleMesh<MeshAttributeType>& InputMesh, const DenseVector<int_max>& BounaryPointIndexList, 
	                                                     const int_max PointCountOfBounary_output);
}//namespace mdk

#include "mdkTriangleMeshProcessing_Function_Part_1.hpp"
