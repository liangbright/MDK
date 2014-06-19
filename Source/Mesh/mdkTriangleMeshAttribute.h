#ifndef __mdkTriangleMeshAttribute_h
#define __mdkTriangleMeshAttribute_h


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

//============================================== VertexAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshVertexAttributeEnum
{
    MeanCurvature,
    Normal
};

template<typename ScalarType>
struct Data_Of_VertexAttribute_Of_TriangleMesh
{
    ScalarType MeanCurvature;
    DenseVector<ScalarType, 3> Normal;
};

template<typename ScalarType>
class VertexAttribute_Of_TriangleMesh
{
private:
    std::unique_ptr<Data_Of_VertexAttribute_Of_TriangleMesh<ScalarType>> m_Data;
public:
    VertexAttribute_Of_TriangleMesh();
    VertexAttribute_Of_TriangleMesh(const VertexAttribute_Of_TriangleMesh& InputAttribute);
    VertexAttribute_Of_TriangleMesh(VertexAttribute_Of_TriangleMesh&& InputAttribute);
    ~VertexAttribute_Of_TriangleMesh();

    void operator=(const VertexAttribute_Of_TriangleMesh& InputAttribute);
    void operator=(VertexAttribute_Of_TriangleMesh&& InputAttribute);

};

//============================================== EdgeAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshEdgeAttributeEnum
{
    PhysicalLength,
};

template<typename ScalarType>
struct Data_Of_EdgeAttribute_Of_TriangleMesh
{
    ScalarType PhysicalLength;
};

template<typename ScalarType>
class EdgeAttribute_Of_TriangleMesh
{
private:
    std::unique_ptr<Data_Of_EdgeAttribute_Of_TriangleMesh<ScalarType>> m_Data;
public:
    EdgeAttribute_Of_TriangleMesh();
    EdgeAttribute_Of_TriangleMesh(const EdgeAttribute_Of_TriangleMesh& InputAttribute);
    EdgeAttribute_Of_TriangleMesh(EdgeAttribute_Of_TriangleMesh&& InputAttribute);
    ~EdgeAttribute_Of_TriangleMesh();

    void operator=(const EdgeAttribute_Of_TriangleMesh& InputAttribute);
    void operator=(EdgeAttribute_Of_TriangleMesh&& InputAttribute);
};

//============================================== DirectedEdgeAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshDirectedEdgeAttributeEnum
{
    Orientation,
};

template<typename ScalarType>
struct Data_Of_DirectedEdgeAttribute_Of_TriangleMesh
{
    DenseVector<ScalarType, 3> Orientation;
};

template<typename ScalarType>
class DirectedEdgeAttribute_Of_TriangleMesh
{
private:
    std::unique_ptr<Data_Of_DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>> m_Data;
public:
    DirectedEdgeAttribute_Of_TriangleMesh();
    DirectedEdgeAttribute_Of_TriangleMesh(const DirectedEdgeAttribute_Of_TriangleMesh& InputAttribute);
    DirectedEdgeAttribute_Of_TriangleMesh(DirectedEdgeAttribute_Of_TriangleMesh&& InputAttribute);
    ~DirectedEdgeAttribute_Of_TriangleMesh();

    void operator=(const DirectedEdgeAttribute_Of_TriangleMesh& InputAttribute);
    void operator=(DirectedEdgeAttribute_Of_TriangleMesh&& InputAttribute);
};

//============================================== CellAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshCellAttributeEnum
{
    Area,
};

template<typename ScalarType>
struct Data_Of_CellAttribute_Of_TriangleMesh
{
    ScalarType Area;
};

template<typename ScalarType>
class CellAttribute_Of_TriangleMesh
{
private:
    std::unique_ptr<Data_Of_CellAttribute_Of_TriangleMesh<ScalarType>> m_Data;
public:
    CellAttribute_Of_TriangleMesh();
    CellAttribute_Of_TriangleMesh(const CellAttribute_Of_TriangleMesh& InputAttribute);
    CellAttribute_Of_TriangleMesh(CellAttribute_Of_TriangleMesh&& InputAttribute);
    ~CellAttribute_Of_TriangleMesh();

    void operator=(const CellAttribute_Of_TriangleMesh& InputAttribute);
    void operator=(CellAttribute_Of_TriangleMesh&& InputAttribute);
};

}// namespace mdk

#include "mdkTriangleMeshAttribute.hpp"

#endif