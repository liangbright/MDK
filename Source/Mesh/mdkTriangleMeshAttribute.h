#ifndef __mdkTriangleMeshAttribute_h
#define __mdkTriangleMeshAttribute_h

#include "mdkPolygonMeshAttribute.h"

namespace mdk
{
//============================================== GlobalAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshGlobalAttributeTypeEnum
{
    MeanCellArea,
    MeanEdgePhysicalLength
};

template<typename ScalarType>
struct GlobalAttribute_Of_TriangleMesh : public GlobalAttribute_Of_PolygonMesh<ScalarType>
{

//--------------------------------------------------------------
    GlobalAttribute_Of_TriangleMesh() { this->Clear(); }
    GlobalAttribute_Of_TriangleMesh(const GlobalAttribute_Of_TriangleMesh& InputAttribute) { (*this) = InputAttribute; }
    ~GlobalAttribute_Of_TriangleMesh() {}

    void operator=(const GlobalAttribute_Of_TriangleMesh& InputAttribute)
    {
        this->GlobalAttribute_Of_PolygonMesh::operator=(InputAttribute);
    }

    void Clear()
    {
        this->GlobalAttribute_Of_PolygonMesh::Clear();
        MeanCellArea = 0;
        MeanEdgePhysicalLength = 0;
    }
};

//============================================== PointAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshPointAttributeTypeEnum
{
    MeanCurvature,
    Normal
};

template<typename ScalarType>
struct PointAttribute_Of_TriangleMesh : PointAttribute_Of_PolygonMesh<ScalarType>
{

//-----------------------------------------------
    PointAttribute_Of_TriangleMesh() { this->Clear(); }
    PointAttribute_Of_TriangleMesh(const PointAttribute_Of_TriangleMesh& InputAttribute) { (*this) = InputAttribute; }
    ~PointAttribute_Of_TriangleMesh() {}

    void operator=(const PointAttribute_Of_TriangleMesh& InputAttribute)
    {
        this->PointAttribute_Of_PolygonMesh::operator=(InputAttribute);
    }

    void Clear()
    {
        this->PointAttribute_Of_TriangleMesh::Clear();
    }
};

//============================================== EdgeAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshEdgeAttributeEnum
{
    PhysicalLength,
};


template<typename ScalarType>
struct EdgeAttribute_Of_TriangleMesh : public EdgeAttribute_Of_PolygonMesh<ScalarType>
{
//----------------------------------------------------------------------
    EdgeAttribute_Of_TriangleMesh() { this->Clear(); }
    EdgeAttribute_Of_TriangleMesh(const EdgeAttribute_Of_TriangleMesh& InputAttribute) { (*this) = InputAttribute; }
    ~EdgeAttribute_Of_TriangleMesh() {}

    void operator=(const EdgeAttribute_Of_TriangleMesh& InputAttribute)
    {
        this->EdgeAttribute_Of_PolygonMesh::operator=(InputAttribute);
    }

    void Clear()
    {
        this->EdgeAttribute_Of_PolygonMesh::Clear();
    }
};

//============================================== DirectedEdgeAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshDirectedEdgeAttributeEnum
{
    Orientation,
};

template<typename ScalarType>
struct DirectedEdgeAttribute_Of_TriangleMesh : public DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>
{
    DenseVector<ScalarType, 3> Orientation;
//-------------------------------------------------------
    DirectedEdgeAttribute_Of_TriangleMesh() { this->Clear(); }
    DirectedEdgeAttribute_Of_TriangleMesh(const DirectedEdgeAttribute_Of_TriangleMesh& InputAttribute) { (*this) = InputAttribute; }
    ~DirectedEdgeAttribute_Of_TriangleMesh() {}

    void operator=(const DirectedEdgeAttribute_Of_TriangleMesh& InputAttribute)
    {
        this->DirectedEdgeAttribute_Of_PolygonMesh::operator=(InputAttribute);
    }

    void Clear()
    {
        this->DirectedEdgeAttribute_Of_PolygonMesh::Clear();
    }

};

//============================================== CellAttribute_Of_TriangleMesh ===========================================//
enum class TriangleMeshCellAttributeEnum
{
    Area,
};

template<typename ScalarType>
struct CellAttribute_Of_TriangleMesh : public CellAttribute_Of_PolygonMesh<ScalarType>
{
    ScalarType Area;

//---------------------------------------
    CellAttribute_Of_TriangleMesh() { this->Clear(); }
    CellAttribute_Of_TriangleMesh(const CellAttribute_Of_TriangleMesh& InputAttribute) { (*this) = InputAttribute; }
    ~CellAttribute_Of_TriangleMesh() {}

    void operator=(const CellAttribute_Of_TriangleMesh& InputAttribute)
    {
        this->CellAttribute_Of_PolygonMesh::operator=(InputAttribute);
    }

    void Clear()
    {
        this->CellAttribute_Of_PolygonMesh::Clear();
    }
};

}// namespace mdk


#endif