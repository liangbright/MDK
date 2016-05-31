#pragma once

namespace mdk
{

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh)
{
	typedef typename MeshAttributeType::ScalarType ScalarType;
	
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
	DenseVector<int_max> PointIndexList_init;
	PointIndexList_init.SetCapacity(PointCount_input);
	int_max PointIndex_output_init = -1;
	for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		PointIndexList_init.Append(OutputMesh.AddPoint(Pos));
		PointIndex_output_init += 1;
		PointIndexMap_init[it.GetPointIndex()] = PointIndex_output_init;
	}

	//------- add new point by splitting each edge of InputMesh -----------------//   
	DenseVector<int_max> PointIndexMap_new;
	PointIndexMap_new.Resize(EdgeCount_input + InputMesh.GetDeletedEdgeCount());
	PointIndexMap_new.Fill(-1);
	DenseVector<int_max> PointIndexList_new;
	PointIndexList_new.SetCapacity(EdgeCount_input);
	int_max PointIndex_output_new = -1;
	for (auto it = InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto TempList = it.Edge().GetPointIndexList();
		auto P0 = InputMesh.GetPointPosition(TempList[0]);
		auto P1 = InputMesh.GetPointPosition(TempList[1]);
		auto P3 = P0 + P1;
		P3 /= ScalarType(2);
		auto H3 = OutputMesh.AddPoint(P3);
		PointIndexList_new.Append(H3);
		PointIndex_output_new += 1;
		PointIndexMap_new[it.GetEdgeIndex()] = PointIndex_output_new;
	}

	//------- add new cell by splitting each cell of InputMesh and add center point ----------------//   
	for (auto it = InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointIndexList_input = it.Face().GetPointIndexList(); // P0, P1, P2, P3
		auto EdgeIndexList_input = it.Face().GetEdgeIndexList();   // P0-P1, P1-P2, P2-P3, P3-P0
		if (PointIndexList_input.GetLength() != 4)
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
		auto H0 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[0]]];
		auto H1 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[1]]];
		auto H2 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[2]]];
		auto H3 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[3]]];
		auto H4 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[0]]];
		auto H5 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[1]]];
		auto H6 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[2]]];
		auto H7 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[3]]];
		
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

		DenseVector<int_max> PointIndexList_a, PointIndexList_b, PointIndexList_c, PointIndexList_d;
		PointIndexList_a = {H0, H4, H8, H7};
		PointIndexList_b = {H4, H1, H5, H8};
		PointIndexList_c = {H8, H5, H2, H6};
		PointIndexList_d = {H7, H8, H6, H3};
		OutputMesh.AddFaceByPoint(PointIndexList_a);
		OutputMesh.AddFaceByPoint(PointIndexList_b);
		OutputMesh.AddFaceByPoint(PointIndexList_c);
		OutputMesh.AddFaceByPoint(PointIndexList_d);
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

	//------- copy all point of InputMesh ----------------//
	DenseVector<int_max> PointIndexMap;
	PointIndexMap.Resize(PointCount_input + InputMesh.GetDeletedPointCount());
	PointIndexMap.Fill(-1);
	DenseVector<int_max> PointIndexList;
	PointIndexList.SetCapacity(PointCount_input);
	int_max PointIndex_output = -1;
	for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		PointIndexList.Append(OutputMesh.AddPoint(Pos));
		PointIndex_output += 1;
		PointIndexMap[it.GetPointIndex()] = PointIndex_output;
	}

	//------- add new cell by splitting each cell of InputMesh ----------------//   
	for (auto it = InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointIndexList_input = it.Face().GetPointIndexList(); // P0, P1, P2, P3
		auto EdgeIndexList_input = it.Face().GetEdgeIndexList();   // P0-P1, P1-P2, P2-P3, P3-P0
		if (PointIndexList_input.GetLength() != 4)
		{
			MDK_Error("Input is NOT QuadMesh @ ConvertQuadMeshToTriangleMesh(...)")			
			return;
		}
		//-----------------
		// 3    2 
		//  
		// 0    1
		//-----------------		
		auto H0 = PointIndexList[PointIndexMap[PointIndexList_input[0]]];
		auto H1 = PointIndexList[PointIndexMap[PointIndexList_input[1]]];
		auto H2 = PointIndexList[PointIndexMap[PointIndexList_input[2]]];
		auto H3 = PointIndexList[PointIndexMap[PointIndexList_input[3]]];

		OutputMesh.AddFaceByPoint(H0, H1, H2);
		OutputMesh.AddFaceByPoint(H0, H2, H3);
	}

	//-----------------------------------------------------------
	int_max NamedPointCount = InputMesh.GetNamedPointCount();
	if (NamedPointCount > 0)
	{
		auto PointNameList = InputMesh.GetValidPointNameList();
		for (int_max k = 0; k < PointNameList.GetLength(); ++k)
		{
			auto PointIndex = InputMesh.GetPointIndexByName(PointNameList[k]);
			OutputMesh.Point(PointIndex).SetName(PointNameList[k]);
		}
	}
	//----------------------------------------------------------
	auto PointSetCount = InputMesh.GetPointSetCount();
	if (PointSetCount > 0)
	{
		auto PointSetNameList = InputMesh.GetPointSetName(ALL);
		for (int_max PointSetIndex = 0; PointSetIndex < PointSetCount; ++PointSetIndex)
		{
			auto PointSet = InputMesh.GetPointSet(PointSetIndex);
			OutputMesh.SetPointSet(PointSetNameList[PointSetIndex], PointSet);
		}
	}
}

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(const PolygonMesh<MeshAttributeType>& InputMesh)
{
	typedef typename MeshAttributeType::ScalarType ScalarType;	
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
	DenseVector<int_max> PointIndexList_init;
	PointIndexList_init.SetCapacity(PointCount_input);
	int_max PointIndex_output_init = -1;
	for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		PointIndexList_init.Append(OutputMesh.AddPoint(Pos));
		PointIndex_output_init += 1;
		PointIndexMap_init[it.GetPointIndex()] = PointIndex_output_init;
	}

	//------- add new point by splitting each edge of InputMesh -----------------//   
	DenseVector<int_max> PointIndexMap_new;
	PointIndexMap_new.Resize(EdgeCount_input + InputMesh.GetDeletedEdgeCount());
	PointIndexMap_new.Fill(-1);
	DenseVector<int_max> PointIndexList_new;
	PointIndexList_new.SetCapacity(EdgeCount_input);
	int_max PointIndex_output_new = -1;
	for (auto it = InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto TempList = it.Edge().GetPointIndexList();
		auto P0 = InputMesh.GetPointPosition(TempList[0]);
		auto P1 = InputMesh.GetPointPosition(TempList[1]);
		auto P3 = P0 + P1;
		P3 /= ScalarType(2);
		auto H3 = OutputMesh.AddPoint(P3);
		PointIndexList_new.Append(H3);
		PointIndex_output_new += 1;
		PointIndexMap_new[it.GetEdgeIndex()] = PointIndex_output_new;
	}

	//------- add new cell by splitting each cell of InputMesh and add center point ----------------//   
	for (auto it = InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointIndexList_input = it.Face().GetPointIndexList(); // P0, P1, P2, P3
		auto EdgeIndexList_input = it.Face().GetEdgeIndexList();   // P0-P1, P1-P2, P2-P3, P3-P0

		if (PointIndexList_input.GetLength() == 4)
		{
			//-----------------
			// 3    6     2 
			// 7    8     5   
			// 0    4     1
			//-----------------		
			auto H0 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[0]]];
			auto H1 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[1]]];
			auto H2 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[2]]];
			auto H3 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[3]]];
			auto H4 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[0]]];
			auto H5 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[1]]];
			auto H6 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[2]]];
			auto H7 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[3]]];

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
		else if (PointIndexList_input.GetLength() == 3)
		{
			//-----------------
			//      0
			//   3     5			
			//      6
			// 1    4     2
			//-----------------		
			auto H0 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[0]]];
			auto H1 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[1]]];
			auto H2 = PointIndexList_init[PointIndexMap_init[PointIndexList_input[2]]];
			auto H3 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[0]]];
			auto H4 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[1]]];
			auto H5 = PointIndexList_new[PointIndexMap_new[EdgeIndexList_input[2]]];

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

template<typename MeshAttributeTypeA, typename MeshAttributeTypeB>
void ConvertMixedTriangleQuadMeshToTriangleMesh(const PolygonMesh<MeshAttributeTypeA>& InputMesh, TriangleMesh<MeshAttributeTypeB>& OutputMesh)
{
	OutputMesh.Clear();

	auto PointCount_input = InputMesh.GetPointCount();
	auto EdgeCount_input = InputMesh.GetEdgeCount();
	auto FaceCount_input = InputMesh.GetFaceCount();
	auto PointCount = PointCount_input;
	auto EdgeCount = EdgeCount_input + FaceCount_input;
	auto FaceCount = FaceCount_input * 2;
	OutputMesh.SetCapacity(PointCount, EdgeCount, FaceCount);

	//------- copy all point of InputMesh ----------------//
	DenseVector<int_max> PointIndexMap;
	PointIndexMap.Resize(PointCount_input + InputMesh.GetDeletedPointCount());
	PointIndexMap.Fill(-1);
	DenseVector<int_max> PointIndexList;
	PointIndexList.SetCapacity(PointCount_input);
	int_max PointIndex_output = -1;
	for (auto it = InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto Pos = it.Point().GetPosition();
		PointIndexList.Append(OutputMesh.AddPoint(Pos));
		PointIndex_output += 1;
		PointIndexMap[it.GetPointIndex()] = PointIndex_output;
	}

	//------- add new cell by splitting each cell of InputMesh ----------------//   
	for (auto it = InputMesh.GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto PointIndexList_input = it.Face().GetPointIndexList(); // P0, P1, P2, P3
		auto EdgeIndexList_input = it.Face().GetEdgeIndexList();   // P0-P1, P1-P2, P2-P3, P3-P0
		if (PointIndexList_input.GetLength() == 4)
		{
			//-----------------
			// 3    2 
			//  
			// 0    1
			//-----------------		
			auto H0 = PointIndexList[PointIndexMap[PointIndexList_input[0]]];
			auto H1 = PointIndexList[PointIndexMap[PointIndexList_input[1]]];
			auto H2 = PointIndexList[PointIndexMap[PointIndexList_input[2]]];
			auto H3 = PointIndexList[PointIndexMap[PointIndexList_input[3]]];

			OutputMesh.AddFaceByPoint(H0, H1, H2);
			OutputMesh.AddFaceByPoint(H0, H2, H3);
		}
		else if (PointIndexList_input.GetLength() == 3)
		{
			// do nothing
		}
		else
		{
			MDK_Error("Input is NOT QuadMesh @ ConvertMixedTriangleQuadMeshToTriangleMesh(...)")
			return;
		}
	}

	//-----------------------------------------------------------
	auto PointCount = InputMesh.GetPointCount();
	int_max NamedPointCount = InputMesh.GetNamedPointCount();
	auto PointSetCount = InputMesh.GetPointSetCount();
	//----------------------------------------------------------
	if (NamedPointCount > 0)
	{
		auto PointNameList = InputMesh.GetValidPointNameList();
		for (int_max k = 0; k < PointNameList.GetLength(); ++k)
		{
			auto PointIndex = InputMesh.GetPointIndexByName(PointNameList[k]);
			OutputMesh.Point(PointIndex).SetName(PointNameList[k]);
		}
	}
	//----------------------------------------------------------
	if (PointSetCount > 0)
	{	
		auto PointSetNameList = InputMesh.GetPointSetName(ALL);
		for (int_max PointSetIndex = 0; PointSetIndex < PointSetCount; ++PointSetIndex)
		{
			auto PointSet = InputMesh.GetPointSet(PointSetIndex);
			OutputMesh.SetPointSet(PointSetNameList[PointSetIndex], PointSet);
		}
	}
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> CreateQuadMeshOfRectangularFlatSurface(int_max PointCount_x, int_max PointCount_y, double Spacing_x, double Spacing_y)
{
	typedef PolygonMesh<MeshAttributeType>::ScalarType ScalarType;

	PolygonMesh<MeshAttributeType> OutputMesh;

	if (PointCount_x <= 0 || PointCount_y <= 0)
	{
		MDK_Error("invalid input @ CreateQuadMeshOfRectangularFlatSurface(...)")
		return OutputMesh;
	}
	int_max PointCount = PointCount_x*PointCount_y;
	OutputMesh.SetCapacity(PointCount, PointCount / 2, PointCount / 4 + 4);

	DenseVector<int_max> PointIndexList_a, PointIndexList_b;
	PointIndexList_a.Resize(PointCount_x);
	PointIndexList_b.Resize(PointCount_x);
	for (int_max x = 0; x < PointCount_x; ++x)
	{
		PointIndexList_a[x] = OutputMesh.AddPoint(ScalarType(x)*Spacing_x, ScalarType(0), ScalarType(0));
	}
	PointIndexList_b = PointIndexList_a;

	for (int_max y = 1; y < PointCount_y; ++y)
	{
		for (int_max x = 0; x < PointCount_x; ++x)
		{
			PointIndexList_a[x] = OutputMesh.AddPoint(ScalarType(x)*Spacing_x, ScalarType(y)*Spacing_y, ScalarType(0));
		}

		for (int_max x = 0; x < PointCount_x - 1; ++x)
		{
			auto P0 = PointIndexList_b[x];
			auto P1 = PointIndexList_b[x+1];
			auto P2 = PointIndexList_a[x+1];
			auto P3 = PointIndexList_a[x];
			OutputMesh.AddFaceByPoint({ P0, P1, P2, P3 });
		}

		PointIndexList_b = PointIndexList_a;
	}

	return OutputMesh;
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> CreateQuadMeshOfCylinderSurface(int_max PointCountPerRing, int_max RingCount, double Radius, double Height)
{
	typedef PolygonMesh<MeshAttributeType>::ScalarType ScalarType;

	PolygonMesh<MeshAttributeType> OutputMesh;

	if (PointCountPerRing <= 2 || RingCount <= 0)
	{
		MDK_Error("invalid input @ CreateQuadMeshOfCylinderSurface(...)")
		return OutputMesh;
	}

	auto PointCount = PointCountPerRing*RingCount;
	OutputMesh.SetCapacity(PointCount, PointCount / 2, PointCount / 4 + 4);

	DenseVector<int_max> PointIndexList_a, PointIndexList_b;
	PointIndexList_a.Resize(PointCountPerRing+1);
	PointIndexList_b.Resize(PointCountPerRing+1);
	
	double theta = 2*3.141592654 / double(PointCountPerRing);

	for (int_max k = 0; k < PointCountPerRing; ++k)
	{
		ScalarType x = Radius*std::cos(theta*k);
		ScalarType y = Radius*std::sin(theta*k);
		PointIndexList_a[k] = OutputMesh.AddPoint(x, y, ScalarType(0));
	}
	PointIndexList_a[PointCountPerRing] = PointIndexList_a[0];
	PointIndexList_b = PointIndexList_a;

	for (int_max n = 1; n < RingCount; ++n)
	{
		for (int_max k = 0; k < PointCountPerRing; ++k)
		{
			ScalarType x = Radius*std::cos(theta*k);
			ScalarType y = Radius*std::sin(theta*k);
			ScalarType z = (Height / ScalarType(RingCount-1))*ScalarType(n);
			PointIndexList_a[k] = OutputMesh.AddPoint(x, y, z);
		}
		PointIndexList_a[PointCountPerRing] = PointIndexList_a[0];

		for (int_max k = 0; k < PointCountPerRing; ++k)
		{
			auto P0 = PointIndexList_b[k];
			auto P1 = PointIndexList_b[k + 1];
			auto P2 = PointIndexList_a[k + 1];
			auto P3 = PointIndexList_a[k];
			OutputMesh.AddFaceByPoint({ P0, P1, P2, P3 });
		}

		PointIndexList_b = PointIndexList_a;
	}

	return OutputMesh;
}


}//namespace mdk
