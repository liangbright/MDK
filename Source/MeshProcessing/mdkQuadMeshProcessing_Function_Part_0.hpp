#ifndef mdk_QuadMeshProcessing_Function_Part_0_hpp
#define mdk_QuadMeshProcessing_Function_Part_0_hpp

namespace mdk
{

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh)
{
	typedef typename MeshAttributeType::ScalarType ScalarType;
	typedef Handle_Of_Point_Of_MembraneMesh PointHandleType;
	//----------------------------------------------------------------------------//
	PolygonMesh<MeshAttributeType> OutputMesh;
	
	auto PointCount_input = InputMesh.GetPointCount();
	auto EdgeCount_input = InputMesh.GetEdgeCount();
	auto FaceCount_input = InputMesh.GetFaceCount();
	auto PointCount = PointCount_input + EdgeCount_input + FaceCount_input;
	auto EdgeCount = EdgeCount_input * 2;
	auto FaceCount = FaceCount_input * 4;
	OutputMesh.SetCapacity(PointCount, EdgeCount, FaceCount);

	//------- add initial point by copying all point of InputMesh ----------------//
	DenseVector<int_max> PointIndexMap_init;
	PointIndexMap_init.Resize(PointCount_input + InputMesh.GetDeletedPointCount());
	PointIndexMap_init.Fill(-1);
	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList_init;
	PointHandleList_init.SetCapacity(PointCount_input);
	int_max PointIndex_output_init = -1;
	for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		PointHandleList_init.Append(OutputMesh.AddPoint(Pos));
		PointIndex_output_init += 1;
		PointIndexMap_init[it.GetPointHandle().GetIndex()] = PointIndex_output_init;
	}

	//------- add new point by splitting each edge of InputMesh -----------------//   
	DenseVector<int_max> PointIndexMap_new;
	PointIndexMap_new.Resize(EdgeCount_input + InputMesh.GetDeletedEdgeCount());
	PointIndexMap_new.Fill(-1);
	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList_new;
	PointHandleList_new.SetCapacity(EdgeCount_input);
	int_max PointIndex_output_new = -1;
	for (auto it = InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto TempList = it.Edge().GetPointHandleList();
		auto P0 = InputMesh.GetPointPosition(TempList[0]);
		auto P1 = InputMesh.GetPointPosition(TempList[1]);
		auto P3 = P0 + P1;
		P3 /= ScalarType(2);
		auto H3 = OutputMesh.AddPoint(P3);
		PointHandleList_new.Append(H3);
		PointIndex_output_new += 1;
		PointIndexMap_new[it.GetEdgeHandle().GetIndex()] = PointIndex_output_new;
	}

	//------- add new cell by splitting each cell of InputMesh and add center point ----------------//   
	for (auto it = InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList_input = it.Face().GetPointHandleList(); // P0, P1, P2, P3
		auto EdgeHandleList_input = it.Face().GetEdgeHandleList();   // P0-P1, P1-P2, P2-P3, P3-P0
		if (PointHandleList_input.GetLength() != 4)
		{
			MDK_Error("Input is NOT QuadMesh @ SubdivideQuadMesh_Linear(...)")
			OutputMesh.Clear();
			return OutputMesh;
		}
		//-----------------
		// 3    6     2 
		// 7    8     5   
		// 0    4     1
		//-----------------		
		auto H0 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[0].GetIndex()]];
		auto H1 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[1].GetIndex()]];
		auto H2 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[2].GetIndex()]];
		auto H3 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[3].GetIndex()]];
		auto H4 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[0].GetIndex()]];
		auto H5 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[1].GetIndex()]];
		auto H6 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[2].GetIndex()]];
		auto H7 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[3].GetIndex()]];
		
		DenseVector<ScalarType, 3> Point8;
		{
			auto Point0 = OutputMesh.GetPointPosition(H0);
			auto Point1 = OutputMesh.GetPointPosition(H1);
			auto Point2 = OutputMesh.GetPointPosition(H2);
			auto Point3 = OutputMesh.GetPointPosition(H3);
			auto L02 = (Point2 - Point0).L2Norm();
			auto L13 = (Point3 - Point1).L2Norm();
			if (L02 < L13)
			{
				Point8 = (Point2 + Point0) / ScalarType(2);
			}
			else
			{
				Point8 = (Point3 + Point1) / ScalarType(2);
			}
		}
		auto H8 = OutputMesh.AddPoint(Point8);

		DenseVector<PointHandleType> PointHandleList_a, PointHandleList_b, PointHandleList_c, PointHandleList_d;
		PointHandleList_a = {H0, H4, H8, H7};
		PointHandleList_b = {H4, H1, H5, H8};
		PointHandleList_c = {H8, H5, H2, H6};
		PointHandleList_d = {H7, H8, H6, H3};
		OutputMesh.AddFaceByPoint(PointHandleList_a);
		OutputMesh.AddFaceByPoint(PointHandleList_b);
		OutputMesh.AddFaceByPoint(PointHandleList_c);
		OutputMesh.AddFaceByPoint(PointHandleList_d);
	}
	//--------------------------------------------------------------------------//
	return OutputMesh;
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh, int_max SubdivisionNumber)
{
	PolygonMesh<MeshAttributeType> OutputMesh;
	if (SubdivisionNumber <= 0)
	{
		MDK_Error(" SubdivisionNumber <=0 @ SubdivideQuadMesh_Linear(...,...)")
	}
	else
	{
		OutputMesh = SubdivideQuadMesh_Linear(InputMesh);
		for (int_max k = 1; k < SubdivisionNumber; ++k)//k from 1
		{
			OutputMesh = SubdivideQuadMesh_Linear(OutputMesh);
		}
	}
	return OutputMesh;
}


template<typename MeshAttributeTypeA, typename MeshAttributeTypeB>
void ConvertQuadMeshToTriangleMesh(const PolygonMesh<MeshAttributeTypeA>& InputMesh, TriangleMesh<MeshAttributeTypeB>& OutputMesh)
{	
	OutputMesh.Clear();

	auto PointCount_input = InputMesh.GetPointCount();
	auto EdgeCount_input = InputMesh.GetEdgeCount();
	auto FaceCount_input = InputMesh.GetFaceCount();
	auto PointCount = PointCount_input;
	auto EdgeCount = EdgeCount_input + FaceCount_input;
	auto FaceCount = FaceCount_input * 2;
	OutputMesh.SetCapacity(PointCount, EdgeCount, FaceCount);

	//------- add initial point by copying all point of InputMesh ----------------//
	DenseVector<int_max> PointIndexMap;
	PointIndexMap.Resize(PointCount_input + InputMesh.GetDeletedPointHandleCount());
	PointIndexMap.Fill(-1);
	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList;
	PointHandleList.SetCapacity(PointCount_input);
	int_max PointIndex_output = -1;
	for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		PointHandleList.Append(OutputMesh.AddPoint(Pos));
		PointIndex_output += 1;
		PointIndexMap[it.GetPointHandle().GetIndex()] = PointIndex_output;
	}

	//------- add new cell by splitting each cell of InputMesh ----------------//   
	for (auto it = InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList_input = it.Face().GetPointHandleList(); // P0, P1, P2, P3
		auto EdgeHandleList_input = it.Face().GetEdgeHandleList();   // P0-P1, P1-P2, P2-P3, P3-P0
		if (PointHandleList_input.GetLength() != 4)
		{
			MDK_Error("Input is NOT QuadMesh @ ConvertQuadMeshToTriangleMesh(...)")			
			return;
		}
		//-----------------
		// 3    2 
		//  
		// 0    1
		//-----------------		
		auto H0 = PointHandleList[PointIndexMap[PointHandleList_input[0].GetIndex()]];
		auto H1 = PointHandleList[PointIndexMap[PointHandleList_input[1].GetIndex()]];
		auto H2 = PointHandleList[PointIndexMap[PointHandleList_input[2].GetIndex()]];
		auto H3 = PointHandleList[PointIndexMap[PointHandleList_input[3].GetIndex()]];

		OutputMesh.AddFaceByPoint(H0, H1, H2);
		OutputMesh.AddFaceByPoint(H0, H2, H3);
	}
}

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh)
{
	typedef typename MeshAttributeType::ScalarType ScalarType;
	typedef Handle_Of_Point_Of_MembraneMesh PointHandleType;
	//----------------------------------------------------------------------------//
	PolygonMesh<MeshAttributeType> OutputMesh;

	auto PointCount_input = InputMesh.GetPointCount();
	auto EdgeCount_input = InputMesh.GetEdgeCount();
	auto FaceCount_input = InputMesh.GetFaceCount();
	auto PointCount = PointCount_input + EdgeCount_input + FaceCount_input;
	auto EdgeCount = EdgeCount_input * 2;
	auto FaceCount = FaceCount_input * 4;
	OutputMesh.SetCapacity(PointCount, EdgeCount, FaceCount);

	//------- add initial point by copying all point of InputMesh ----------------//
	DenseVector<int_max> PointIndexMap_init;
	PointIndexMap_init.Resize(PointCount_input + InputMesh.GetDeletedPointCount());
	PointIndexMap_init.Fill(-1);
	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList_init;
	PointHandleList_init.SetCapacity(PointCount_input);
	int_max PointIndex_output_init = -1;
	for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		PointHandleList_init.Append(OutputMesh.AddPoint(Pos));
		PointIndex_output_init += 1;
		PointIndexMap_init[it.GetPointHandle().GetIndex()] = PointIndex_output_init;
	}

	//------- add new point by splitting each edge of InputMesh -----------------//   
	DenseVector<int_max> PointIndexMap_new;
	PointIndexMap_new.Resize(EdgeCount_input + InputMesh.GetDeletedEdgeCount());
	PointIndexMap_new.Fill(-1);
	DenseVector<Handle_Of_Point_Of_MembraneMesh> PointHandleList_new;
	PointHandleList_new.SetCapacity(EdgeCount_input);
	int_max PointIndex_output_new = -1;
	for (auto it = InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto TempList = it.Edge().GetPointHandleList();
		auto P0 = InputMesh.GetPointPosition(TempList[0]);
		auto P1 = InputMesh.GetPointPosition(TempList[1]);
		auto P3 = P0 + P1;
		P3 /= ScalarType(2);
		auto H3 = OutputMesh.AddPoint(P3);
		PointHandleList_new.Append(H3);
		PointIndex_output_new += 1;
		PointIndexMap_new[it.GetEdgeHandle().GetIndex()] = PointIndex_output_new;
	}

	//------- add new cell by splitting each cell of InputMesh and add center point ----------------//   
	for (auto it = InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList_input = it.Face().GetPointHandleList(); // P0, P1, P2, P3
		auto EdgeHandleList_input = it.Face().GetEdgeHandleList();   // P0-P1, P1-P2, P2-P3, P3-P0

		if (PointHandleList_input.GetLength() == 4)
		{
			//-----------------
			// 3    6     2 
			// 7    8     5   
			// 0    4     1
			//-----------------		
			auto H0 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[0].GetIndex()]];
			auto H1 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[1].GetIndex()]];
			auto H2 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[2].GetIndex()]];
			auto H3 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[3].GetIndex()]];
			auto H4 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[0].GetIndex()]];
			auto H5 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[1].GetIndex()]];
			auto H6 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[2].GetIndex()]];
			auto H7 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[3].GetIndex()]];

			DenseVector<ScalarType, 3> Point8;
			{
				auto Point0 = OutputMesh.GetPointPosition(H0);
				auto Point1 = OutputMesh.GetPointPosition(H1);
				auto Point2 = OutputMesh.GetPointPosition(H2);
				auto Point3 = OutputMesh.GetPointPosition(H3);
				auto L02 = (Point2 - Point0).L2Norm();
				auto L13 = (Point3 - Point1).L2Norm();
				if (L02 < L13)// this is good
				{
					Point8 = (Point2 + Point0) / ScalarType(2);
				}
				else
				{
					Point8 = (Point3 + Point1) / ScalarType(2);
				}
			}
			auto H8 = OutputMesh.AddPoint(Point8);

			OutputMesh.AddFaceByPoint({ H0, H4, H8, H7 });
			OutputMesh.AddFaceByPoint({ H4, H1, H5, H8 });
			OutputMesh.AddFaceByPoint({ H8, H5, H2, H6 });
			OutputMesh.AddFaceByPoint({ H7, H8, H6, H3 });
		}
		else if (PointHandleList_input.GetLength() == 3)
		{
			//-----------------
			//      0
			//   3     5			
			//      6
			// 1    4     2
			//-----------------		
			auto H0 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[0].GetIndex()]];
			auto H1 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[1].GetIndex()]];
			auto H2 = PointHandleList_init[PointIndexMap_init[PointHandleList_input[2].GetIndex()]];
			auto H3 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[0].GetIndex()]];
			auto H4 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[1].GetIndex()]];
			auto H5 = PointHandleList_new[PointIndexMap_new[EdgeHandleList_input[2].GetIndex()]];

			auto Point0 = OutputMesh.GetPointPosition(H0);
			auto Point1 = OutputMesh.GetPointPosition(H1);
			auto Point2 = OutputMesh.GetPointPosition(H2);
			auto Point6 = (Point0 + Point1 + Point2) / ScalarType(3);
			auto H6 = OutputMesh.AddPoint(Point6);

			OutputMesh.AddFaceByPoint({ H6, H5, H0, H3 });
			OutputMesh.AddFaceByPoint({ H6, H3, H1, H4 });
			OutputMesh.AddFaceByPoint({ H6, H4, H2, H5 });
		}
		else
		{
			MDK_Error("Input is NOT MixedTriangleQuadMesh @ SubdivideMixedTriangleQuadMeshToQuadMesh(...)")
			OutputMesh.Clear();
			return OutputMesh;
		}
	}
	//--------------------------------------------------------------------------//
	return OutputMesh;
}

}//namespace mdk


#endif