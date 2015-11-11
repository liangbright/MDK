#ifndef mdk_QuadMeshProcessing_Function_Part_0_h
#define mdk_QuadMeshProcessing_Function_Part_0_h

#include "mdkDenseMatrix.h"
#include "mdkPolygonMesh.h"
#include "mdkTriangleMesh.h"

namespace mdk
{

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh);// divide once only

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh, int_max SubdivisionNumber);

template<typename MeshAttributeTypeA, typename MeshAttributeTypeB>
void ConvertQuadMeshToTriangleMesh(const PolygonMesh<MeshAttributeTypeA>& InputMesh, TriangleMesh<MeshAttributeTypeB>& OutputMesh);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh);

}//namespace mdk

#include "mdkQuadMeshProcessing_Function_Part_0.hpp"

#endif