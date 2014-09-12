#ifndef __mdkPolygonMeshProcessing_Function_Part_0_h
#define __mdkPolygonMeshProcessing_Function_Part_0_h

#include "mdkDenseMatrix.h"
#include "mdkPolygonMesh.h"

namespace mdk
{

template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_MembraneMesh> TraceMeshBoundaryCurve(const PolygonMesh<MeshAttributeType>& TargetMesh,
                                                                    Handle_Of_Point_Of_MembraneMesh PointHandle_start);

template<typename MeshAttributeType>
ObjectArray<DenseVector<Handle_Of_Point_Of_MembraneMesh>> TraceMeshBoundaryCurve(const PolygonMesh<MeshAttributeType>& TargetMesh);

template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh FindNearestPointOnMesh(const PolygonMesh<MeshAttributeType>& TargetMesh,
                                                       const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition);

}//namespace mdk

#include "mdkPolygonMeshProcessing_Function_Part_0.hpp"

#endif