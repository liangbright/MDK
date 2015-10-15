#ifndef mdk_QuadMeshProcessing_Function_Part_0_h
#define mdk_QuadMeshProcessing_Function_Part_0_h

#include "mdkDenseMatrix.h"
#include "mdkPolygonMesh.h"

namespace mdk
{

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh);// divide once only

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh, int_max SubdivisionNumber);

}//namespace mdk

#include "mdkQuadMeshProcessing_Function_Part_0.hpp"

#endif