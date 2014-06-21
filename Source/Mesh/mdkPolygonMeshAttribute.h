#ifndef __mdkPolygonMeshAttribute_h
#define __mdkPolygonMeshAttribute_h


#include "mdkDenseMatrix.h"

namespace mdk
{

//----------------- forward declare ------------------//

template<typename ScalarType>
class PolygonMesh;

template<typename ScalarType>
class Point_Of_PolygonMesh;

template<typename ScalarType>
class Edge_Of_PolygonMesh;

template<typename ScalarType>
class DirectedEdge_Of_PolygonMesh;

template<typename ScalarType>
class Cell_Of_PolygonMesh;
//---------------------------------------------------//

//============================================== PointAttributeType_Of_PolygonMesh ===========================================//
enum class PolygonMeshPointAttributeTypeEnum
{
    MeanCurvature,
    Normal
};

template<typename ScalarType>
struct Data_Of_PointAttributeType_Of_PolygonMesh
{
    ScalarType MeanCurvature;
    DenseVector<ScalarType, 3> Normal;
};

template<typename ScalarType>
class PointAttributeType_Of_PolygonMesh
{
private:
    std::unique_ptr<Data_Of_PointAttributeType_Of_PolygonMesh<ScalarType>> m_Data;
public:
    PointAttributeType_Of_PolygonMesh();
    PointAttributeType_Of_PolygonMesh(const PointAttributeType_Of_PolygonMesh& InputAttribute);
    PointAttributeType_Of_PolygonMesh(PointAttributeType_Of_PolygonMesh&& InputAttribute);
    ~PointAttributeType_Of_PolygonMesh();

    void operator=(const PointAttributeType_Of_PolygonMesh& InputAttribute);
    void operator=(PointAttributeType_Of_PolygonMesh&& InputAttribute);

    void Clear();
};

//============================================== EdgeAttribute_Of_PolygonMesh ===========================================//
enum class PolygonMeshEdgeAttributeEnum
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

    void Clear();
};

//============================================== DirectedEdgeAttribute_Of_PolygonMesh ===========================================//
enum class PolygonMeshDirectedEdgeAttributeEnum
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

    void Clear();
};

//============================================== CellAttribute_Of_PolygonMesh ===========================================//
enum class PolygonMeshCellAttributeEnum
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

    void Clear();
};

}// namespace mdk

#include "mdkPolygonMeshAttribute.hpp"

#endif