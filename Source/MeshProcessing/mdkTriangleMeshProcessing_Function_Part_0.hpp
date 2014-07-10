#ifndef __mdkTriangleMeshProcessing_Function_Part_0_hpp
#define __mdkTriangleMeshProcessing_Function_Part_0_hpp

namespace mdk
{

template<typename MeshAttributeType>
DenseVector<Handle_Of_Point_Of_MembraneMesh> TraceMeshBoundaryCurve(const TriangleMesh<MeshAttributeType>& TargetMesh,
                                                                    Handle_Of_Point_Of_MembraneMesh PointHandle_start)
{
    const PolygonMesh<MeshAttributeType>& TargetMesh_ref = TargetMesh;
    return TraceMeshBoundaryCurve(TargetMesh_ref, PointHandle_start);
}


template<typename MeshAttributeType>
Handle_Of_Point_Of_MembraneMesh FindNearestPointOnMesh(const TriangleMesh<MeshAttributeType>& TargetMesh, 
                                                       const DenseVector<typename MeshAttributeType::ScalarType, 3>& PointPosition)
{
    const PolygonMesh<MeshAttributeType>& TargetMesh_ref = TargetMesh;
    return FindNearestPointOnMesh(TargetMesh_ref, PointPosition);
}

}//namespace mdk


#endif