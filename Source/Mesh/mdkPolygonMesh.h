#ifndef __mdkPolygonMesh_h
#define __mdkPolygonMesh_h

#include <utility>
#include <cmath>

#include "mdkMembraneMesh.h"
#include "mdkPolygonMeshStandardAttribute.h"
#include "mdkGeometry3D.h"

namespace mdk
{

//------------------------------- Empty MeshAttributeType -------------------------------//
template<typename ScalarType>
struct PolygonMeshEmptyAttributeType
{
    typedef ScalarType  ScalarType;
	typedef GlobalStandardAttribute_Of_MembraneMesh<ScalarType>             GlobalAttribute;
	typedef StandardAttribute_Of_Point_Of_MembraneMesh<ScalarType>          PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_MembraneMesh<ScalarType>           EdgeAttributeType;
	typedef StandardAttribute_Of_DirectedEdge_Of_MembraneMesh<ScalarType>   DirectedEdgeAttributeType;
	typedef StandardAttribute_Of_Cell_Of_MembraneMesh<ScalarType>           CellAttributeType;
};
//---------------------------------------------------------------------------------------------------------//

//------------------------------- standard/default MeshAttributeType -------------------------------//
template<typename ScalarType>
struct PolygonMeshStandardAttributeType
{
    typedef ScalarType  ScalarType;
	typedef GlobalStandardAttribute_Of_PolygonMesh<ScalarType>             GlobalAttribute;
	typedef StandardAttribute_Of_Point_Of_PolygonMesh<ScalarType>          PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_PolygonMesh<ScalarType>           EdgeAttributeType;
	typedef StandardAttribute_Of_DirectedEdge_Of_PolygonMesh<ScalarType>   DirectedEdgeAttributeType;
	typedef StandardAttribute_Of_Cell_Of_PolygonMesh<ScalarType>           CellAttributeType;
};
//------------------------------------------------------------------------------------------------//

template<typename MeshAttribute_Type>
class PolygonMesh : public MembraneMesh<MeshAttribute_Type>
{
public:
	typedef MeshAttribute_Type MeshAttributeType;

    typedef PolygonMesh<MeshAttributeType> MeshType;

    typedef typename MeshAttributeType::ScalarType                  ScalarType;
    typedef typename MeshAttributeType::GlobalAttribute             GlobalAttribute;
    typedef typename MeshAttributeType::PointAttributeType          PointAttributeType;
    typedef typename MeshAttributeType::EdgeAttributeType           EdgeAttributeType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType   DirectedEdgeAttributeType;
    typedef typename MeshAttributeType::CellAttributeType           CellAttributeType;

    typedef Point_Of_MembraneMesh<MeshAttributeType>           PointType;
    typedef Edge_Of_MembraneMesh<MeshAttributeType>            EdgeType;
    typedef DirectedEdge_Of_MembraneMesh<MeshAttributeType>    DirectedEdgeType;
    typedef Cell_Of_MembraneMesh<MeshAttributeType>            CellType;

    typedef Handle_Of_Point_Of_MembraneMesh          PointHandleType;
    typedef Handle_Of_Edge_Of_MembraneMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_MembraneMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Cell_Of_MembraneMesh           CellHandleType;

    typedef Iterator_Of_Point_Of_MembraneMesh<MeshAttributeType>         PointIteratorType;
    typedef Iterator_Of_Edge_Of_MembraneMesh<MeshAttributeType>          EdgeIteratorType;
    typedef Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType>  DirectedEdgeIteratorType;
    typedef Iterator_Of_Cell_Of_MembraneMesh<MeshAttributeType>          CellIteratorType;

public:
    PolygonMesh();
    PolygonMesh(const PolygonMesh& InputMesh);
    PolygonMesh(PolygonMesh&& InputMesh);
    ~PolygonMesh();

    inline void operator=(const PolygonMesh& InputMesh);
    inline void operator=(PolygonMesh&& InputMesh);

    //------------ Construct from input data ------------------------------------//

    bool Construct(const DenseMatrix<ScalarType>& InputPointPositionTable, const ObjectArray<DenseVector<int_max>>& InputCellTable);
    // index order in each PointIndexList should be consistent

    void Construct(MembraneMesh<MeshAttributeType> InputMembraneMesh);

    // get a sub mesh by CellHandleList or CellIDList ----------------------------//
    PolygonMesh<MeshAttributeType> GetSubMeshByCell(const DenseVector<CellHandleType>& CellHandleList) const;
    PolygonMesh<MeshAttributeType> GetSubMeshByCell(const DenseVector<int_max>& CellIDList) const;

    //--------------------- output -------------------------------------------------//

    std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>> GetPointPositionMatrixAndCellTable() const;

	void GetPointPositionMatrixAndCellTable(DenseMatrix<ScalarType>& PointPositionTable, ObjectArray<DenseVector<int_max>>& CellTable) const;

    //------------- Mesh Attribute --------------------------------------------------//
	/*
    void UpdateNormalAtCell(); // all
    void UpdateNormalAtCell(CellHandleType CellHandle);
    void UpdateNormalAtCell(int_max CellID);

    void UpdateAreaOfCell(); // all
    void UpdateAreaOfCell(CellHandleType CellHandle);
    void UpdateAreaOfCell(int_max CellID);

    void UpdateCornerAngleOfCell(); // all
    void UpdateCornerAngleOfCell(CellHandleType CellHandle);
    void UpdateCornerAngleOfCell(int_max CellID);

	void UpdateAngleWeightedNormalAtPoint(); // all
	void UpdateAngleWeightedNormalAtPoint(PointHandleType PointHandle);
	void UpdateAngleWeightedNormalAtPoint(int_max PointID);

    void UpdateGaussianCurvatureAtPoint(); // all
    void UpdateGaussianCurvatureAtPoint(PointHandleType PointHandle);
    void UpdateGaussianCurvatureAtPoint(int_max PointID);

    void UpdateMeanCurvatureAtPoint(); // all
    void UpdateMeanCurvatureAtPoint(PointHandleType PointHandle);
    void UpdateMeanCurvatureAtPoint(int_max PointID);
	*/
};

}// namespace mdk

#include "mdkPolygonMesh.hpp"

#endif