#ifndef mdk_GeodesicSphereBuilder_hpp
#define mdk_GeodesicSphereBuilder_hpp

namespace mdk
{

template<typename ScalarType>
GeodesicSphereBuilder<ScalarType>::GeodesicSphereBuilder()
{
	this->Clear();
}


template<typename ScalarType>
GeodesicSphereBuilder<ScalarType>::~GeodesicSphereBuilder()
{
}


template<typename ScalarType>
void GeodesicSphereBuilder<ScalarType>::Clear()
{
	m_SphereList.Clear();
	m_MaxDepth = -1;
}


template<typename ScalarType>
void GeodesicSphereBuilder<ScalarType>::SetMaxDepth(int_max MaxDepth)
{
	m_MaxDepth = MaxDepth;
}


template<typename ScalarType>
void GeodesicSphereBuilder<ScalarType>::Update()
{
	if (m_MaxDepth < 0)
	{
		MDK_Error("m_MaxDepth < 0 @ GeodesicSphereBuilder::Update()")
		return;
	}

	m_SphereList.Clear();
	m_SphereList.Resize(m_MaxDepth+1);

	this->BuildInitialSphere();

	for (int_max k = 1; k <= m_MaxDepth; ++k)
	{
		this->BuildSphereAtDepth(k);
	}
}


template<typename ScalarType>
void GeodesicSphereBuilder<ScalarType>::BuildInitialSphere()
{// icosahedron
	const auto X = ScalarType(0.525731112119133606);
	const auto Z = ScalarType(0.850650808352039932);
	ObjectArray<DenseVector<ScalarType, 3>> vdata;//point
	vdata = { { -X, 0.0, Z }, { X, 0.0, Z },  { -X, 0.0, -Z }, { X, 0.0, -Z },
		      { 0.0, Z, X },  { 0.0, Z, -X }, { 0.0, -Z, X },  { 0.0, -Z, -X },
		      { Z, X, 0.0 },  { -Z, X, 0.0 }, { Z, -X, 0.0 },  { -Z, -X, 0.0 }};

	ObjectArray<DenseVector<int, 3>> edata;//cell
	edata = { { 0, 4, 1 },  { 0, 9, 4 },  { 9, 5, 4 },  { 4, 5, 8 },  { 4, 8, 1 },
		      { 8, 10, 1 }, { 8, 3, 10 }, { 5, 3, 8 },  { 5, 2, 3 },  { 2, 7, 3 },
		      { 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, { 11, 0, 6 }, { 0, 1, 6 },
		      { 6, 1, 10 }, { 9, 0, 11 }, { 9, 11, 2 }, { 9, 2, 5 },  { 7, 2, 11 }};

	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;
	PointHandleList.Resize(12);
	for (int_max k = 0; k < 12; ++k)
	{
		PointHandleList[k] = m_SphereList[0].AddPoint(vdata[k]);				
	}

	m_SphereList[0].ReserveCapacity(12, 30, 20);
	for (int_max k = 0; k < 20; ++k)
	{
		auto H0 = PointHandleList[edata[k][0]];
		auto H1 = PointHandleList[edata[k][1]];
		auto H2 = PointHandleList[edata[k][2]];
		m_SphereList[0].AddCellByPoint(H0, H1, H2);
	}
}


template<typename ScalarType>
void GeodesicSphereBuilder<ScalarType>::BuildSphereAtDepth(int_max Depth)
{
	auto PointCount_prev = m_SphereList[Depth - 1].GetPointCount();
	auto EdgeCount_prev = m_SphereList[Depth - 1].GetEdgeCount();
	auto CellCount_prev = m_SphereList[Depth - 1].GetCellCount();
	auto PointCount = PointCount_prev + EdgeCount_prev;
	auto EdgeCount  = EdgeCount_prev*2;
	auto CellCount = CellCount_prev * 4;
	m_SphereList[Depth].ReserveCapacity(PointCount, EdgeCount, CellCount);
	
	//------- add initial point by copying all point of previous Sphere ----------------//
	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList_init;
	PointHandleList_init.Resize(PointCount_prev);
	for (auto it = m_SphereList[Depth - 1].GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Index = it.GetPointHandle().GetIndex();
		auto Pos = it.Point().GetPosition();
		PointHandleList_init[Index] = m_SphereList[Depth].AddPoint(Pos);
	}

	//------- add new point by splitting each edge of previous Sphere -----------------//   
	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList_new;
	for (auto it = m_SphereList[Depth - 1].GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto TempList = it.Edge().GetPointHandleList();
		auto P0 = m_SphereList[Depth - 1].GetPointPosition(TempList[0]);
		auto P1 = m_SphereList[Depth - 1].GetPointPosition(TempList[1]);
		auto P3 = P0 + P1;
		P3 /= P3.L2Norm();
		auto H3 = m_SphereList[Depth].AddPoint(P3);
		PointHandleList_new.Append(H3);
	}

	//------- add cell to currrent Sphere by splitting each cell of previous Sphere ----//   
	for (auto it = m_SphereList[Depth - 1].GetIteratorOfCell(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList_prev = it.Cell().GetPointHandleList(); // P0, P1, P2
		auto EdgeHandleList_prev = it.Cell().GetEdgeHandleList();   // P0-P1, P1-P2, P2-P1
		//-----------------
		//      0
		//    3    5
		// 1    4     2
		//-----------------		
		auto H0 = PointHandleList_init[PointHandleList_prev[0].GetIndex()];
		auto H1 = PointHandleList_init[PointHandleList_prev[1].GetIndex()];
		auto H2 = PointHandleList_init[PointHandleList_prev[2].GetIndex()];
		auto H3 = PointHandleList_new[EdgeHandleList_prev[0].GetIndex()];
		auto H4 = PointHandleList_new[EdgeHandleList_prev[1].GetIndex()];
		auto H5 = PointHandleList_new[EdgeHandleList_prev[2].GetIndex()];
		m_SphereList[Depth].AddCellByPoint(H0, H3, H5);
		m_SphereList[Depth].AddCellByPoint(H3, H1, H4);
		m_SphereList[Depth].AddCellByPoint(H3, H4, H5);
		m_SphereList[Depth].AddCellByPoint(H5, H4, H2);
	}
}


template<typename ScalarType>
TriangleMesh<TriangleMeshEmptyAttributeType<ScalarType>>* GeodesicSphereBuilder<ScalarType>::GetSphereAtDepth(int_max Depth)
{
	if (Depth < 0 || Depth > m_MaxDepth)
	{
		MDK_Error("Depth is invalid @ GeodesicSphereBuilder::GetSphereAtMaxDepth(Depth)")
		return nullptr;
	}
	else
	{
		return &m_SphereList[Depth];
	}
}


template<typename ScalarType>
TriangleMesh<TriangleMeshEmptyAttributeType<ScalarType>>* GeodesicSphereBuilder<ScalarType>::GetSphereAtMaxDepth()
{
	if (m_MaxDepth < 0)
	{
		MDK_Error("m_MaxDepth < 0 @ GeodesicSphereBuilder::GetSphereAtMaxDepth()")
		return nullptr;
	}
	else
	{
		return &m_SphereList[m_MaxDepth];
	}
}


template<typename ScalarType>
ObjectArray<TriangleMesh<TriangleMeshEmptyAttributeType<ScalarType>>>* GeodesicSphereBuilder<ScalarType>::GetSphereList()
{
	return &m_SphereList;
}


}//namespace mdk

#endif