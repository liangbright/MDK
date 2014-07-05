#ifndef __mdkPolygonMeshAttribute_h
#define __mdkPolygonMeshAttribute_h

#include "mdkDenseMatrix.h"
#include "mdkSurfaceMeshAttribute.h"

namespace mdk
{

//============================================== GlobalAttribute_Of_PolygonMesh ===========================================//
enum class PolygonMeshGlobalAttributeTypeEnum
{
    MeanCellArea,
    MeanEdgePhysicalLength
};

template<typename ScalarType>
struct GlobalAttribute_Of_PolygonMesh : GlobalAttribute_Of_SurfaceMesh<ScalarType>
{
    ScalarType MeanCellArea;
    ScalarType MeanEdgePhysicalLength;

//--------------------------------------------------------------
    GlobalAttribute_Of_PolygonMesh() { this->Clear(); }
    GlobalAttribute_Of_PolygonMesh(const GlobalAttribute_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
    ~GlobalAttribute_Of_PolygonMesh() {}

    void operator=(const GlobalAttribute_Of_PolygonMesh& InputAttribute)
    {
        MeanCellArea = InputAttribute.MeanCellArea;
        MeanEdgePhysicalLength = InputAttribute.MeanCellArea;
    }

    void Clear()
    {
        MeanCellArea = 0;
        MeanEdgePhysicalLength = 0;
    }
};

//============================================== PointAttribute_Of_PolygonMesh ===========================================//
enum class PolygonMeshPointAttributeTypeEnum
{
    GaussianCurvature,
    UnweightedGaussianCurvature,
    MeanCurvature,
    MeanCurvatureNormal,
    Normal
};

template<typename ScalarType>
struct PointAttribute_Of_PolygonMesh : PointAttribute_Of_SurfaceMesh<ScalarType>
{
    ScalarType GaussianCurvature;
    ScalarType UnweightedGaussianCurvature; //[-pi, pi]
    ScalarType MeanCurvature; // > 0
    DenseVector<ScalarType, 3> MeanCurvatureNormal; // may have different/opposite direction compared to Normal
    DenseVector<ScalarType, 3> Normal; // unit normal

//-----------------------------------------------
    PointAttribute_Of_PolygonMesh() { this->Clear(); }
    PointAttribute_Of_PolygonMesh(const PointAttribute_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
    ~PointAttribute_Of_PolygonMesh() {}

    void operator=(const PointAttribute_Of_PolygonMesh& InputAttribute)
    {
        GaussianCurvature = InputAttribute.GaussianCurvature;
        UnweightedGaussianCurvature = InputAttribute.UnweightedGaussianCurvature;
        MeanCurvature = InputAttribute.MeanCurvature;
        MeanCurvatureNormal = InputAttribute.MeanCurvatureNormal;
        Normal = InputAttribute.Normal;
    }

    void Clear()
    {
        GaussianCurvature = 0;
        UnweightedGaussianCurvature = 0;
        MeanCurvature = 0;
        MeanCurvatureNormal.Fill(0);
        Normal.Fill(0);
    }
};

//============================================== EdgeAttribute_Of_PolygonMesh ===========================================//
enum class PolygonMeshEdgeAttributeEnum
{
    PhysicalLength,
};


template<typename ScalarType>
struct EdgeAttribute_Of_PolygonMesh : EdgeAttribute_Of_SurfaceMesh<ScalarType>
{
    ScalarType PhysicalLength;

//----------------------------------------------------------------------
    EdgeAttribute_Of_PolygonMesh() { this->Clear(); }
    EdgeAttribute_Of_PolygonMesh(const EdgeAttribute_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
    ~EdgeAttribute_Of_PolygonMesh() {}

    void operator=(const EdgeAttribute_Of_PolygonMesh& InputAttribute)
    {
        PhysicalLength = InputAttribute.PhysicalLength;
    }

    void Clear()
    {
        PhysicalLength = 0;
    }
};

//============================================== DirectedEdgeAttribute_Of_PolygonMesh ===========================================//
enum class PolygonMeshDirectedEdgeAttributeEnum
{
    Orientation,
};

template<typename ScalarType>
struct DirectedEdgeAttribute_Of_PolygonMesh : DirectedEdgeAttribute_Of_SurfaceMesh<ScalarType>
{
    DenseVector<ScalarType, 3> Orientation;
//-------------------------------------------------------
    DirectedEdgeAttribute_Of_PolygonMesh() { this->Clear(); }
    DirectedEdgeAttribute_Of_PolygonMesh(const DirectedEdgeAttribute_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
    ~DirectedEdgeAttribute_Of_PolygonMesh() {}

    void operator=(const DirectedEdgeAttribute_Of_PolygonMesh& InputAttribute)
    {
        Orientation = InputAttribute.Orientation;
    }

    void Clear()
    {
        Orientation.Fill(0);
    }

};

//============================================== CellAttribute_Of_PolygonMesh ===========================================//
enum class PolygonMeshCellAttributeEnum
{
    Area,
};

template<typename ScalarType>
struct CellAttribute_Of_PolygonMesh : CellAttribute_Of_SurfaceMesh<ScalarType>
{
    ScalarType Area;
    DenseVector<ScalarType> CornerAngle;
    DenseVector<ScalarType, 3> Normal;
    bool Flag_ReverseNormalDirection;
//---------------------------------------
    CellAttribute_Of_PolygonMesh() { this->Clear(); }
    CellAttribute_Of_PolygonMesh(const CellAttribute_Of_PolygonMesh& InputAttribute) { (*this) = InputAttribute; }
    ~CellAttribute_Of_PolygonMesh() {}

    void operator=(const CellAttribute_Of_PolygonMesh& InputAttribute)
    {
        Area = InputAttribute.Area;
        CornerAngle = InputAttribute.CornerAngle;
        Normal = InputAttribute.Normal;
        Flag_ReverseNormalDirection = InputAttribute.Flag_ReverseNormalDirection;
    }

    void Clear()
    {
        Area = 0;
        CornerAngle.Clear();
        Normal.Fill(0);
        Flag_ReverseNormalDirection = false;
    }
};

}// namespace mdk


#endif