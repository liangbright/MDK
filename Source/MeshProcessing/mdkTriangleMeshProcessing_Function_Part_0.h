#ifndef __mdkTriangleMeshProcessing_Function_Part_0_h
#define __mdkTriangleMeshProcessing_Function_Part_0_h

#include "mdkDenseMatrix.h"
#include "mdkTriangleMesh.h"

namespace mdk
{

template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_MembraneMesh> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh,
                                                                    Handle_Of_Point_Of_MembraneMesh PointHandle_start);

template<typename MeshAttributeType>
DataArray<DenseVector<Handle_Of_Point_Of_MembraneMesh>> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh);

template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh FindNearestPointOnMesh(const TriangleMesh<MeshAttributeType>& TargetMesh, 
                                                       const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition);

}//namespace mdk

#include "mdkTriangleMeshProcessing_Function_Part_0.hpp"

#endif