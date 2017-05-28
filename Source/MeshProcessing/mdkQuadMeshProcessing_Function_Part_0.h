#pragma once

namespace mdk
{

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh);// divide once only

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh, int_max SubdivisionCount);

template<typename MeshAttributeTypeA, typename MeshAttributeTypeB>
void ConvertQuadMeshToTriangleMesh(const PolygonMesh<MeshAttributeTypeA>& InputMesh, TriangleMesh<MeshAttributeTypeB>& OutputMesh, const std::string& Method = "1to2");

//Input: Quad or Mixed-Quad-Triangle
template<typename MeshAttributeTypeA, typename MeshAttributeTypeB>
void ConvertQuadMeshToTriangleMesh_1to2(const PolygonMesh<MeshAttributeTypeA>& InputMesh, TriangleMesh<MeshAttributeTypeB>& OutputMesh);

//Input: Quad or Mixed-Quad-Triangle
template<typename MeshAttributeTypeA, typename MeshAttributeTypeB>
void ConvertQuadMeshToTriangleMesh_1to8(const PolygonMesh<MeshAttributeTypeA>& InputMesh, TriangleMesh<MeshAttributeTypeB>& OutputMesh);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh);

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh, int_max SubdivisionCount);

template<typename MeshAttributeType = PolygonMeshStandardAttributeType<double>>
PolygonMesh<MeshAttributeType> CreateQuadMeshOfRectangularFlatSurface(int_max PointCount_x, int_max PointCount_y, double Spacing_x=1.0, double Spacing_y=1.0);

template<typename MeshAttributeType = PolygonMeshStandardAttributeType<double>>
PolygonMesh<MeshAttributeType> CreateQuadMeshOfCylinderSurface(int_max PointCountPerRing, int_max RingCount, double Radius, double Height);

}//namespace mdk

#include "mdkQuadMeshProcessing_Function_Part_0.hpp"
