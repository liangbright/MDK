#ifndef mdk_MembraneMesh_h
#define mdk_MembraneMesh_h

#include <unordered_map>

#include "mdkMembraneMeshItem.h"
#include "mdkMembraneMeshItemIterator.h"
#include "mdkMembraneMeshStandardAttribute.h"

namespace mdk
{

//------------------------------- standard/empty MeshAttributeType -----------------------//
template<typename ScalarType>
struct MembraneMeshStandardAttributeType
{
    typedef ScalarType  ScalarType;
	typedef GlobalStandardAttribute_Of_MembraneMesh<ScalarType>             GlobalAttribute;
	typedef StandardAttribute_Of_Point_Of_MembraneMesh<ScalarType>          PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_MembraneMesh<ScalarType>           EdgeAttributeType;
	typedef StandardAttribute_Of_DirectedEdge_Of_MembraneMesh<ScalarType>   DirectedEdgeAttributeType;
	typedef StandardAttribute_Of_Face_Of_MembraneMesh<ScalarType>           FaceAttributeType;
};

template<typename ScalarType>
struct MembraneMeshEmptyAttributeType
{
	typedef ScalarType  ScalarType;
	typedef GlobalStandardAttribute_Of_MembraneMesh<ScalarType>             GlobalAttribute;
	typedef StandardAttribute_Of_Point_Of_MembraneMesh<ScalarType>          PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_MembraneMesh<ScalarType>           EdgeAttributeType;
	typedef StandardAttribute_Of_DirectedEdge_Of_MembraneMesh<ScalarType>   DirectedEdgeAttributeType;
	typedef StandardAttribute_Of_Face_Of_MembraneMesh<ScalarType>           FaceAttributeType;
};
//------------------------------------------------------------------------------------------------//

template<typename MeshAttributeType>
struct MembraneMeshData
{
    //-------------------------------------------------------------------------------------------//
    typedef typename MeshAttributeType::ScalarType      ScalarType;
    typedef typename MeshAttributeType::GlobalAttribute GlobalAttribute;
    //-------------------------------------------------------------------------------------------//

    DenseMatrix<ScalarType> PointPositionTable;
    // row_0: x
    // row_1: y
    // row_2: z
    // a point may or may not connect to other point by edge

	StdObjectVector<Point_Of_MembraneMesh<MeshAttributeType>> PointList;

    DenseVector<int_max> PointValidityFlagList; // do not use bool (std::vector<bool> is compressed)
    // 1: point is an element of the mesh 
    // 0: point is deleted

	StdObjectVector<Edge_Of_MembraneMesh<MeshAttributeType>> EdgeList;

    // do not use this, DirectedEdge_Of_MembraneMesh::operator(&&) will not work when append to the list
    //StdObjectVector<DenseVector<DirectedEdge_Of_MembraneMesh<MeshAttributeType>, 2>> DirectedEdgePairList;

    DenseVector<int_max> EdgeValidityFlagList;
    // 1: Edge is an element of the mesh 
    // 0: Edge is deleted

	StdObjectVector<Face_Of_MembraneMesh<MeshAttributeType>> FaceList; // also known as face, facet, element

    DenseVector<int_max>  FaceValidityFlagList;
    // 1: Face is an element of the mesh 
    // 0: Face is deleted

    // Attention: ID must >= 0
    std::unordered_map<int_max, int_max> Map_PointID_to_PointIndex;
    std::unordered_map<int_max, int_max> Map_EdgeID_to_EdgeIndex;
    std::unordered_map<int_max, DirectedEdgeIndex_Of_MembraneMesh> Map_DirectedEdgeID_to_DirectedEdgeIndex;
    std::unordered_map<int_max, int_max> Map_FaceID_to_FaceIndex;

    //Mesh Attribute
    GlobalAttribute Attribute;
};


template<typename MeshAttribute_Type>
class MembraneMesh : public Object
{
public:
	typedef MeshAttribute_Type MeshAttributeType;
    //-------------------------------------------------------------------------------------------//
    typedef typename MeshAttributeType::ScalarType                ScalarType;
    typedef typename MeshAttributeType::GlobalAttribute           GlobalAttribute;
    typedef typename MeshAttributeType::PointAttributeType        PointAttributeType;
    typedef typename MeshAttributeType::EdgeAttributeType         EdgeAttributeType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType DirectedEdgeAttributeType;
    typedef typename MeshAttributeType::FaceAttributeType         FaceAttributeType;
    //--------------------------------------------------------------------------------------------//
    typedef Point_Of_MembraneMesh<MeshAttributeType>           PointType;
    typedef Edge_Of_MembraneMesh<MeshAttributeType>            EdgeType;
    typedef DirectedEdge_Of_MembraneMesh<MeshAttributeType>    DirectedEdgeType;
    typedef Face_Of_MembraneMesh<MeshAttributeType>            FaceType;

    typedef Handle_Of_Point_Of_MembraneMesh          PointHandleType;
    typedef Handle_Of_Edge_Of_MembraneMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_MembraneMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Face_Of_MembraneMesh           FaceHandleType;

    typedef Iterator_Of_Point_Of_MembraneMesh<MeshAttributeType>           PointIteratorType;
    typedef Iterator_Of_Edge_Of_MembraneMesh<MeshAttributeType>            EdgeIteratorType;
    typedef Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttributeType>    DirectedEdgeIteratorType;
    typedef Iterator_Of_Face_Of_MembraneMesh<MeshAttributeType>            FaceIteratorType;
    //--------------------------------------------------------------------------------------------------//
	typedef StdObjectVector<Point_Of_MembraneMesh<MeshAttributeType>>          PointListType;
	typedef StdObjectVector<Edge_Of_MembraneMesh<MeshAttributeType>>           EdgeListType;
	typedef StdObjectVector<DirectedEdge_Of_MembraneMesh<MeshAttributeType>>   DirectedEdgeListType;
	typedef StdObjectVector<Face_Of_MembraneMesh<MeshAttributeType>>           FaceListType;
	//--------------------------------------------------------------------------------------------------//

protected:
    std::shared_ptr<MembraneMeshData<MeshAttributeType>> m_MeshData;

protected:
    template<typename T>
    friend class Point_Of_MembraneMesh;

    template<typename T>
    friend struct Iterator_Of_Point_Of_MembraneMesh;

    template<typename T>
    friend class Edge_Of_MembraneMesh;

    template<typename T>
    friend struct Iterator_Of_Edge_Of_MembraneMesh;

    template<typename T>
    friend class DirectedEdge_Of_MembraneMesh;

    template<typename T>
    friend struct Iterator_Of_DirectedEdge_Of_MembraneMesh;

    template<typename T>
    friend class Face_Of_MembraneMesh;    

    template<typename T>
    friend struct Iterator_Of_Face_Of_MembraneMesh;

public:
    MembraneMesh();
    MembraneMesh(const MDK_Symbol_PureEmpty&);
    MembraneMesh(const MembraneMesh& InputMesh);
    MembraneMesh(MembraneMesh&& InputMesh);
    ~MembraneMesh();

    inline void operator=(const MembraneMesh& InputMesh);
    inline void operator=(MembraneMesh&& InputMesh);

    inline void Clear();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void Copy(const MembraneMesh& InputMesh);
	inline void Copy(MembraneMesh&& InputMesh);
	
	inline void Share(MembraneMesh& InputMesh);
    inline void ForceShare(const MembraneMesh& InputMesh);

	void Recreate();

    //-------------------------------------------------------------------
    inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;

    inline int_max GetPointCount() const;
    inline int_max GetEdgeCount() const;
    inline int_max GetDirectedEdgeCount() const;
    inline int_max GetFaceCount() const;
 
	inline int_max GetMaxValueOfPointIndex() const;// the maximum value of PointHanlde.GetIndex()
	inline int_max GetMaxValueOfEdgeIndex() const;// the maximum value of EdgeHanlde.GetIndex()
	//inline int_max GetMaxValueOfDirectedEdgeIndex() const;// the maximum value of DirectedEdgeHanlde.GetIndex()
	inline int_max GetMaxValueOfFaceIndex() const;// the maximum value of FaceHanlde.GetIndex()

    //------ Get/Set GlobalAttribute -----------------------------------//

    inline GlobalAttribute& Attribute();
    inline const GlobalAttribute& Attribute() const;

	//------------- Get All the position (valid point) --------------------------------//
	inline DenseMatrix<ScalarType> GetPointPositionMatrix() const;
	inline void GetPointPositionMatrix(DenseMatrix<ScalarType>& PositionMatrix) const;

    // Get/Set 3D Position by PointHandle or PointID --------------------------------------------------------------------------//

    inline void SetPointPosition(PointHandleType PointHandle, ScalarType x, ScalarType y, ScalarType z);
    inline void SetPointPosition(PointHandleType PointHandle, const ScalarType Position[3]);

    inline void SetPointPosition(int_max PointID, ScalarType x, ScalarType y, ScalarType z);
    inline void SetPointPosition(int_max PointID, const ScalarType Position[3]);

    inline DenseVector<ScalarType, 3> GetPointPosition(PointHandleType PointHandle) const;
    inline void GetPointPosition(PointHandleType PointHandle, ScalarType& x, ScalarType& y, ScalarType& z) const;
    inline void GetPointPosition(PointHandleType PointHandle, ScalarType Position[3]) const;

    inline DenseVector<ScalarType, 3> GetPointPosition(int_max PointID) const;
    inline void GetPointPosition(int_max PointID, ScalarType& x, ScalarType& y, ScalarType& z) const;
    inline void GetPointPosition(int_max PointID, ScalarType Position[3]) const;

    inline void SetPointPosition(const DenseVector<PointHandleType>& PointHandleList, const DenseMatrix<ScalarType>& PointPositionMatrix);

    inline void SetPointPosition(const DenseVector<int_max>& PointIDList, const DenseMatrix<ScalarType>& PointPositionMatrix);

    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<PointHandleType>& PointHandleList) const;
    inline void GetPointPosition(const DenseVector<PointHandleType>& PointHandleList, DenseMatrix<ScalarType>& PointPositionMatrix) const;

    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<int_max>& PointIDList) const;
    inline void GetPointPosition(const DenseVector<int_max>& PointIDList, DenseMatrix<ScalarType>& PointPositionMatrix) const;
    
    //----- Get/Set Mesh Item {Point, , Edge, DirectedEdge, Face} by using Handle or ID ------//

    inline PointType& Point(PointHandleType PointHandle);
    inline const PointType& Point(PointHandleType PointHandle) const;

    inline PointType& Point(int_max PointID);
    inline const PointType& Point(int_max PointID) const;

    inline EdgeType& Edge(EdgeHandleType EdgeHandle);
    inline const EdgeType& Edge(EdgeHandleType EdgeHandle) const;

    inline EdgeType& Edge(int_max EdgeID);
    inline const EdgeType& Edge(int_max EdgeID) const;

    inline DirectedEdgeType& DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle);
    inline const DirectedEdgeType& DirectedEdge(DirectedEdgeHandleType DirectedEdgeHandle) const;

    inline DirectedEdgeType& DirectedEdge(int_max DirectedEdgeID);
    inline const DirectedEdgeType& DirectedEdge(int_max DirectedEdgeID) const;

    inline FaceType& Face(FaceHandleType FaceHandle);
    inline const FaceType& Face(FaceHandleType FaceHandle) const;

    inline FaceType& Face(int_max FaceID);
    inline const FaceType& Face(int_max FaceID) const;

    //-------------- check handle -------------------------------------------------------//

    inline bool IsValidHandle(PointHandleType PointHandle) const;
    inline bool IsValidHandle(EdgeHandleType EdgeHandle) const;
    inline bool IsValidHandle(DirectedEdgeHandleType DirectedEdgeHandle) const;
    inline bool IsValidHandle(FaceHandleType FaceHandle) const;

    //--------- get HandleList ------------------------------------------------------------//

    inline DenseVector<PointHandleType> GetPointHandleList() const;
    inline void GetPointHandleList(DenseVector<PointHandleType>& OutputHandleList) const;

    inline DenseVector<EdgeHandleType> GetEdgeHandleList() const;
    inline void GetEdgeHandleList(DenseVector<EdgeHandleType>& OutputHandleList) const;

    inline DenseVector<DirectedEdgeHandleType> GetDirectedEdgeHandleList() const;
    inline void GetDirectedEdgeHandleList(DenseVector<DirectedEdgeHandleType>& OutputHandleList) const;

    inline DenseVector<FaceHandleType> GetFaceHandleList() const;
    inline void GetFaceHandleList(DenseVector<FaceHandleType>& OutputHandleList) const;

    //----------- get PointHandle by Position, ID, ----------------------------------------------//

    inline PointHandleType GetPointHandleByPosition(const DenseVector<ScalarType, 3>& Position,
                                                    ScalarType DistanceThreshold = std::numeric_limits<ScalarType>::epsilon()) const;
    inline PointHandleType GetPointHandleByPosition(const ScalarType Position[3], 
                                                    ScalarType DistanceThreshold = std::numeric_limits<ScalarType>::epsilon()) const;
    inline PointHandleType GetPointHandleByPosition(ScalarType x, ScalarType y, ScalarType z,
                                                    ScalarType DistanceThreshold = std::numeric_limits<ScalarType>::epsilon()) const;
    inline PointHandleType  GetPointHandleByID(int_max PointID) const;

    //----------- get EdgeHandle by ID, PointHandleList, PointIDList -------------------------------------//

    inline EdgeHandleType GetEdgeHandleByID(int_max EdgeID) const;
    inline EdgeHandleType GetEdgeHandleByPoint(PointHandleType PointHandle0, PointHandleType PointHandle1) const;
    inline EdgeHandleType GetEdgeHandleByPoint(int_max PointID0, int_max PointID1) const;

    //----------- get DirectedEdgeHandle by ID, PointHandleList, PointIDList -------------------------------//

    inline DirectedEdgeHandleType GetDirectedEdgeHandleByID(int_max DirectedEdgeID) const;

	// non-manifold is allowed: maybe more than one DirectedEdge from PointHandle_start to PointHandle_end
	//                          there are many face containing PointHandle_start and  PointHandle_end
	//                          each face has a DirectedEdge from PointHandle_start to PointHandle_end
    inline DenseVector<DirectedEdgeHandleType> GetDirectedEdgeHandleByPoint(PointHandleType PointHandle_start, PointHandleType PointHandle_end) const;
	inline DenseVector<DirectedEdgeHandleType> GetDirectedEdgeHandleByPoint(int_max PointID_start, int_max PointID_end) const;

    //----------- get FaceHandle by ID, PointHandleList or PointIDList EdgeHandleList or EdgeIDList ----------//

    inline FaceHandleType GetFaceHandleByID(int_max FaceID) const;

	// PointHandleList/PointIDList/EdgeHandleList/EdgeIDList can have random point-order 
    inline FaceHandleType GetFaceHandleByPoint(const DenseVector<PointHandleType>& PointHandleList) const;
    inline FaceHandleType GetFaceHandleByPoint(const DenseVector<int_max>& PointIDList) const;
    inline FaceHandleType GetFaceHandleByEdge(const DenseVector<EdgeHandleType>& EdgeHandleList) const;
    inline FaceHandleType GetFaceHandleByEdge(const DenseVector<int_max>& EdgeIDList) const;

    //-------------- check ID -------------------------------------------------------//

    inline bool IsValidPointID(int_max PointID) const;
    inline bool IsValidEdgeID(int_max EdgeID) const;
    inline bool IsValidDirectedEdgeID(int_max DirectedEdgeID) const;
    inline bool IsValidFaceID(int_max FaceID) const;

    //--------- get IDList ------------------------------------------------------------//

    inline DenseVector<int_max> GetPointIDList() const;
    inline void GetPointIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetEdgeIDList() const;
    inline void GetEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetDirectedEdgeIDList() const;
    inline void GetDirectedEdgeIDList(DenseVector<int_max>& OutputIDList) const;

    inline DenseVector<int_max> GetFaceIDList() const;
    inline void GetFaceIDList(DenseVector<int_max>& OutputIDList) const;

    //----------- get Point ID by Handle, Position -----------------------------------------------------------//

    inline int_max GetPointIDByHandle(PointHandleType PointHandle) const;
    inline int_max GetPointIDByPosition(ScalarType Position[3]) const;
    inline int_max GetPointIDByPosition(ScalarType x, ScalarType y, ScalarType z) const;

    //----------- get Edge ID by EdgeHandle, PointHandleList, PointIDList ------------------------------------------------------//

    inline int_max GetEdgeIDByHandle(EdgeHandleType EdgeHandle) const;
    inline int_max GetEdgeIDByPoint(PointHandleType PointHandle0, PointHandleType PointHandle1) const;
    inline int_max GetEdgeIDByPoint(int_max PointID0, int_max PointID1) const;

    //----------- get DirectedEdge ID by DirectedEdgeHandle, PointHandle, PointID -------------------------------------------//

    inline int_max GetDirectedEdgeIDByHandle(DirectedEdgeHandleType DirectedEdgeHandle) const;
    inline int_max GetDirectedEdgeIDByPoint(PointHandleType PointHandle_start, PointHandleType PointHandle_end) const;
    inline int_max GetDirectedEdgeIDByPoint(int_max PointID_start, int_max PointID_end) const;

    //----------- get FaceID by FaceHandle, EdgeHandleList, EdgeIDList, PointHandleList, PointIDList --------------------------//

    inline int_max GetFaceIDByHandle(FaceHandleType FaceHandle) const;
    inline int_max GetFaceIDByPoint(const DenseVector<PointHandleType>& PointHandleList) const;
    inline int_max GetFaceIDByPoint(const DenseVector<int_max>& PointIDList) const;
    inline int_max GetFaceIDByEdge(const DenseVector<EdgeHandleType>& EdgeHandleList) const;
    inline int_max GetFaceIDByEdge(const DenseVector<int_max>& EdgeIDList) const;

    //------------- Iterator --------------------------------------------------------------//

    inline PointIteratorType  GetIteratorOfPoint();
    inline const PointIteratorType  GetIteratorOfPoint() const;

    inline EdgeIteratorType   GetIteratorOfEdge();
    inline const EdgeIteratorType   GetIteratorOfEdge() const;

    inline DirectedEdgeIteratorType   GetIteratorOfDirectedEdge();
    inline const DirectedEdgeIteratorType   GetIteratorOfDirectedEdge() const;

    inline FaceIteratorType   GetIteratorOfFace();
    inline const FaceIteratorType   GetIteratorOfFace() const;

	//------------ SetCapacity, ReleaseUnusedCapacity -------------------------------------//

	void SetCapacity(int_max PointCount, int_max EdgeCount, int_max FaceCount);

	void ReleaseUnusedCapacity();

    // Add Mesh Item -------------------------------------------------------------------------//
    // add an item and return index (-1 if input is invalid)

    // add a Point and return PointHandle -> PointIndex in m_MeshData->PointList
    PointHandleType AddPoint(const DenseVector<ScalarType, 3>& Position);
    PointHandleType AddPoint(const DenseVector<ScalarType>& Position);
    PointHandleType AddPoint(const DenseMatrix<ScalarType>& Position);
    PointHandleType AddPoint(const ScalarType Position[3]);
    PointHandleType AddPoint(ScalarType x, ScalarType y, ScalarType z);
    
    // add a set of points and return PointHandleList
    DenseVector<PointHandleType> AddPointSet(const DenseMatrix<ScalarType>& PointSet);
	DenseVector<PointHandleType> AddPointSet(DenseMatrix<ScalarType>&& PointSet);

    // add an Edge and return EdgeHandle -> EdgeIndex in m_MeshData->EdgeList
    // also create invalid DirectedEdge to hold place at m_MeshData->DirectedEdgeList[EdgeIndex]
    // Check is performed in the function to ensure that an edge will not be added more than once
    EdgeHandleType AddEdge(PointHandleType PointHandle0, PointHandleType PointHandle1);
    EdgeHandleType AddEdge(int_max PointID0, int_max PointID1);

    // add a cell and return FaceHandle -> FaceIndex in m_MeshData->FaceList
    // add DirectedEdge of the cell
    // the order of Edge in EdgeHandleList determine the direction of each DirectedEdge and the direction/sign of the normal vector
    FaceHandleType AddFaceByEdge(const DenseVector<EdgeHandleType>& EdgeHandleList);
    FaceHandleType AddFaceByEdge(const DenseVector<int_max>& EdgeIDList);

    // Add Face with PointHandleList, Point0 <- Edge0 -> Point1 <- Edge1 -> Point2 ... Point_end <- Edge_end -> Point0
    // in this function, AddFaceByEdge() is called if necessary
    FaceHandleType AddFaceByPoint(const DenseVector<PointHandleType>& PointHandleList);
    FaceHandleType AddFaceByPoint(const DenseVector<int_max>& PointIDList);

    // Delete Mesh Item ----------------------------------------------------------------------------//

    // m_MeshData->FaceList[FaceIndex].Clear() only release memory
    // this function will delete each DirectedEdge of the Face, and modify any information related to the cell
    // FaceHandle and FaceID of the cell become invalid after the cell is deleted
	void DeleteFace(FaceHandleType FaceHandle);
	void DeleteFace(int_max FaceID);

    // m_MeshData->EdgeList[EdgeIndex].Clear() only release memory
    // this function will modify any information related to the Edge
    // EdgeHandle and EdgeID of the edge become invalid after the edge is deleted
    // Check is performed in the function to make sure an edge can not be deleted if any adjacent cell exit
	//
	// note:  Call Edge(EdgeHandle).GetAdjacentFaceCount() to check if the edge can be deleted or not
	void DeleteEdge(EdgeHandleType EdgeHandle);
	void DeleteEdge(int_max EdgeID);

    // m_MeshData->PointList[PointIndex].Cear() only release memory
    // this function will modify any information related to the point
    // Check is performed in the function to make sure a point can not be deleted if any adjacent edge exit	
    // PointHandle and PointID of the point become invalid after the point is deleted
	//
	// note: Call Point(PointHandle).IsOnEdge() to check if the point can be deleted or not
	void DeletePoint(PointHandleType PointHandle);
	void DeletePoint(const DenseVector<PointHandleType>& PointHandleList);
    void DeletePoint(int_max PointID);
    void DeletePoint(const DenseVector<int_max>& PointIDList);

    // remove deleted item from object list ----------------------------------------------------------//
    // attention: after CleanDataStructure() is called, handle may become invalid, but, ID will not change
    // use this function when InvalidPointHandleCount/ValidPointCount( GetPointCount() ) > 0.5
    void CleanDataStructure();	
	bool Check_If_DataStructure_is_Clean() const;//true: clean, false: invalid handle exit
    int_max GetDeletedPointCount() const; // the number of Deleted points
	int_max GetDeletedEdgeCount() const; // the number of Deleted edges
	int_max GetDeletedFaceCount() const; // the number of Deleted cells

    //---------------------------------------------------------------------------------------------------

    // get a sub mesh by FaceHandleList or FaceIDList
    MembraneMesh<MeshAttributeType> GetSubMeshByFace(const DenseVector<FaceHandleType>& FaceHandleList) const;
    MembraneMesh<MeshAttributeType> GetSubMeshByFace(const DenseVector<int_max>& FaceIDList) const;

    // Change Topology ----------------------------------------------------------------------------------------

    FaceHandleType DilatePointToFace(PointHandleType PointHandle);
    FaceHandleType DilatePointToFace(int_max PointID);

    // Position  = (Position_0 + Position_1) / 2
    PointHandleType ShrinkEdgeToPoint(EdgeHandleType EdgeHandle);
    PointHandleType ShrinkEdgeToPoint(int_max EdgeID);

    //EdgeHandleType MergeTwoAdjacentEdge(EdgeHandleType EdgeHandleA, EdgeHandleType EdgeHandleB);
    //EdgeHandleType MergeTwoAdjacentEdge(int_max EdgeIDA, int_max EdgeIDB);

    std::pair<EdgeHandleType, EdgeHandleType> SplitEdge(PointHandleType PointHandle);
    std::pair<EdgeHandleType, EdgeHandleType> SplitEdge(int_max PointID);

    // MiddlePointPosition  = sum(Position_i) / sum(i)
    PointHandleType ShrinkFaceToPoint(FaceHandleType FaceHandle);
    PointHandleType ShrinkFaceToPoint(int_max FaceID);

    FaceHandleType MergeTwoAdjacentFace(FaceHandleType FaceHandleA, FaceHandleType FaceHandleB);
    FaceHandleType MergeTwoAdjacentFace(int_max FaceIDA, int_max FaceIDB);

	std::pair<FaceHandleType, FaceHandleType> SplitFaceByTwoPoint(PointHandleType PointHandleA, PointHandleType PointHandleB);
	std::pair<FaceHandleType, FaceHandleType> SplitFaceByTwoPoint(int_max PointIDA, int_max PointIDB);

    //-----------------------------------------------------------------------------------------------------

private:
    void InternalFuction_DeletePoint(int_max PointIndex);
    void InternalFuction_DeleteEdge(int_max EdgeIndex);
	void InternalFuction_DeleteDirectedEdge(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex);
    void InternalFuction_DeleteFace(int_max FaceIndex);
};

}// namespace mdk

#include "mdkMembraneMesh.hpp"

#endif