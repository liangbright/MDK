#pragma once

#include <tuple>
#include <unordered_map>

#include "mdkMeshItem.h"
#include "mdkMeshStandardAttribute.h"

namespace mdk
{

template<typename ScalarType>
struct MeshData
{
	String Name;// name of the mesh

    DenseMatrix<ScalarType> PointPositionTable;
    // row_0: x
    // row_1: y
    // row_2: z
    // a point may or may not connect to other point by edge

	StdObjectVector<Point_Of_Mesh<ScalarType>> PointList;

    DenseVector<int_max> PointValidityFlagList; 
    // 1: point is an element of the mesh 
    // 0: point is deleted
	// problem if using int8, PointValidityFlagList.Sum() may overflow
	// do not use bool (std::vector<bool> is compressed)

	StdObjectVector<Edge_Of_Mesh<ScalarType>> EdgeList;
    DenseVector<int_max> EdgeValidityFlagList;
    // 1: Edge is an element of the mesh 
    // 0: Edge is deleted

	StdObjectVector<Face_Of_Mesh<ScalarType>> FaceList;
    DenseVector<int_max>  FaceValidityFlagList;
    // 1: Face is an element of the mesh 
    // 0: Face is deleted

	StdObjectVector<Cell_Of_Mesh<ScalarType>> CellList;
	DenseVector<int_max>  CellValidityFlagList;
	// 1: Cell is an element of the mesh 
	// 0: Cell is deleted
	
	std::unordered_map<String, int_max, StringHash<String>> Map_Point_Name_to_Index;
	std::unordered_map<String, int_max, StringHash<String>> Map_Edge_Name_to_Index;
	std::unordered_map<String, int_max, StringHash<String>> Map_Face_Name_to_Index;
	std::unordered_map<String, int_max, StringHash<String>> Map_Cell_Name_to_Index;

	StdObjectVector<DenseMatrix<ScalarType>> PointDataSet;	//PointDataSet[DataSetIndex](:, PointIndex) 
	std::unordered_map<String, int_max, StringHash<String>> Map_PointDataSet_Name_to_Index;

	StdObjectVector<DenseMatrix<ScalarType>> EdgeDataSet; //EdgeDataSet[DataSetIndex](:, EdgeIndex)
	std::unordered_map<String, int_max, StringHash<String>> Map_EdgeDataSet_Name_to_Index;
	
	StdObjectVector<DenseMatrix<ScalarType>> FaceDataSet; //FaceDataSet[DataSetIndex](:, FaceIndex)
	std::unordered_map<String, int_max, StringHash<String>> Map_FaceDataSet_Name_to_Index;

	StdObjectVector<DenseMatrix<ScalarType>> CellDataSet; //CellDataSet[DataSetIndex](:, CellIndex)
	std::unordered_map<String, int_max, StringHash<String>> Map_CellDataSet_Name_to_Index;

	StdObjectVector<DenseVector<int_max>>  PointSetList;// PointSetList[k] is a set of PointIndex
	std::unordered_map<String, int_max, StringHash<String>> Map_PointSet_Name_to_Index;

	StdObjectVector<DenseVector<int_max>>  EdgeSetList;// EdgeSetList[k] is a set of EdgeIndex
	std::unordered_map<String, int_max, StringHash<String>> Map_EdgeSet_Name_to_Index;

	StdObjectVector<DenseVector<int_max>>  FaceSetList; // FaceSetList[k] is a set of FaceIndex
	std::unordered_map<String, int_max, StringHash<String>> Map_FaceSet_Name_to_Index;

	StdObjectVector<DenseVector<int_max>>  CellSetList; // CellSetList[k] is a set of CellIndex
	std::unordered_map<String, int_max, StringHash<String>> Map_CellSet_Name_to_Index;

	StandardAttribute_Of_Mesh<ScalarType> Attribute;
};


template<typename Scalar_Type>
class Mesh : public Object
{
public:
	typedef Scalar_Type ScalarType;
    //------------------------------------------------------------------------//
	typedef StandardAttribute_Of_Mesh<ScalarType>          MeshAttribute;
	typedef StandardAttribute_Of_Point_Of_Mesh<ScalarType> PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_Mesh<ScalarType>  EdgeAttributeType;
	typedef StandardAttribute_Of_Face_Of_Mesh<ScalarType>  FaceAttributeType;
	typedef StandardAttribute_Of_Cell_Of_Mesh<ScalarType>  CellAttributeType;
    //------------------------------------------------------------------------//
    typedef Point_Of_Mesh<ScalarType>  PointType;
    typedef Edge_Of_Mesh<ScalarType>   EdgeType;
    typedef Face_Of_Mesh<ScalarType>   FaceType;
	typedef Cell_Of_Mesh<ScalarType>   CellType;
    //------------------------------------------------------------------------//
	typedef StdObjectVector<PointType> PointListType;
	typedef StdObjectVector<EdgeType>  EdgeListType;
	typedef StdObjectVector<FaceType>  FaceListType;
	typedef StdObjectVector<CellType>  CellListType;
	//------------------------------------------------------------------------//
	typedef StdObjectVector<DenseVector<DenseVector<ScalarType>>> DataSetType;

protected:
    std::shared_ptr<MeshData<ScalarType>> m_MeshData;

protected:
    template<typename T>
    friend class Point_Of_Mesh;

    template<typename T>
    friend class Edge_Of_Mesh;

    template<typename T>
    friend class Face_Of_Mesh;

	template<typename T>
	friend class Cell_Of_Mesh;

public:
    Mesh();
    Mesh(const MDK_Symbol_PureEmpty&);
    Mesh(const Mesh& InputMesh);
    Mesh(Mesh&& InputMesh);
    ~Mesh();

    inline void operator=(const Mesh& InputMesh);
    inline void operator=(Mesh&& InputMesh);

    inline void Clear();
	inline void Clear(const MDK_Symbol_PureEmpty&);

    inline void Copy(const Mesh& InputMesh);
	inline void Copy(Mesh&& InputMesh);
	
	inline void Share(Mesh& InputMesh);
    inline void ForceShare(const Mesh& InputMesh);

	void Recreate();

    //-------------------------------------------------------------------
    inline bool IsEmpty() const;
	inline bool IsPureEmpty() const;

	//valid Point/Edge/Face/Cell Count
    inline int_max GetPointCount() const;
    inline int_max GetEdgeCount() const;
    inline int_max GetFaceCount() const;
	inline int_max GetCellCount() const;

	inline int_max GetNamedPointCount() const;
	inline int_max GetNamedEdgeCount() const;
	inline int_max GetNamedFaceCount() const;
	inline int_max GetNamedCellCount() const;

	// valid+invalid/deleted Point/Edge/Face/Cell Count
	inline int_max GetMaxValueOfPointIndex() const;
	inline int_max GetMaxValueOfEdgeIndex() const;	
	inline int_max GetMaxValueOfFaceIndex() const;
	inline int_max GetMaxValueOfCellIndex() const;

    //------ Get/Set Attribute -----------------------------------//

    inline MeshAttribute& Attribute();
    inline const MeshAttribute& Attribute() const;

	//-----------------------------------------------------------------//	
	void SetName(String Name);
	String GetName() const;

	bool CheckIfPolygonMesh() const;
	bool CheckIfTriangleMesh() const;
	bool CheckIfQuadMesh() const;
	bool CheckIfMixedTriangleQuadMesh() const;
	bool CheckIfPolyhedronMesh() const;	
	bool CheckIfTetrahedronMesh() const;
	bool CheckIfHexahedronMesh() const;
	bool CheckIfMixedTetHexMesh() const;

	//------------- Get/Set All the position (valid point) --------------------------------//

	inline DenseMatrix<ScalarType> GetPointPosition(const MDK_Symbol_ALL&) const;
	inline void GetPointPosition(const MDK_Symbol_ALL&, DenseMatrix<ScalarType>& PositionMatrix) const;

	inline void SetPointPosition(const MDK_Symbol_ALL&, const DenseMatrix<ScalarType>& PositionMatrix) const;

    // Get/Set 3D Position by Index --------------------------------------------------------------------------//

    inline void SetPointPosition(int_max PointIndex, ScalarType x, ScalarType y, ScalarType z);
    inline void SetPointPosition(int_max PointIndex, const ScalarType Position[3]);
	inline void SetPointPosition(int_max PointIndex, const DenseVector<ScalarType, 3>& Position);
	inline void SetPointPosition(const DenseVector<int_max>& PointIndexList, const DenseMatrix<ScalarType>& PointPositionMatrix);

    inline DenseVector<ScalarType, 3> GetPointPosition(int_max PointIndex) const;
	inline void GetPointPosition(int_max PointIndex, DenseVector<ScalarType, 3>& Position) const;	
	inline void GetPointPosition(int_max PointIndex, ScalarType& x, ScalarType& y, ScalarType& z) const;
	inline void GetPointPosition(int_max PointIndex, ScalarType Position[3]) const;
    inline DenseMatrix<ScalarType> GetPointPosition(const DenseVector<int_max>& PointIndexList) const;
    inline void GetPointPosition(const DenseVector<int_max>& PointIndexList, DenseMatrix<ScalarType>& PointPositionMatrix) const;
    
	// get PointPositionMatrix And FaceTable
	std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>> GetPointPositionMatrixAndFaceTable() const;
	void GetPointPositionMatrixAndFaceTable(DenseMatrix<ScalarType>& PointPositionMatrix, ObjectArray<DenseVector<int_max>>& FaceTable) const;

	//---- construct a polygon mesh ------------------
	void Construct(DenseMatrix<ScalarType> InputPointPositionMatrix, const ObjectArray<DenseVector<int_max>>& InputFaceTable);

    //----- Get/Set Mesh Item {Point, Edge, Face, Cell} by index  ------//

    inline PointType& Point(int_max PointIndex);
    inline const PointType& Point(int_max PointIndex) const;

    inline EdgeType& Edge(int_max EdgeIndex);
    inline const EdgeType& Edge(int_max EdgeIndex) const;

    inline FaceType& Face(int_max FaceIndex);
    inline const FaceType& Face(int_max FaceIndex) const;

	inline CellType& Cell(int_max CellIndex);
	inline const CellType& Cell(int_max CellIndex) const;

    //-------------- check index -------------------------------------------------------//
	// true: exist, false: not-exist
    inline bool IsValidPointIndex(int_max PointIndex) const;
    inline bool IsValidEdgeIndex(int_max EdgeIndex) const;
    inline bool IsValidFaceIndex(int_max FaceIndex) const;
	inline bool IsValidCellIndex(int_max CellIndex) const;

    //--------- get valid IndexList ------------------------------------------------------//
    inline DenseVector<int_max> GetPointIndexList() const;
    inline DenseVector<int_max> GetEdgeIndexList() const;
    inline DenseVector<int_max> GetFaceIndexList() const;
	inline DenseVector<int_max> GetCellIndexList() const;

    //----------- get PointIndex by Position, ID, Name ----------------------------------------------//
    inline int_max GetPointIndexByPosition(const DenseVector<ScalarType, 3>& Position, ScalarType DistanceThreshold) const;
    inline int_max GetPointIndexByPosition(const ScalarType Position[3], ScalarType DistanceThreshold) const;
    inline int_max GetPointIndexByPosition(ScalarType x, ScalarType y, ScalarType z, ScalarType DistanceThreshold) const;
	inline int_max GetPointIndexByName(const String& PointName) const;

    //----------- get EdgeIndex by Point, Name -------------------------------------//
	inline int_max GetEdgeIndexByPoint(int_max PointIndex0, int_max PointIndex1) const;
	inline int_max GetEdgeIndexByName(const String& EdgeName) const;

    //----------- get FaceIndex by Point, Edge, Name ----------//    
	// PointIndexList/EdgeIndexList can have random order of point/edge
	inline int_max GetFaceIndexByPoint(const DenseVector<int_max>& PointIndexList) const;
    inline int_max GetFaceIndexByEdge(const DenseVector<int_max>& EdgeIndexList) const;	
	inline int_max GetFaceIndexByName(const String& FaceName) const;

	//----------- get CellIndex by Point, Edge, Face, Name ----------//    
	// PointIndexList/EdgeIndexList/FaceIndexList can have random order of point/edge
	inline int_max GetCellIndexByPoint(const DenseVector<int_max>& PointIndexList) const;
	inline int_max GetCellIndexByEdge(const DenseVector<int_max>& EdgeIndexList) const;
	inline int_max GetCellIndexByFace(const DenseVector<int_max>& FaceIndexList) const;
	inline int_max GetCellIndexByName(const String& CellName) const;

	//-------------- check Name -------------------------------------------------------//
	// true: exist, false: not-exist
	inline bool IsValidPointName(const String& PointName) const;
	inline bool IsValidEdgeName(const String& EdgeName) const;	
	inline bool IsValidFaceName(const String& FaceName) const;
	inline bool IsValidCellName(const String& CellName) const;

	//--------- get Valid NameList ------------------------------------------------------------//
	ObjectArray<String> GetValidPointNameList() const;
	ObjectArray<String> GetValidEdgeNameList() const;	
	ObjectArray<String> GetValidFaceNameList() const;
	ObjectArray<String> GetValidCellNameList() const;

	//----------- get Point/Edge/Face Name by Index -----------------------------------------------------------//
	inline String GetPointName(int_max PointIndex) const;
	inline ObjectArray<String> GetPointName(const DenseVector<int_max>& PointIndexList) const;
	inline String GetEdgeName(int_max EdgeIndex) const;
	inline ObjectArray<String> GetEdgeName(const DenseVector<int_max>& EdgeIndexList) const;	
	inline String GetFaceName(int_max FaceIndex) const;
	inline ObjectArray<String> GetFaceName(const DenseVector<int_max>& FaceIndexList) const;	

	//----------- PointDataSet, EdgeDataSet, FaceDataset, CellDataSet ----------------------------------//		
	// PointDataSet/EdgeDataSet/FaceDataset/CellDataSet will NOT be updated if new Point/Edge/Face is added, or old is deleted
	// They will only be updated in CleanDataStructure
	int_max GetPointDataSetCount() const;
	int_max InitializePointDataSet(const String& Name, int_max ScalarCountPerPoint);//return DataSetIndex or -1
	int_max SetPointDataSet(const String& Name, const DenseMatrix<ScalarType>& DataSet);//create new set if not exist
	int_max GetPointDataSetIndex(const String& Name) const;
	String GetPointDataSetName(int_max Index) const;
	DenseMatrix<ScalarType> GetPointDataSet(int_max Index) const;
	DenseMatrix<ScalarType> GetPointDataSet(const String& Name) const;
	//Point(k).GetData(int_max Index);             
	//Point(k).SetData(int_max Index, const DenseVector<ScalarType>& Data);

	int_max GetEdgeDataSetCount() const;
	int_max InitializeEdgeDataSet(const String& Name, int_max ScalarCountPerEdge);//return DataSetIndex or -1
	int_max SetEdgeDataSet(const String& Name, const DenseMatrix<ScalarType>& DataSet);//create new set if not exist
	int_max GetEdgeDataSetIndex(const String& Name) const;
	String GetEdgeDataSetName(int_max Index) const;
	DenseMatrix<ScalarType> GetEdgeDataSet(int_max Index) const;
	DenseMatrix<ScalarType> GetEdgeDataSet(const String& Name) const;
	//Edge(k).GetData(int_max Index);
	//Edge(k).SetData(int_max Index, const DenseVector<ScalarType>& Data);

	int_max GetFaceDataSetCount() const;
	int_max InitializeFaceDataSet(const String& Name, int_max ScalarCountPerFace);//return DataSetIndex or -1
	int_max SetFaceDataSet(const String& Name, const DenseMatrix<ScalarType>& DataSet);//create new set if not exist
	int_max GetFaceDataSetIndex(const String& Name) const;
	String GetFaceDataSetName(int_max Index) const;
	DenseMatrix<ScalarType> GetFaceDataSet(int_max Index) const;
	DenseMatrix<ScalarType> GetFaceDataSet(const String& Name) const;
	//Face(k).GetData(int_max Index);	
	//Face(k).SetData(int_max Index, const DenseVector<ScalarType>& Data);

	int_max GetCellDataSetCount() const;
	int_max InitializeCellDataSet(const String& Name, int_max ScalarCountPerCell);//return CellSetIndex or -1
	int_max SetCellDataSet(const String& Name, const DenseMatrix<ScalarType>& DataSet);//create new set if not exist
	int_max GetCellDataSetIndex(const String& Name) const;
	String GetCellDataSetName(int_max Index) const;
	DenseMatrix<ScalarType> GetCellDataSet(int_max Index) const;
	DenseMatrix<ScalarType> GetCellDataSet(const String& Name) const;
	//Cell(k).GetData(int_max Index);
	//Cell(k).SetData(int_max Index, const DenseVector<ScalarType>& Data);

	//------------ PointSet, EdgeSet, FaceSet, CellSet ------------------------------------------//
	// PointSet/EdgeSet/FaceSet/CellSet will NOT be updated if new Point/Edge/Face is added, or old is deleted
	// They will only be updated in CleanDataStructure
	int_max GetPointSetCount() const;
	int_max SetPointSet(const String& PointSetName, DenseVector<int_max> PointSet);
	int_max GetPointSetIndex(const String& PointSetName) const;
	String GetPointSetName(int_max PointSetIndex) const;
	DenseVector<int_max> GetPointSet(int_max PointSetIndex) const;
	DenseVector<int_max> GetPointSet(const String& PointSetName) const;
	ObjectArray<String> GetPointSetName(MDK_Symbol_ALL&) const;
	ObjectArray<DenseVector<int_max>> GetPointSet(MDK_Symbol_ALL&) const;

	int_max GetEdgeSetCount() const;
	int_max SetEdgeSet(const String& EdgeSetName, DenseVector<int_max> EdgeSet);
	int_max GetEdgeSetIndex(const String& EdgeSetName) const;
	String GetEdgeSetName(int_max EdgeSetIndex) const;
	DenseVector<int_max> GetEdgeSet(int_max EdgeSetIndex) const;
	DenseVector<int_max> GetEdgeSet(const String& EdgeSetName) const;
	ObjectArray<String> GetEdgeSetName(MDK_Symbol_ALL&) const;
	ObjectArray<DenseVector<int_max>> GetEdgeSet(MDK_Symbol_ALL&) const;

	int_max GetFaceSetCount() const;
	int_max SetFaceSet(const String& FaceSetName, DenseVector<int_max> FaceSet);
	int_max GetFaceSetIndex(const String& FaceSetName) const;
	String GetFaceSetName(int_max FaceSetIndex) const;
	DenseVector<int_max> GetFaceSet(int_max FaceSetIndex) const;
	DenseVector<int_max> GetFaceSet(const String& FaceSetName) const;
	ObjectArray<String> GetFaceSetName(MDK_Symbol_ALL&) const;
	ObjectArray<DenseVector<int_max>> GetFaceSet(MDK_Symbol_ALL&) const;

	int_max GetCellSetCount() const;
	int_max SetCellSet(const String& CellSetName, DenseVector<int_max> CellSet);
	int_max GetCellSetIndex(const String& CellSetName) const;
	String GetCellSetName(int_max CellSetIndex) const;
	DenseVector<int_max> GetCellSet(int_max CellSetIndex) const;
	DenseVector<int_max> GetCellSet(const String& CellSetName) const;
	ObjectArray<String> GetCellSetName(MDK_Symbol_ALL&) const;
	ObjectArray<DenseVector<int_max>> GetCellSet(MDK_Symbol_ALL&) const;

	//------------ SetCapacity, ReleaseUnusedCapacity -------------------------------------//
	void SetCapacity(int_max PointCount, int_max EdgeCount, int_max FaceCount);
	void SetCapacity(int_max PointCount, int_max EdgeCount, int_max FaceCount, int_max CellCount);	
	void ReleaseUnusedCapacity();

    // Add Mesh Item -------------------------------------------------------------------------//
    // add an item and return index (-1 if input is invalid)

    // add a Point and return PointIndex in m_MeshData->PointList
    int_max AddPoint(const DenseVector<ScalarType, 3>& Position);
	int_max AddPoint(const DenseVector<ScalarType>& Position);
	int_max AddPoint(const DenseMatrix<ScalarType>& Position);
	int_max AddPoint(const ScalarType Position[3]);
	int_max AddPoint(ScalarType x, ScalarType y, ScalarType z);
    
    // add a set of points and return PointIndexList
	// do not use the name "AddPointSet", PointSet mean something else
    DenseVector<int_max> AddPoint_batch(const DenseMatrix<ScalarType>& PointSet);
	DenseVector<int_max> AddPoint_batch(DenseMatrix<ScalarType>&& PointSet);

    // add an Edge and return EdgeIndex in m_MeshData->EdgeList
    // Check is performed in the function to ensure that an edge will not be added more than once
	int_max AddEdge(int_max PointIndex0, int_max PointIndex1);

	// EdgeIndex_input may be the Index of a deleted edge, so to reuse old EdgeIndex if necessary
	int_max AddEdge(int_max PointIndex0, int_max PointIndex1, int_max EdgeIndex_input);

    // add a face and return FaceIndex in m_MeshData->FaceList
    // the order of Edge in EdgeIndexList determine the direction/sign of the normal vector
	int_max AddFaceByEdge(const DenseVector<int_max>& EdgeIndexList);

	//FaceIndex_input may be the Index of a deleted face, so to reused old FaceIndex if necessary
	int_max AddFaceByEdge(const DenseVector<int_max>& EdgeIndexList, int_max FaceIndex_input);

    // Add Face with PointIndexList, Point0 <- Edge0 -> Point1 <- Edge1 -> Point2 ... Point_end <- Edge_end -> Point0
    // in this function, AddFaceByEdge() is called if necessary
	int_max AddFaceByPoint(const DenseVector<int_max>& PointIndexList);

	//FaceIndex_input may be the Index of a deleted face, so to reused old FaceIndex if necessary
	int_max AddFaceByPoint(const DenseVector<int_max>& PointIndexList, int_max FaceIndex_input);

	// point/edge will be extracted, the order is random
	int_max AddCellByFace(MeshCellTypeEnum Type, const DenseVector<int_max>& FaceIndexList);		
	int_max AddCellByFace(MeshCellTypeEnum Type, const DenseVector<int_max>& FaceIndexList, int_max CellIndex_input);//reuse old CellIndex
	int_max AddCellByFace(const DenseVector<int_max>& FaceIndexList);//Type is Polyhedron
	int_max AddCellByFace(const DenseVector<int_max>& FaceIndexList, int_max CellIndex_input);//Type is Polyhedron

	//specify face-order and point-order in each cell
	//edge-order in the cell is determined by the point-order
	int_max AddCellByPointAndFace(MeshCellTypeEnum Type, const DenseVector<int_max>& PointIndexList, const DenseVector<int_max>& FaceIndexList);	
	int_max AddCellByPointAndFace(MeshCellTypeEnum Type, const DenseVector<int_max>& PointIndexList, const DenseVector<int_max>& FaceIndexList, int_max CellIndex_input);//reuse old CellIndex
	int_max AddCellByPointAndFace(const DenseVector<int_max>& PointIndexList, const DenseVector<int_max>& FaceIndexList);
	int_max AddCellByPointAndFace(const DenseVector<int_max>& PointIndexList, const DenseVector<int_max>& FaceIndexList, int_max CellIndex_input);

    // Delete Mesh Item ----------------------------------------------------------------------------//

	// m_MeshData->FaceList[CellIndex].Clear() only release memory
	// this function will modify any information related to the cell
	// CellIndex, CellName become invalid after the cell is deleted
	void DeleteCell(int_max CellIndex);
	void DeleteCell(const DenseVector<int_max>& CellIndexList);

    // m_MeshData->FaceList[FaceIndex].Clear() only release memory
    // this function will modify any information related to the face
    // FaceIndex, FaceID, FaceName become invalid after the face is deleted
	void DeleteFace(int_max FaceIndex);
	void DeleteFace(const DenseVector<int_max>& FaceIndexList);

    // m_MeshData->EdgeList[EdgeIndex].Clear() only release memory
    // this function will modify any information related to the Edge
    // EdgeIndex, EdgeID, EdgeName become invalid after the edge is deleted
    // Check is performed in the function to make sure an edge can not be deleted if any adjacent face exit
	//
	// note: Call Edge(EdgeIndex).GetAdjacentFaceCount() to check if the edge can be deleted or not
	void DeleteEdge(int_max EdgeIndex);
	void DeleteEdge(const DenseVector<int_max>& EdgeIndexList);

    // m_MeshData->PointList[PointIndex].Cear() only release memory
    // this function will modify any information related to the point
    // Check is performed in the function to make sure a point can not be deleted if any adjacent edge exit	
    // PointIndex, PointID, PointName become invalid after the point is deleted
	//
	// note: Call Point(PointIndex).IsOnEdge() to check if the point can be deleted or not
	void DeletePoint(int_max PointIndex);
	void DeletePoint(const DenseVector<int_max>& PointIndexList);

    // remove deleted item from object list ----------------------------------------------------------//
    // attention: after CleanDataStructure() is called, Index may become invalid
    // use this function when DeletedPointCount/ValidPointCount( GetPointCount() ) > 0.5
    void CleanDataStructure();
	void CleanDataStructure(DenseVector<int_max>& PointIndexMap_Old_To_New, DenseVector<int_max>& EdgeIndexMap_Old_To_New, DenseVector<int_max>& FaceIndexMap_Old_To_New);
	void CleanDataStructure(DenseVector<int_max>& PointIndexMap_Old_To_New, DenseVector<int_max>& EdgeIndexMap_Old_To_New,
		                    DenseVector<int_max>& FaceIndexMap_Old_To_New, DenseVector<int_max>& CellIndexMap_Old_To_New);
	bool Check_If_DataStructure_is_Clean() const;//true: clean, false: contain deleted point/edge/face i.e. invalid index exist
    int_max GetDeletedPointCount() const; // the number of Deleted point
	int_max GetDeletedEdgeCount() const; // the number of Deleted edge
	int_max GetDeletedFaceCount() const; // the number of Deleted face
	int_max GetDeletedCellCount() const; // the number of Deleted cell

    //---------------------------------------------------------------------------------------------------
	
	//MaxGraphDistance is the edge count
	//Output.first[k] is PointIndex
	//Output.second[k] is GraphDistance between Point(PointIndex) and Point(PointIndex_input)
	std::pair<DenseVector<int_max>, DenseVector<int_max>> GetNeighborPointOfPoint(int_max PointIndex_ref, int_max MaxGraphDistance) const;

    // get a sub mesh by FaceIndexList, output polygon mesh
    Mesh<ScalarType> GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const;

	// get a sub mesh by CellIndexList
	Mesh<ScalarType> GetSubMeshByCell(const DenseVector<int_max>& CellIndexList) const;

	// append a mesh, duplication of cell/face/edge/point will not be checked
	void Append(const Mesh<ScalarType>& InputMesh);

    // other basic operation ----------------------------------------------------------------------------------------
	// for now, only support polygon mesh

	// swap PointA and PointB: move A to B and move B to A
	// NOT change the mesh topology
	// A, B, C, D are point index/name
	//------------------------
	//   C          C
	//   |          |    
	//   A   B  =>  B   A
	//       |          |
	//       D          D
    //------------------------	
	bool SwapPoint(int_max PointIndexA, int_max PointIndexB);

	// swap the connection: NOT move A or B, Change the mesh topology
	// A, B, C, D are point ID/name
	//------------------------
	//   C         C 
	//   |           \ 
	//   A   B  => A   B
	//       |       \
	//       D         D
	//------------------------------
	bool SwapConnectivityOfPoint(int_max PointIndexA, int_max PointIndexB);

	// merge the connection of PointB to the connection of PointA, then PointB become an isolated point
	// if an edge between A and B exist, then it will be deleted
	// return true if success
	// can NOT merge two point of a triangle face: return false
	//--------------------------------
	//    |   |          | /
	// ---A---B =>    ---A   B
	//    |   |          | \
    //-------------------------------
	// special case: may produce duplicated edge
	// C---B                 C   B
	// |  /   Merge(A, B)    ||
	// A                     A
	// ABC may not be a face, i.e., ABC is a hole
	// Flag_CheckTriangle= ture then check is performed in the function to prevent this case
	//------------------------------
	bool MergeConnectivityOfPoint(int_max PointIndexA, int_max PointIndexB, bool Flag_CheckTriangle = false);

	// Point may not be on the edge
	// return true if success
	bool ShrinkEdgeToPoint(int_max EdgeIndex, int_max PointIndex);

	DenseVector<int_max, 2> SplitEdgeByPoint(int_max EdgeIndex, int_max PointIndex);

	// if sucess, return the index of the newly merged edge
	// int_max MergeAdjacentEdge(int_max EdgeIndexA, int_max EdgeIndexB);
	// int_max MergeAdjacentEdge(int_max EdgeIndexA, int_max EdgeIndexB, int_max EdgeIndex_A_or_B_or_Other);

	// Point may not be on the face
	// return true if success
	// can NOT shrink if it lead to merge of two point of a triangle face/hole: return false
	bool ShrinkFaceToPoint(int_max FaceIndex, int_max PointIndex);

	// if sucess, return the index of the newly merged face
	// int_max MergeAdjacentFace(int_max FaceIndexA, int_max FaceIndexB);
	// int_max MergeAdjacentFace(int_max FaceIndexA, int_max FaceIndexB, int_max FaceIndex_A_or_B_or_Other);

	//PointA, PointB belong to the face
	DenseVector<int_max, 2> SplitFace(int_max FaceIndex, int_max PointIndexA, int_max PointIndexB);

	DenseVector<int_max, 2> SplitFaceByEdge(int_max FaceIndex, int_max EdgeABIndex);

	// bool ShrinkCellToPoint(int_max CellIndex, int_max PointIndex)
	// int_max MergeAdjacentCell(int_max CellIndexA, int_max CellIndexB);
	// int_max MergeAdjacentCell(int_max CellIndexA, int_max CellIndexB, int_max CellIndex_A_or_B_or_Other);

protected:
    void UpdateRecord_DeletePoint(int_max PointIndex);
    void UpdateRecord_DeleteEdge(int_max EdgeIndex);
    void UpdateRecord_DeleteFace(int_max FaceIndex);
	void UpdateRecord_DeleteCell(int_max CellIndex);
};

}// namespace mdk

#include "mdkMesh.hpp"
#include "mdkMesh_FileIO.h"
