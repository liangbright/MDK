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

template<typename MeshAttributeType = PolygonMeshStandardAttributeType<double>>
PolygonMesh<MeshAttributeType> CreateQuadMeshOfRectangularFlatSurface(int_max PointCount_x, int_max PointCount_y, double Spacing_x=1.0, double Spacing_y=1.0);

template<typename MeshAttributeType = PolygonMeshStandardAttributeType<double>>
PolygonMesh<MeshAttributeType> CreateQuadMeshOfCylinderSurface(int_max PointCountPerRing, int_max RingCount, double Radius, double Height);

}//namespace mdk

#include "mdkQuadMeshProcessing_Function_Part_0.hpp"

#endif