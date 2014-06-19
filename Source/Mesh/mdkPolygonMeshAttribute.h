#ifndef __mdkmdkPolygonMeshAttribute_h
#define __mdkmdkPolygonMeshAttribute_h


#include "mdkDenseMatrix.h"

namespace mdk
{

//----------------- forward declare ------------------//

template<typename ScalarType>
class PolygonMesh;

template<typename ScalarType>
struct Point_Of_PolygonMesh;

template<typename ScalarType>
class Vertex_Of_PolygonMesh;

template<typename ScalarType>
class Edge_Of_PolygonMesh;

template<typename ScalarType>
class DirectedEdge_Of_PolygonMesh;

template<typename ScalarType>
class Cell_Of_PolygonMesh;
//---------------------------------------------------//

//============================================== VertexAttribute_Of_PolygonMesh ===========================================//
enum class VertexAttributeEnum
{
    MeanCurvature,
    Normal
};

template<typename ScalarType>
struct Data_Of_VertexAttribute_Of_PolygonMesh
{
    ScalarType MeanCurvature;
    DenseVector<ScalarType, 3> Normal;
};

template<typename ScalarType>
class VertexAttribute_Of_PolygonMesh
{
private:
    std::unique_ptr<Data_Of_VertexAttribute_Of_PolygonMesh<ScalarType>> m_Data;
public:
    VertexAttribute_Of_PolygonMesh();
    VertexAttribute_Of_PolygonMesh(const VertexAttribute_Of_PolygonMesh& InputAttribute);
    VertexAttribute_Of_PolygonMesh(VertexAttribute_Of_PolygonMesh&& InputAttribute);
    ~VertexAttribute_Of_PolygonMesh();

    void operator=(const VertexAttribute_Of_PolygonMesh& InputAttribute);
    void operator=(VertexAttribute_Of_PolygonMesh&& InputAttribute);

};

//============================================== EdgeAttribute_Of_PolygonMesh ===========================================//
enum class EdgeAttributeEnum
{
    PhysicalLength,
};

template<typename ScalarType>
struct Data_Of_EdgeAttribute_Of_PolygonMesh
{
    ScalarType PhysicalLength;
};

template<typename ScalarType>
class EdgeAttribute_Of_PolygonMesh
{
private:
    std::unique_ptr<Data_Of_EdgeAttribute_Of_PolygonMesh<ScalarType>> m_Data;
public:
    EdgeAttribute_Of_PolygonMesh();
    EdgeAttribute_Of_PolygonMesh(const EdgeAttribute_Of_PolygonMesh& InputAttribute);
    EdgeAttribute_Of_PolygonMesh(EdgeAttribute_Of_PolygonMesh&& InputAttribute);
    ~EdgeAttribute_Of_PolygonMesh();

    void operator=(const EdgeAttribute_Of_PolygonMesh& InputAttribute);
    void operator=(EdgeAttribute_Of_PolygonMesh&& InputAttribute);
};

//============================================== DirectedEdgeAttribute_Of_PolygonMesh ===========================================//
enum class DirectedEdgeAttributeEnum
{
    Orientation,
};

template<typename ScalarType>
struct Data_Of_DirectedEdgeAttribute_Of_PolygonMesh
{
    DenseVector<ScalarType, 3> Orientation;
};

template<typename ScalarType>
class DirectedEdgeAttribute_Of_PolygonMesh
{
private:
    std::unique_ptr<Data_Of_DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>> m_Data;
public:
    DirectedEdgeAttribute_Of_PolygonMesh();
    DirectedEdgeAttribute_Of_PolygonMesh(const DirectedEdgeAttribute_Of_PolygonMesh& InputAttribute);
    DirectedEdgeAttribute_Of_PolygonMesh(DirectedEdgeAttribute_Of_PolygonMesh&& InputAttribute);
    ~DirectedEdgeAttribute_Of_PolygonMesh();

    void operator=(const DirectedEdgeAttribute_Of_PolygonMesh& InputAttribute);
    void operator=(DirectedEdgeAttribute_Of_PolygonMesh&& InputAttribute);
};

//============================================== CellAttribute_Of_PolygonMesh ===========================================//
enum class CellAttributeEnum
{
    Area,
};

template<typename ScalarType>
struct Data_Of_CellAttribute_Of_PolygonMesh
{
    ScalarType Area;
};

template<typename ScalarType>
class CellAttribute_Of_PolygonMesh
{
private:
    std::unique_ptr<Data_Of_CellAttribute_Of_PolygonMesh<ScalarType>> m_Data;
public:
    CellAttribute_Of_PolygonMesh();
    CellAttribute_Of_PolygonMesh(const CellAttribute_Of_PolygonMesh& InputAttribute);
    CellAttribute_Of_PolygonMesh(CellAttribute_Of_PolygonMesh&& InputAttribute);
    ~CellAttribute_Of_PolygonMesh();

    void operator=(const CellAttribute_Of_PolygonMesh& InputAttribute);
    void operator=(CellAttribute_Of_PolygonMesh&& InputAttribute);
};

}// namespace mdk

#include "mdkPolygonMeshAttribute.hpp"

#endif