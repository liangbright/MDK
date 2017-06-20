#pragma once

namespace mdk
{

template<typename ScalarType>
PolygonMesh<ScalarType> SubdivideQuadMesh_Linear(const PolygonMesh<ScalarType>& InputMesh);// divide once only

template<typename ScalarType>
PolygonMesh<ScalarType> SubdivideQuadMesh_Linear(const PolygonMesh<ScalarType>& InputMesh, int_max SubdivisionCount);

template<typename ScalarType>
TriangleMesh<ScalarType> ConvertQuadMeshToTriangleMesh(const PolygonMesh<ScalarType>& InputMesh, const std::string& Method = "1to2");

//Input: Quad or Mixed-Quad-Triangle
template<typename ScalarType>
TriangleMesh<ScalarType> ConvertQuadMeshToTriangleMesh_1to2(const PolygonMesh<ScalarType>& InputMesh);

//Input: Quad or Mixed-Quad-Triangle
template<typename ScalarType>
TriangleMesh<ScalarType> ConvertQuadMeshToTriangleMesh_1to8(const PolygonMesh<ScalarType>& InputMesh);

template<typename ScalarType>
PolygonMesh<ScalarType> SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(const PolygonMesh<ScalarType>& InputMesh);

template<typename ScalarType>
PolygonMesh<ScalarType> SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(const PolygonMesh<ScalarType>& InputMesh, int_max SubdivisionCount);

template<typename ScalarType = double>
PolygonMesh<ScalarType> CreateQuadMeshOfRectangularFlatSurface(int_max PointCount_x, int_max PointCount_y, double Spacing_x=1.0, double Spacing_y=1.0);

template<typename ScalarType = double>
PolygonMesh<ScalarType> CreateQuadMeshOfCylinderSurface(int_max PointCountPerRing, int_max RingCount, double Radius, double Height);

}//namespace mdk

#include "mdkQuadMeshProcessing_Function_Part_0.hpp"
